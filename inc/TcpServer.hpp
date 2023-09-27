/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 14:27:01 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/27 19:42:57 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef TCPSERVER_HPP
# define TCPSERVER_HPP

# define MAXIREQ 64
# define TEVENT 30000
# define MAXEVENT 16
# define BUFFER_SIZE 64

# include <iostream>
# include <map>
# include <cstring>
# include <cerrno>
# include <algorithm>
# include <iomanip>

# include <unistd.h>
# include <arpa/inet.h>
# include <sys/epoll.h>

# include "Sockets.hpp"

class TcpServer
{
	public:
		TcpServer(void);
		~TcpServer(void);

		void	run(void);
		void	create(unsigned int port);
	private:
		void	_add_client(const int &fdServer);
		void	_remove_client(Sockets &client);
		
		void	_processEvent(struct epoll_event &ev);
		void	_processEPOLLIN(struct epoll_event &ev);		
		void	_processEPOLLOUT(struct epoll_event &ev);		
		void	_processEPOLLERR(struct epoll_event &ev);
		void	_processEPOLLHUP(struct epoll_event &ev);

		int									_epfd;
		std::map<int, Sockets>				_streams;

		class InternalError: public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};
};

#endif
