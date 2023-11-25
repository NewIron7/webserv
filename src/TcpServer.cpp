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
		{
			std::cerr << "ERROR epoll_wait" << std::endl;
			throw InternalError();
		}
			
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
			catch(const InternalError& e)
			{
				if (this->_streams.find(evlist[i].data.fd) != this->_streams.end())
				{
					Sockets &client = this->_streams[evlist[i].data.fd];
					if (client.CGIrun == true)
					{
						client.cgi.endCGI(true);
					}
					throw;
				}
			}
			catch(const std::exception& e)
			{
				//std::cerr << "\033[41m" << e.what() << "\033[0m" << std::endl;
				if (this->_streams.find(evlist[i].data.fd) != this->_streams.end())
				{
					Sockets &client = this->_streams[evlist[i].data.fd];
					this->_endCGI(&client);
					client.response = DefaultErrorPages::generate(500,
						client.oRequest.getErrorMsg());
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
		std::cerr << "Error while accepting client" << std::endl;
		throw InternalError();
	}

	this->_streams[clientFd].info = info;

	Sockets &client = this->_streams[clientFd];
	client.setup(clientFd, fdServer, server.port, false, server.host, server.config);
	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, client.socket,
				&client.event) == -1)
	{
		std::cerr << "Error while addind client to epoll" << std::endl;
		this->_streams.erase(client.socket);
		throw InternalError();
	}

}

void	TcpServer::_remove_client(Sockets &client)
{
	if (this->_streams.find(client.socket) != this->_streams.end())
	{
		this->_endCGI(&client);

		if (epoll_ctl(this->_epfd, EPOLL_CTL_DEL, client.socket,
					&client.event) == -1)
		{
			this->_streams.erase(client.socket);

			std::cerr << "Error while deleting client from epoll" << std::endl;
			throw InternalError();
		}

		this->_streams.erase(client.socket);
	}
	
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
	{
		std::cerr << "ERROR while binding the server" << std::endl;
		throw InternalError();
	}
	if (listen(server.socket, MAXIREQ) == -1)
	{
		std::cerr << "ERROR while starting listenning the server" << std::endl;
		throw InternalError();
	}

	std::cout << "\033[44m@Server [ " << server.socket << " ] " << server.host
		<< ":" << server.port << "\033[0m" << std::endl;
	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, server.socket,
				&(this->_streams[server.socket].event)) == -1)
	{
		std::cerr << "ERROR while adding the server to epoll" << std::endl;
		throw InternalError();
	}
}

void	TcpServer::create(void)
{

	std::cout << "\033[2m@Got hold of Config\033[0m" << std::endl;

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

