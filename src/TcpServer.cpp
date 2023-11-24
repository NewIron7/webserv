/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 14:50:50 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/15 07:47:09 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "TcpServer.hpp"

TcpServer::TcpServer(const ConfigurationManager &config): config(config)
{
	this->_epfd = epoll_create1(EPOLL_CLOEXEC);
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
	if (this->_CGIstreams.find(ev.data.fd) != this->_CGIstreams.end())
	{
		Sockets *client = this->_CGIstreams[ev.data.fd];
		CGIprocess &cgi = client->cgi;

		if (cgi.step == 0)
		{
			cgi.sendBody(client->oRequest);
			if (client->oRequest.getErrorCode() == 500)
			{
				this->_endCGI(client);
			}
			else
			{
				this->_remove_cgi(*client, 0);
				this->_add_cgi(*client, 1);
			}
		}

		return ;
	}

	Sockets	&client = this->_streams[ev.data.fd];


	if (client.resSent || client.reqGot == false || client.CGIrun)
		return ;

	if (client.resGen == false)
	{
		std::cout << "\033[33m[+] Generating response\033[0m" << std::endl;
		std::cout << "\033[35m[] Request ->\033[0m" << std::endl;
		std::cout << "\033[2m" << client.request << "\033[0m" << std::endl;

		client.process();
		if (client.CGIrun)
		{
			if (client.cgi.step)
				this->_add_cgi(client, 1);
			else
				this->_add_cgi(client, 0);
			return ;
		}

		client.resGen = true;
		client.request.clear();

		std::cout << "\033[35m[] Response ->\033[0m" << std::endl;
		std::cout << "\033[2m" << client.response << "\033[0m" << std::endl;
	}
	else
		client.request.clear();

	int	err = write(client.socket, client.response.c_str(),
			client.response.size());
	if (err == -1 && (err == 0 && client.response.empty() == false))
	{
		std::cout << "\033[35m[#] Failed to sent response\033[0m" << std::endl;
	}
	else
	{
		client.resSent = true;
		std::cout << "\033[32m[+] Response succesfully sent to client\033[0m" << std::endl;
	}
}

void	TcpServer::_processEPOLLIN(struct epoll_event &ev)
{
	if ((this->_streams.find(ev.data.fd) != this->_streams.end())
			&& this->_streams[ev.data.fd].main)
	{
		Sockets &server = this->_streams[ev.data.fd];
		std::cout << "\033[32;1m[+] New connection from port " << server.port
			<< " on server [ " << server.socket << " ]\033[0m" << std::endl;
		this->_add_client(server.socket);
	}
	else if (this->_CGIstreams.find(ev.data.fd) != this->_CGIstreams.end())
	{
		Sockets	*client = this->_CGIstreams[ev.data.fd];
		CGIprocess	&cgi = client->cgi;

		cgi.readResponse(client->oRequest);
		if (client->oRequest.getErrorCode() == 500)
		{
			this->_endCGI(client);
		}
	}
	else
	{
		Sockets	&client = this->_streams[ev.data.fd];

		char	buf[BUFFER_SIZE];
		memset((void*)buf, 0, BUFFER_SIZE);

		int	bytesRead = read(client.socket, buf, BUFFER_SIZE);
		if (bytesRead == -1)
		{
			client.reqGot = false;
			std::cout << "[#] Failed to read request" << std::endl;
		}
		else
		{
			client.request.insert(client.request.size(), buf, bytesRead);
			std::cout << "\033[36m[+] " << bytesRead << " bytes read\033[0m" << std::endl;
			client.reqGot = true;
			client.resGen = false;
			client.resSent = false;
		}
	}
}

