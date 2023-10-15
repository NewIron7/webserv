/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 12:04:20 by hboissel          #+#    #+#             */
/*   Updated: 2023/10/13 07:37:08 by hboissel         ###   ########.fr       */
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

# include "Request.hpp"
# include "InternalError.hpp"
# include "DefaultErrorPages.hpp"
# include "CGIprocess.hpp"

class Sockets
{
	public:
		Sockets(void);
		~Sockets(void);

		Sockets	&operator=(Sockets const &src);


		void	setup(int sock, int sfd, int sp, bool m = false);
		void	changeEvents(uint32_t ev, int epfd);

		void	printAttributes(void);

		void				process(void);

		bool				main;
		int					server;
		int					socket;
		struct sockaddr_in	info;
		socklen_t			size;
		unsigned int		port;
		struct epoll_event	event;
		std::string			request;
		Request				oRequest;
		bool				reqGot;
		std::string			response;
		bool				resGen;
		bool				resSent;

		bool				CGIrun;
		CGIprocess			cgi;
	private:
		void	_processMethod(void);
		void	_processGET(void);
		void	_processPOST(void);
		void	_processDELETE(void);
		void	_checkBodyEmpty(void);

		bool	_isCGI(void);
		void	_processCGI(void);

		class Error : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
};

#endif
