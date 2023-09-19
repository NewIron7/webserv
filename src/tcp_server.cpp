/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp_server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 12:46:25 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/19 12:59:27 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "webserv.hpp"
#include <iostream>
#include <netinet/in.h>

#define BUFFER_SIZE 64

static int server;

int	ft_err(int n)
{
	if (n == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		return (-1);
	}
	return (0);
}

void init_sockaddr(int port, struct sockaddr_in &address)
{
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);
}

static void	sig_usr(int sign, siginfo_t *siginfo, void *ucontext)
{
	(void)sign;
	(void)siginfo;
	(void)ucontext;
	close(server);
}

void init_signal(void)
{
	struct sigaction	signal_action;
	signal_action.sa_sigaction = sig_usr;
	sigemptyset(&signal_action.sa_mask);
	signal_action.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &signal_action, NULL);
}

int	tcp_create(int port)
{
	server = socket(AF_INET, SOCK_STREAM, 0);
	if (ft_err(server))
		return (-1);

	struct sockaddr_in address;
	socklen_t addr_size = sizeof(address);
	bzero((void*)&address, addr_size);

	init_sockaddr(port, address);
	if (ft_err(bind(server, (struct sockaddr *)&address, addr_size)))
		return (-1);
	if (ft_err(listen(server, 3)))
		return (-1);
	std::cout << "@Listenning on port: " << port << std::endl;
	return (0);
}

int tcp_connection(void)
{
	int	new_connection;
	int	bytesRead = BUFFER_SIZE;
	char buf[BUFFER_SIZE];
	struct sockaddr_in address_connect;
	socklen_t addr_connect_size = sizeof(address_connect);
	while (1)
	{
		bzero((void *)&address_connect, addr_connect_size);
		new_connection = accept(server, (struct sockaddr *)&address_connect, &addr_connect_size);
		if (ft_err(new_connection))
			break ;

		std::cout << "@Client has been connected" << std::endl;
		int t0 = ntohl(address_connect.sin_addr.s_addr) & 0x000000FF;
		int t1 = ntohl(address_connect.sin_addr.s_addr) & 0x0000FF00;
		int t2 = ntohl(address_connect.sin_addr.s_addr) & 0x00FF0000;
		int t3 = ntohl(address_connect.sin_addr.s_addr) & 0xFF000000;
		std::cout << t3 << "." << t2 << "." << t1 << "." << t0 << std::endl;
		while (bytesRead == BUFFER_SIZE)
		{
			bytesRead = read(new_connection, buf, BUFFER_SIZE);
			write(1, buf, bytesRead);
		}

		if (ft_err(bytesRead))
			break ;
		bytesRead = BUFFER_SIZE;
		std::cout << "@Disconnection" << std::endl;
	}
	return (-1);
}

int	tcp_server(int port)
{
	init_signal();

	if (tcp_create(port))
		return (-1);

	if (tcp_connection())
		return (-1);

	close(server);
	return (0);
}
