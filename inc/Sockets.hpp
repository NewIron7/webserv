/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 12:04:20 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/17 12:39:53 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef SOCKETS_HPP
# define SOCKETS_HPP

# include <fstream>
# include <unistd.h>
# include <iostream>
# include <cstring>
# include <sys/epoll.h>
# include <sys/stat.h>
# include <arpa/inet.h>
# include <cerrno>
# include <fcntl.h>
# include <cstdlib>

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
		void	_processPOSTMethod(const Route &target);
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
		std::string	_readFile(const std::string& filename);

		std::string _generateHTTPResponseHeader(const Route &target);
		std::string _generateHTTPResponseHeaderPOST(void);

		std::string _getExtFile(const std::string &filename);

		void	_getRootFileDir(Route &target);

		bool	_checkWritePermission(const std::string &file);
		void	_processUpload(const Route &target);

		void	_createFile(const std::string& filePath);
		void	_appendToFile(const std::string& filename,
			const std::string& content);

		void	_parseMultipartFormData(const std::string& body,
			const std::string &boundary);
		std::string _getBoundaryFromContentType(const std::string& contentType);

		class Error : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
};

#endif
