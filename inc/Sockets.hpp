/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 12:04:20 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/23 14:03:36 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef SOCKETS_HPP
# define SOCKETS_HPP

# include <unistd.h>
# include <iostream>
# include <cstring>
# include <sys/epoll.h>
# include <arpa/inet.h>

class Sockets
{
	public:
		Sockets(void);
		~Sockets(void);

		Sockets	&operator=(Sockets const &src);


		void	setup(int sock, int sfd, int sp, bool m = false);

		bool				main;
		int					server;
		int					socket;
		struct sockaddr_in	info;
		socklen_t			size;
		unsigned int		port;
		struct epoll_event	event;
};

#endif
