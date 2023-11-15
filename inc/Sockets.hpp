/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 12:04:20 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/15 10:32:59 by hboissel         ###   ########.fr       */
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
# include "ConfigurationObject.hpp"

class Sockets
{
	public:
		Sockets(void);
		~Sockets(void);

		Sockets	&operator=(Sockets const &src);


		void	setup(int sock, int sfd, int sp, bool m, std::string host,
				const std::vector<ConfigurationObject> &config);
		void	changeEvents(uint32_t ev, int epfd);

		void	printAttributes(void);

		void				process(void);

		bool				main;
		int					server;
		int					socket;
		struct sockaddr_in	info;
		socklen_t			size;
		std::string			host;
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
		std::vector<ConfigurationObject>	config;
	private:

		void	_processMethod(const ConfigurationObject &currentConfig);
		void	_processGET(const ConfigurationObject &currentConfig);
		void	_processPOST(const ConfigurationObject &currentConfig);
		void	_processDELETE(const ConfigurationObject &currentConfig);
		void	_checkBodyEmpty(void);
		void	_checkBodySize(const ConfigurationObject &currentConfig);

		bool	_isCGI(void);
		void	_processCGI(void);

		const ConfigurationObject	&_getCurrentConfig(void);
		void						_checkMethodAuthorized(const Route &target,
				const std::string m);
		Route					_getRealTarget(Request &req,
				const ConfigurationObject &currentConfig);

		class Error : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
};

#endif
