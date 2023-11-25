/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 12:19:06 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/15 07:45:02 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Sockets.hpp"

void Sockets::printAttributes() {
    std::cout << "Attributes of the Sockets class:" << std::endl;
    std::cout << "main: " << main << std::endl;
    std::cout << "server: " << server << std::endl;
    std::cout << "socket: " << socket << std::endl;
    
    // Print the sockaddr_in structure (info)
    std::cout << "info.sin_family: " << info.sin_family << std::endl;
    std::cout << "info.sin_port: " << ntohs(info.sin_port) << std::endl;
    std::cout << "info.sin_addr.s_addr: " << inet_ntoa(info.sin_addr) << std::endl;
    // Add more fields from info as needed
    
    std::cout << "size: " << size << std::endl;
    std::cout << "port: " << port << std::endl;

    std::cout << "event.events: " << event.events << std::endl;
    std::cout << "event.data.fd: " << event.data.fd << std::endl;
    
    std::cout << "request: " << request << std::endl;
    
    // You can print the attributes of the oRequest object if it has public members

    std::cout << "reqGot: " << reqGot << std::endl;
    std::cout << "response: " << response << std::endl;
    std::cout << "resGen: " << resGen << std::endl;
    std::cout << "resSent: " << resSent << std::endl;
    std::cout << "CGIrun: " << CGIrun << std::endl;

    // Print the attributes of the cgi object if it has public members
}

Sockets::Sockets(void):
	main(false), server(-1), socket(-1), host("0.0.0.0"), port(0), reqGot(false), resGen(false)
	,resSent(false), CGIrun(false)
{
	this->size = sizeof(this->info);
	memset((void*)&this->info, 0, this->size);
	this->event.events = EPOLLIN | EPOLLOUT;
	this->event.events |= EPOLLRDHUP;
	this->event.events |= EPOLLPRI;

	this->response = "";
}

Sockets::~Sockets(void)
{
	if (this->socket > 2)
	{
		//std::cout << "@Socket of fd [ " << this->socket << " ] closed" << std::endl;
		close(this->socket);
	}
	return ;
}

Sockets::Sockets(const Sockets &src)
{
	*this = src;
}

Sockets	&Sockets::operator=(Sockets const &rhs)
{
	this->main = rhs.main;
	this->server = rhs.server;
	this->socket = rhs.socket;
	this->info = rhs.info;
	this->size = rhs.size;
	this->host = rhs.host;
	this->port = rhs.port;
	this->event = rhs.event;
	this->request = rhs.request;
	this->oRequest = rhs.oRequest;
	this->reqGot = rhs.reqGot;
	this->response = rhs.response;
	this->resGen = rhs.resGen;
	this->resSent = rhs.resSent;
	this->CGIrun = rhs.CGIrun;
	this->cgi = rhs.cgi;
	this->config = rhs.config;

	return (*this);
}

void	Sockets::setup(int sock, int sfd, int sp, bool m, std::string host,
		const std::vector<ConfigurationObject> &config)
{
	this->socket = sock;
	this->event.data.fd = sock;
	this->main = m;
	this->server = sfd;
	this->port = sp;
	this->host = host;
	this->config = config;

	if (this->main)
	{
		this->info.sin_family = AF_INET;
		this->info.sin_addr.s_addr = inet_addr(this->host.c_str());
		this->info.sin_port = htons(port);
	}

	int f = fcntl(this->socket, F_GETFL, 0);
	if (f == -1)
		throw InternalError();
	f |= O_NONBLOCK;
	if (fcntl(this->socket, F_SETFL, f) == -1)
		throw InternalError();
}

void	Sockets::changeEvents(uint32_t ev, int epfd)
{
	this->event.events = ev;
	if (epoll_ctl(epfd, EPOLL_CTL_MOD, this->socket,
				&this->event) == -1)
		throw InternalError();
}

