/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 14:50:50 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/20 17:19:33 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "TcpServer.hpp"


TcpServer::TcpServer(unsigned int port): _port(port)
{
	this->create(port);
}

TcpServer::~TcpServer(void)
{
	close(this->_server.socket);
	std::cout << "@Server on port " << this->_port << "stopped" << std::endl;
}

void	TcpServer::add_client(void)
{
	//accept a new client connection
}

void	TcpServer::_create(unsigned int port)
{
	this->_server.socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_server.socket == -1)
		throw this->InternalError();

	this->_server.size = sizeof(this->_server.info);
	memset((void*)&this->_server.info, 0, this->_server.size);
	
	this->_server.info.sin_family = AF_INET;
	this->_server.info.sin_addr.s_addr = htonl(INADDR_ANY);
	this->_server.info.sin_port = htons(port);

	if (bind(this->_server.socket, (struct sockaddr *)&this->_server.info,
				this->_server.size) == -1)
		throw this->InternalError();
	if (listen(this->_server.socket, MAXIREQ) == -1)
		throw this->InternalError();
	std::cout << "@Server is listenning on port: " << port << std::endl;
}

const char *TcpServer::InternalError::what(void) const throw
{
	return (strerror(errno));
}
