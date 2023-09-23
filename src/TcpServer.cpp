/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 14:50:50 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/23 20:28:56 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "TcpServer.hpp"

TcpServer::TcpServer(void)
{
	this->_epfd = epoll_create1(0);
	if (this->_epfd == -1)
		throw TcpServer::InternalError();
}

TcpServer::~TcpServer(void)
{
	close(this->_epfd);
	return ;
}

void	TcpServer::_processEPOLLOUT(struct epoll_event &ev)
{
	std::cout << "[] EPOLLOUT event" << std::endl;
	Sockets &client = this->_streams[ev.data.fd];
	std::string response("HTTP/1.1 200 OK\nContent-Type: text/plain\n");
	response += "Content-Length: 12\n\nHello world!";

	int	err = write(client.socket, response.c_str(), response.size());
	if (err == -1)
	{
		if (errno != EAGAIN)
			throw TcpServer::InternalError();
	}
	else if (err < static_cast<int>(response.size()))
	{
		//we can do something when write didnt succeed to send the whole response
	}
	else
	{
		std::cout << "[+] Response succesfully sent to client" << std::endl;
		client.changeEvents(EPOLLIN, this->_epfd);
	}
}

void	TcpServer::_processEPOLLIN(struct epoll_event &ev)
{
	std::cout << "[] EPOLLIN event" << std::endl;
	if ((this->_streams.find(ev.data.fd) != this->_streams.end())
			&& this->_streams[ev.data.fd].main)
	{
		Sockets &server = this->_streams[ev.data.fd];
		std::cout << "[+] New connection from port " << server.port
			<< " on server [ " << server.socket << " ]" << std::endl;
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
			if (errno == EAGAIN)
				client.rDone = true;
			else
				throw TcpServer::InternalError();
		}
		else if (bytesRead == 0)
			client.rDone = true;
		else
		{
			buf[bytesRead] = '\0';
			client.request += buf;
			std::cout << "[+] " << bytesRead << " bytes read" << std::endl;
		}
		if (client.rDone)
		{
			std::cout << "[*] Full request received:" << std::endl;
			std::cout << client.request << std::endl;
			client.changeEvents(EPOLLOUT, this->_epfd);
			client.rDone = false;
		}
	}
}

void	TcpServer::_processEvent(struct epoll_event &ev)
{
	std::cout << "@Processing of an event from fd [ " << ev.data.fd << " ]"
		<< std::endl;	
	std::cout << "[] event is ";
	std::cout << std::hex << ev.events << " from fd [ " << ev.data.fd << " ]"
		<<std::endl;
	if ((ev.events & EPOLLIN) == EPOLLIN)
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

		std::cout << "@Waiting for events" << std::endl;
		evNb = epoll_wait(this->_epfd, evlist, MAXEVENT, 20000);
		std::cout << "[] evNb: " << evNb << std::endl;
		if (evNb == -1)
			throw TcpServer::InternalError();
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
		throw TcpServer::InternalError();

	this->_streams[clientFd].info = info;
	this->_streams[clientFd].setup(clientFd, fdServer, -1, false);

	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, clientFd,
				&(this->_streams[clientFd].event)) == -1)
		throw TcpServer::InternalError();
}

void	TcpServer::create(unsigned int port)
{
	int	serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFd == -1)
		throw TcpServer::InternalError();

	this->_streams[serverFd].setup(serverFd, -1, port, true);

	Sockets &server = this->_streams[serverFd];
	if (bind(server.socket, (struct sockaddr *)&server.info,
				server.size) == -1)
		throw TcpServer::InternalError();
	if (listen(server.socket, MAXIREQ) == -1)
		throw TcpServer::InternalError();

	std::cout << "@Server [ " << server.socket << " ] is listenning on port: "
		<< port << std::endl;

	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, server.socket,
				&(this->_streams[server.socket].event)) == -1)
		throw TcpServer::InternalError();
}

const char *TcpServer::InternalError::what(void) const throw()
{
	return (strerror(errno));
}
