/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 14:50:50 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/29 16:55:03 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "TcpServer.hpp"

TcpServer::TcpServer(void)
{
	this->_epfd = epoll_create1(0);
	if (this->_epfd == -1)
		throw InternalError();
}

TcpServer::~TcpServer(void)
{
	close(this->_epfd);
	return ;
}

void	TcpServer::_processEPOLLOUT(struct epoll_event &ev)
{
	Sockets	&client = this->_streams[ev.data.fd];

	//std::cout << "[] EPOLLOUT event" << std::endl;
	
	if (client.resSent)
	{
		//std::cout << "\033[33m[-] Response already sent\033[0m" << std::endl;
		return ;
	}
	if (client.reqGot == false)
	{
		//std::cout << "[-] No request to process" << std::endl;
		return ;
	}

	std::string response;
	if (client.resGen == false)
	{
		std::cout << "\033[33m[+] Generating response\033[0m" << std::endl;
		std::cout << "\033[35m[] Request ->\033[0m" << std::endl;
		std::cout << "\033[2m" << client.request << "\033[0m" << std::endl;


		response = "HTTP/1.1 200 OK\nContent-Type: text/plain\n";
		response += "Content-Length: 12\n\nHello world!";
		
		client.response = response;
		client.resGen = true;
		client.request.clear();
		
		std::cout << "\033[35m[] Response ->\033[0m" << std::endl;
		std::cout << "\033[2m" << client.response << "\033[0m" << std::endl;
	}
	
	int	err = write(client.socket, client.response.c_str(),
			client.response.size());
	if (err == -1)
	{
		std::cout << "\033[35m[#] Failed to sent response\033[0m" << std::endl;
	}
	else if (err < static_cast<int>(response.size()))
	{
		std::cout << "\033[35m[#] Failed to sent response\033[0m" << std::endl;
		//we can do something when write didnt succeed to send the whole response
	}
	else
	{
		client.resSent = true;
		std::cout << "\033[32m[+] Response succesfully sent to client\033[0m" << std::endl;
	}
}

void	TcpServer::_processEPOLLIN(struct epoll_event &ev)
{
	std::cout << "[] EPOLLIN event" << std::endl;
	if ((this->_streams.find(ev.data.fd) != this->_streams.end())
			&& this->_streams[ev.data.fd].main)
	{
		Sockets &server = this->_streams[ev.data.fd];
		std::cout << "\033[32;1m[+] New connection from port " << server.port
			<< " on server [ " << server.socket << " ]\033[0m" << std::endl;
		this->_add_client(server.socket);
	}
	else
	{
		Sockets	&client = this->_streams[ev.data.fd];

		char	buf[BUFFER_SIZE + 1];
		memset((void*)buf, 0, BUFFER_SIZE + 1);

		int	bytesRead = read(client.socket, buf, BUFFER_SIZE);
		if (bytesRead == -1)
		{
			client.reqGot = false;
			std::cout << "[#] Failed to read request" << std::endl;
		}
		else
		{
			buf[bytesRead] = '\0';
			client.request += buf;
			std::cout << "\033[36m[+] " << bytesRead << " bytes read\033[0m" << std::endl;
			client.reqGot = true;
			client.resGen = false;
			client.resSent = false;
		}
	}
}

void	TcpServer::_processEPOLLERR(struct epoll_event &ev)
{
	std::cout << "[] EPOLLERR event" << std::endl;
	Sockets &client = this->_streams[ev.data.fd];
	this->_remove_client(client);
}

void	TcpServer::_processEPOLLHUP(struct epoll_event &ev)
{
	std::cout << "[] EPOLLHUP event" << std::endl;
	Sockets &client = this->_streams[ev.data.fd];
	std::cout << "\033[31m[-] [ " << client.socket << " ] disconnected\033[0m" << std::endl;
	this->_remove_client(client);
}

void	TcpServer::_processEvent(struct epoll_event &ev)
{
	//std::cout << "\033[4m@Processing of an event from fd [ " << ev.data.fd << " ]\033[0m"
	//	<< std::endl;	
	//std::cout << "[] event is ";
	//std::cout << std::hex << ev.events << std::dec << " from fd [ " << ev.data.fd << " ]"
	//	<< std::endl;
	if ((ev.events & EPOLLHUP) == EPOLLHUP
			|| (ev.events & EPOLLRDHUP) == EPOLLRDHUP)
		this->_processEPOLLHUP(ev);
	else if ((ev.events & EPOLLERR) == EPOLLERR)
		this->_processEPOLLERR(ev);
	else if ((ev.events & EPOLLIN) == EPOLLIN
			|| (ev.events & EPOLLPRI) == EPOLLPRI)
		this->_processEPOLLIN(ev);
	else if ((ev.events & EPOLLOUT) == EPOLLOUT)
		this->_processEPOLLOUT(ev);
	else
		std::cout << "Event not handle yet by the program" << std::endl;
}

void	TcpServer::run(void)
{
	struct epoll_event	evlist[MAXEVENT];
	int					evNb;

	while (1)
	{
		evNb = 0;
		memset((void*)evlist, 0, sizeof(evlist));

		//std::cout << "\033[1m@Waiting for events\033[0m" << std::endl;
		evNb = epoll_wait(this->_epfd, evlist, MAXEVENT, TEVENT);
		//std::cout << "[] evNb: " << evNb << std::endl;
		if (evNb == -1)
			throw InternalError();
		if (!evNb)
			continue ;
		for (int i = 0; i < evNb; i++)
			this->_processEvent(evlist[i]);
	}
}

void	TcpServer::_add_client(const int &fdServer)
{
	struct sockaddr_in info;
	socklen_t	size = sizeof(info);
	memset((void*)&info, 0, size);

	int	clientFd = accept(fdServer, (struct sockaddr *)&info, &size);
	if (clientFd == -1)
	{
		if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
			return ;
		throw InternalError();
	}

	this->_streams[clientFd].info = info;

	Sockets &client = this->_streams[clientFd];
	client.setup(clientFd, fdServer, -1, false);

	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, client.socket,
				&client.event) == -1)
		throw InternalError();
}

void	TcpServer::_remove_client(Sockets &client)
{
	if (epoll_ctl(this->_epfd, EPOLL_CTL_DEL, client.socket,
				&client.event) == -1)
		throw InternalError();
	this->_streams.erase(client.socket);
}

void	TcpServer::create(unsigned int port)
{
	std::cout << "\033[2m@Server creation\033[0m" << std::endl;
	int	serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFd == -1)
		throw InternalError();

	int	y = 1;
	if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int)) == -1)
		throw InternalError();

	this->_streams[serverFd].setup(serverFd, -1, port, true);

	std::cout << "\033[2m@Server binding\033[0m" << std::endl;
	Sockets &server = this->_streams[serverFd];
	if (bind(server.socket, (struct sockaddr *)&server.info,
				server.size) == -1)
		throw InternalError();
	if (listen(server.socket, MAXIREQ) == -1)
		throw InternalError();

	std::cout << "\033[44m@Server [ " << server.socket << " ] is listenning on port: "
		<< server.port << "\033[0m" << std::endl;

	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, server.socket,
				&(this->_streams[server.socket].event)) == -1)
		throw InternalError();
}