void	TcpServer::_endCGI(Sockets *client)
{
		CGIprocess &cgi = client->cgi;

		if (cgi.isError())
		{
			if (cgi.response.empty())
				client->oRequest.setErrorMsg("Error with cgi");
			client->response = DefaultErrorPages::generate(500,
				client->oRequest.getErrorMsg());
		}
		else
		{
			cgi.addHeaders();
			client->response = cgi.response;
		}
		
		cgi.done = true;
		this->_remove_cgi(*client, 0);
		this->_remove_cgi(*client, 1);

		cgi.endCGI(false);
		client->CGIrun = false;
		client->resGen = true;

		std::cout << "\033[35m[] Response ->\033[0m" << std::endl;
		std::cout << "\033[2m" << client->response << "\033[0m" << std::endl;
}

void	TcpServer::_processEPOLLERR(struct epoll_event &ev)
{
	if (this->_streams.find(ev.data.fd) != this->_streams.end())
	{
		Sockets &client = this->_streams[ev.data.fd];
		std::cout << "\033[31m[-] [ "
			<< client.socket << " ] error threrefore got disconnected\033[0m"
			<< std::endl;
		this->_remove_client(client);
	}
	else
	{
		Sockets *client = this->_CGIstreams[ev.data.fd];
		this->_endCGI(client);
	}
}

void	TcpServer::_processEPOLLHUP(struct epoll_event &ev)
{
	if (this->_streams.find(ev.data.fd) != this->_streams.end())
	{
		Sockets &client = this->_streams[ev.data.fd];
		std::cout << "\033[31m[-] [ " << client.socket << " ] disconnected\033[0m"
			<< std::endl;
		this->_remove_client(client);
	}
	else if (this->_CGIstreams.find(ev.data.fd) != this->_CGIstreams.end())
	{
		Sockets *client = this->_CGIstreams[ev.data.fd];
		this->_endCGI(client);
	}
}

void	TcpServer::_processEvent(struct epoll_event &ev)
{
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
	{
		std::cout << "Event not handle yet by the program" << std::endl;
	}
}

void	TcpServer::run(void)
{
	struct epoll_event	evlist[MAXEVENT];
	int					evNb;

	while (1)
	{
		evNb = 0;
		memset((void*)evlist, 0, sizeof(evlist));

		evNb = epoll_wait(this->_epfd, evlist, MAXEVENT, TEVENT);
		if (evNb == -1)
			throw InternalError();
		if (!evNb)
			continue ;
		for (int i = 0; i < evNb; i++)
		{
			try
			{
				this->_processEvent(evlist[i]);
			}
			catch(const ErrorCGI& e)
			{
				throw;
			}
			catch(const std::exception& e)
			{
				std::cerr << "\033[41m" << e.what() << "\033[0m" << std::endl;
				if (this->_streams.find(evlist[i].data.fd) != this->_streams.end())
				{
					Sockets &client = this->_streams[evlist[i].data.fd];
					client.reqGot = false;
					client.resGen = false;
					client.resSent = false;
					if (client.CGIrun == true)
					{
						client.cgi.endCGI(true);
					}
					client.CGIrun = false;
					client.request.clear();
					client.response.clear();
				}
			}
		}
	}
}

void	TcpServer::_add_client(const int &fdServer)
{
	struct sockaddr_in info;
	socklen_t	size = sizeof(info);
	memset((void*)&info, 0, size);
	const Sockets &server = this->_streams[fdServer];

	int	clientFd = accept(fdServer, (struct sockaddr *)&info, &size);
	if (clientFd == -1)
	{
		if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
			return ;
		throw InternalError();
	}

	this->_streams[clientFd].info = info;

	Sockets &client = this->_streams[clientFd];
	client.setup(clientFd, fdServer, server.port, false, server.host, server.config);

	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, client.socket,
				&client.event) == -1)
	{
		this->_streams.erase(client.socket);
		throw InternalError();
	}
}

