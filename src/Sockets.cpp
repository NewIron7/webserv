/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 12:19:06 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/27 20:22:43 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Sockets.hpp"

Sockets::Sockets(void):
	main(false), server(-1), socket(-1), port(-1), reqGot(false), resGen(false)
	,resSent(false)
{
	this->size = sizeof(this->info);
	memset((void*)&this->info, 0, this->size);
	this->event.events = EPOLLIN | EPOLLOUT;
	this->event.events |= EPOLLRDHUP;
	this->event.events |= EPOLLPRI;
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
	
	if (this->main)
	{
		this->info.sin_family = AF_INET;
		this->info.sin_addr.s_addr = htonl(INADDR_ANY);
		this->info.sin_port = htons(port);
	}

	int f = fcntl(this->socket, F_GETFL, 0);
	if (f == -1)
		throw Sockets::InternalError();
	f |= O_NONBLOCK;
	if (fcntl(this->socket, F_SETFL, f) == -1)
		throw Sockets::InternalError();
}

void	Sockets::changeEvents(uint32_t ev, int epfd)
{
	this->event.events = ev;
	if (epoll_ctl(epfd, EPOLL_CTL_MOD, this->socket,
				&this->event) == -1)
		throw Sockets::InternalError();
}

const char *Sockets::InternalError::what(void) const throw()
{
	std::string err = "\033[31m";
	err += strerror(errno);
	err += "\033[0m";
	return (err.c_str());
}
