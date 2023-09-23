/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 14:50:50 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/23 18:29:23 by hboissel         ###   ########.fr       */
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

void	TcpServer::_processEvent(struct epoll_event &ev)
{
	std::cout << "@Processing of an event from fd [ " << ev.data.fd << " ["
		<< std::endl;	
	std::cout << "[] event is ";
	std::cout << std::hex << ev.events << " from fd [ " << ev.data.fd << " ]"
		<<std::endl;
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
		break ;
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
