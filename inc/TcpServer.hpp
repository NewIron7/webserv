/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 14:27:01 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/20 15:45:33 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef TCPSERVER_HPP
# define TCPSERVER_HPP

# define MAXIREQ 64

# include <iostream>
# include <vector>
# include <cstring>
# include <cerrno>

# include <arpa/inet.h>

typedef struct s_connect
{
	int					socket;
	struct sockaddr_in	info;
	socklen_t			size;
}	t_connect;

class TcpServer
{
	public:
		TcpServer(unsigned int port);
		~TcpServer(void);

		void	add_client(void);
	private:
		void	_create(unsigned int port);


		t_connect				_server;
		std::vector<t_connect>	_clients;
		unsigned int			_port;

		class InternalError: public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};
};

#endif
