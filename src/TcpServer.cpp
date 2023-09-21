/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 14:50:50 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/21 17:19:29 by hboissel         ###   ########.fr       */
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
	for (std::vector<t_connect>::iterator it = this->_streams.begin();
			it != this->_streams.end(); it++)
	{
		close(it->socket);
	}
}

void	TcpServer::run(void)
{
	//running loop of servers
}

void	TcpServer::_add_client(const t_connect & server)
{
	t_connect	client;

	client.main = false;
	client.server = server;
	client.size = sizeof(client.info);
	memset((void*)&client.info, 0, client.size);
	client.socket = accept(client.server.socket, (struct sockaddr *)&client.info,
			&client.size);
	if (client.socket == -1)
		throw TcpServer::InternalError();
	this->_streams.push_back(client);

	struct epoll_event	event;
	event.data.ptr = reinterpret_cast<void*>&this->_streams.back();
	this->_events.push_back(event);
	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, server.socket,
				&this->_events.back()) == -1)
		throw TcpServer::InternalError();
}

void	TcpServer::create(unsigned int port)
{
	t_connect	server;
	server.main = true;
	server.server = server;
	server.port = port;
	server.socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server.socket == -1)
		throw TcpServer::InternalError();

	server.size = sizeof(server.info);
	memset((void*)&server.info, 0, server.size);
	
	server.info.sin_family = AF_INET;
	server.info.sin_addr.s_addr = htonl(INADDR_ANY);
	server.info.sin_port = htons(port);

	if (bind(server.socket, (struct sockaddr *)&server.info,
				server.size) == -1)
		throw TcpServer::InternalError();
	if (listen(server.socket, MAXIREQ) == -1)
		throw TcpServer::InternalError();
	std::cout << "@Server [" << server.socket << "] is listenning on port: "
		<< port << std::endl;
	this->_streams.push_back(server);

	struct epoll_event	event;
	event.data.ptr = reinterpret_cast<void*>&this->_streams.back();
	this->_events.push_back(event);
	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, server.socket,
				&this->_events.back()) == -1)
		throw TcpServer::InternalError();
}

const char *TcpServer::InternalError::what(void) const throw()
{
	return (strerror(errno));
}
