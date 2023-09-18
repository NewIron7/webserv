/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 12:46:25 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/18 19:55:31 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "webserv.hpp"
#include <iostream>
#include <netinet/in.h>

int server;
int	new_connection;

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
	std::cout << "SERVER FD: " << server << std::endl;
	close(new_connection);
	close(server);
}

int	do_socket(void)
{
	server = socket(AF_INET, SOCK_STREAM, 0);
	if (ft_err(server))
		return (-1);
	std::cout << "SERVER FD: " << server << std::endl;

	struct sigaction	signal_action;
	signal_action.sa_sigaction = sig_usr;
	sigemptyset(&signal_action.sa_mask);
	signal_action.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &signal_action, NULL);

	struct sockaddr_in address;
	init_sockaddr(4245, address);
	if (ft_err(bind(server, (struct sockaddr *)&address, sizeof(address))))
		return (-1);

	//3 is the number of allowed connection in the queue
	if (ft_err(listen(server, 3)))
		return (-1);

	int	bytesRead = 1;
	char buf[64];
	struct sockaddr_in address_connect;
	socklen_t addr_connect_size = sizeof(address_connect);
	int	new_connection = accept(server, (struct sockaddr *)&address_connect, &addr_connect_size);
	while (new_connection != -1)
	{
		std::cout << "Client has been connected" << std::endl;
		while (bytesRead > 0)
		{
			bytesRead = read(new_connection, buf, 64);
			write(1, buf, bytesRead);
		}
		std::cout << "****END****" << std::endl;
		close(new_connection);
		new_connection = accept(server, (struct sockaddr *)&address_connect, &addr_connect_size);
	}
	close(server);
	if (ft_err(new_connection))
		return (-1);
	return (0);
}
