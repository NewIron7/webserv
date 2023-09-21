/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 14:27:01 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/21 17:21:31 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef TCPSERVER_HPP
# define TCPSERVER_HPP

# define MAXIREQ 64

# include <iostream>
# include <vector>
# include <cstring>
# include <cerrno>
# include <algorithm>

# include <unistd.h>
# include <arpa/inet.h>
# include <sys/epoll.h>

typedef struct s_connect t_connect;

struct s_connect
{
	bool				main;
	struct s_connect	&server;
	int					socket;
	struct sockaddr_in	info;
	socklen_t			size;
	unsigned int		port;
};

class TcpServer
{
	public:
		TcpServer(void);
		~TcpServer(void);

		void	run(void);
		void	create(unsigned int port);
	private:
		void	_add_client(const t_connect & server);

		int								_epfd;
		std::vector<t_connect>			_streams;
		std::vector<struct epoll_event>	_events;

		class InternalError: public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};
};

#endif
