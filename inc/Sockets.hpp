/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 12:04:20 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/25 08:57:49 by hboissel         ###   ########.fr       */
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
# include <dirent.h>

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
		Sockets(const Sockets &src);

		Sockets	&operator=(Sockets const &src);


		void	setup(int sock, int sfd, int sp, bool m, std::string host,
				const std::vector<ConfigurationObject> &config);
		void	changeEvents(uint32_t ev, int epfd);

		void	printAttributes(void);

		void	process(void);

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

		bool	_isCGI(const Route &target);
		void	_processCGI(const Route &target);

		const ConfigurationObject	&_getCurrentConfig(void);
		
		void		_checkMethodAuthorized(const Route &target,
				const std::string m);
		Route		_getRealTarget(Request &req,
				const ConfigurationObject &currentConfig);
		std::string	_readFile(const std::string& filename, const Route &target);

		std::string _generateHTTPResponseHeader(const Route &target);
		std::string _generateHTTPResponseHeaderPOST(void);

		std::string _getExtFile(const std::string &filename);

		void		_getRootFileDir(Route &target, bool isGet);

		bool		_checkWritePermission(const std::string &file);
		void		_processUpload(const Route &target);

		void		_createFile(const std::string& filePath);
		void		_appendToFile(const std::string& filename,
			const std::string& content);

		void		_parseMultipartFormData(const std::string& body,
			const std::string &boundary, const Route& target);
		std::string _getBoundaryFromContentType(const std::string& contentType);
		void		_parseContentMultipartFormData(const std::string& content,
				const Route &target);
		std::string	_getFilenameMultipartFormData(const std::string& headers);

		void		_canCreateFileInDirectory(const std::string& path);
		void		_removeFile(const std::string& filePath);

		void		_processRedirection(const ConfigurationObject
				&currentConfig);

		void		_getRealRoute(const ConfigurationObject &currentConfig, 
				const std::string &targetTmp, Route &realTarget,
				std::size_t &sizeRoute);
		std::string	_processDirListing(const std::string &path);

		class Error : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
};

#endif