void	TcpServer::_add_cgi(Sockets &client, unsigned int nb)
{
	CGIprocess	&cgi = client.cgi;

	cgi.size = sizeof(cgi.info);
	memset((void*)&cgi.info, 0, cgi.size);

	this->_CGIstreams[cgi.fds[nb]] = &client;
	cgi.clientFd = client.socket;

	if (nb == 0)
		cgi.event[0].events = EPOLLOUT;
	else
		cgi.event[1].events = EPOLLIN;

	cgi.event[nb].events |= EPOLLRDHUP;
	cgi.event[nb].events |= EPOLLPRI;

	cgi.event[nb].data.fd = cgi.fds[nb];


	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, cgi.fds[nb],
				&cgi.event[nb]) == -1)
	{
		this->_CGIstreams.erase(cgi.fds[nb]);
		throw InternalError();
	}
}

void	TcpServer::_remove_cgi(Sockets &client, unsigned int nb)
{
	CGIprocess	&cgi = client.cgi;

	if (this->_CGIstreams.find(cgi.fds[nb]) == this->_CGIstreams.end())
		return ;

	this->_CGIstreams.erase(cgi.fds[nb]);

	if ((cgi.c == false) && (epoll_ctl(this->_epfd, EPOLL_CTL_DEL, cgi.fds[nb],
					&cgi.event[nb]) == -1))
	{
		std::cout << "Error while removing cgi from epoll" << std::endl;
		throw InternalError();
	}
}

void	TcpServer::_remove_client(Sockets &client)
{
	int	inFd = client.cgi.fds[0];
	int	outFd = client.cgi.fds[1];

	if (this->_CGIstreams.find(inFd) != this->_CGIstreams.end())
	{
		this->_remove_cgi(client, 0);
	}
	if (this->_CGIstreams.find(outFd) != this->_CGIstreams.end())
	{
		this->_remove_cgi(client, 1);
	}
	
	if (epoll_ctl(this->_epfd, EPOLL_CTL_DEL, client.socket,
				&client.event) == -1)
	{
		if (this->_streams.find(client.socket) != this->_streams.end())
			this->_streams.erase(client.socket);

		std::cerr << "Error while deleting client from epoll" << std::endl;
		throw InternalError();
	}

	if (this->_streams.find(client.socket) != this->_streams.end())
		this->_streams.erase(client.socket);
}

static void extractHostAndPort(const std::string& text, std::string& host, unsigned int& port) {
    size_t lastColonPos = text.find_last_of(':');

    if (lastColonPos != std::string::npos && lastColonPos + 1 < text.length()) {
        host = text.substr(0, lastColonPos);
        std::string portStr = text.substr(lastColonPos + 1);

		std::stringstream ss(portStr);
		ss >> port;
    } else {
        std::cerr << "Invalid format: Cannot extract host and port from input text." << std::endl;
    }
}

void	TcpServer::_createServer(std::string host, unsigned int port,
		const std::vector<ConfigurationObject> &config)
{
	int	serverFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverFd == -1)
		throw InternalError();

	int	y = 1;
	if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int)) == -1)
		throw InternalError();

	this->_streams[serverFd].setup(serverFd, -1, port, true, host, config);

	std::cout << "\033[2m@Server binding\033[0m" << std::endl;
	Sockets &server = this->_streams[serverFd];
	if (bind(server.socket, (struct sockaddr *)&server.info,
				server.size) == -1)
		throw InternalError();
	if (listen(server.socket, MAXIREQ) == -1)
		throw InternalError();

	std::cout << "\033[44m@Server [ " << server.socket << " ] " << server.host
		<< ":" << server.port << "\033[0m" << std::endl;
	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, server.socket,
				&(this->_streams[server.socket].event)) == -1)
		throw InternalError();

}

void	TcpServer::create(void)
{

	std::cout << "\033[2m@Got hold of Config\033[0m" << std::endl;
	this->config.printConfig();

	for (std::map<std::string, std::vector<ConfigurationObject> >::const_iterator
			it = this->config.config.begin();
			it != this->config.config.end(); ++it)
	{
		std::cout << "\033[2m@Server creation: " << it->first << "\033[0m" << std::endl;
		std::string host;
		unsigned int port;
		extractHostAndPort(it->first, host, port);
		if (host.empty())
			throw InternalError();
		this->_createServer(host, port, it->second);
	}
}

