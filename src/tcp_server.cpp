/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp_server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 12:46:25 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/21 17:40:01 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "webserv.hpp"

#define BUFFER_SIZE 64

static std::vector<int> all_server;

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

void	close_all_server(void)
{
	std::for_each(all_server.begin(), all_server.end(), close);
}

static void	sig_usr(int sign, siginfo_t *siginfo, void *ucontext)
{
	(void)sign;
	(void)siginfo;
	(void)ucontext;
	close_all_server();
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
	all_server.push_back(socket(AF_INET, SOCK_STREAM, 0));
	if (ft_err(all_server.back()))
		return (-1);

	struct sockaddr_in address;
	socklen_t addr_size = sizeof(address);
	bzero((void*)&address, addr_size);

	init_sockaddr(port, address);
	if (ft_err(bind(all_server.back(), (struct sockaddr *)&address, addr_size)))
		return (-1);

	if (ft_err(listen(all_server.back(), 3)))
		return (-1);
	std::cout << "@Listenning on port: " << port << std::endl;
	return (0);
}

int tcp_connection(int server)
{
	int	new_connection;
	int	bytesRead = BUFFER_SIZE;
	char buf[BUFFER_SIZE + 1];
	struct sockaddr_in address_connect;
	socklen_t addr_connect_size = sizeof(address_connect);
	while (1)
	{
		bzero((void *)&address_connect, addr_connect_size);
		new_connection = accept(server, (struct sockaddr *)&address_connect,
				&addr_connect_size);
		if (ft_err(new_connection))
			break ;

		std::cout << "@Client has been connected " << new_connection << std::endl;
		std::string request;
		while (bytesRead == BUFFER_SIZE)
		{
			bytesRead = read(new_connection, buf, BUFFER_SIZE);
			buf[bytesRead] = '\0';
			request += buf;
		}
		std::cout << "@Request=>" << std::endl << request << std::endl;
		//Parsing of Request to generate the right response
		std::string response("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");
		
		write(new_connection, response.c_str(), response.size());

		if (ft_err(bytesRead))
			break ;
		bytesRead = BUFFER_SIZE;
		std::cout << "@Disconnection" << std::endl;
		close(new_connection);
	}
	return (-1);
}

int	tcp_server(int port)
{
	init_signal();

	if (tcp_create(port))
		return (-1);

	if (tcp_connection(all_server.back()))
		return (-1);

	close_all_server();
	return (0);
}
