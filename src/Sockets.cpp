/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 12:19:06 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/23 14:04:27 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Sockets.hpp"

Sockets::Sockets(void):
	main(false), server(-1), socket(-1), port(-1)
{
	this->size = sizeof(this->info);
	memset((void*)&this->info, 0, this->size);
	this->event.events = EPOLLIN | EPOLLOUT;

	if (this->main)
	{
		this->info.sin_family = AF_INET;
		this->info.sin_addr.s_addr = htonl(INADDR_ANY);
		this->info.sin_port = htons(port);
	}
}

Sockets::~Sockets(void)
{
	if (this->socket != -1)
	{
		std::cout << "@Socket of fd [ " << this->socket << " ] closed" << std::endl;
		close(this->socket);
	}
	return ;
}

Sockets	&Sockets::operator=(Sockets const &rhs)
{
	this->main = rhs.main;
	this->server = rhs.server;
	this->socket = rhs.socket;
	this->info = rhs.info;
	this->size = rhs.size;
	this->port = rhs.port;
	this->event = rhs.event;

	return (*this);
}

void	Sockets::setup(int sock, int sfd, int sp, bool m)
{
	this->socket = sock;
	this->event.data.fd = sock;
	this->main = m;
	this->server = sfd;
	this->port = sp;
}
