/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 12:04:20 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/23 20:24:13 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef SOCKETS_HPP
# define SOCKETS_HPP

# include <unistd.h>
# include <iostream>
# include <cstring>
# include <sys/epoll.h>
# include <arpa/inet.h>
# include <cerrno>
# include <fcntl.h>

class Sockets
{
	public:
		Sockets(void);
		~Sockets(void);

		Sockets	&operator=(Sockets const &src);


		void	setup(int sock, int sfd, int sp, bool m = false);
		void	changeEvents(uint32_t ev, int epfd);

		bool				main;
		int					server;
		int					socket;
		struct sockaddr_in	info;
		socklen_t			size;
		unsigned int		port;
		struct epoll_event	event;
		std::string			request;
		bool				rDone;
	private:
		class InternalError: public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};
};

#endif
