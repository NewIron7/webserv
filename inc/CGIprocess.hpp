/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIprocess.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 17:44:50 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/25 08:55:40 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CGIPROCESS_HPP
# define CGIPROCESS_HPP

# include <cstring>
# include <string>
# include <map>
# include <sys/epoll.h>
# include <arpa/inet.h>
# include <cerrno>
# include <csignal>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# include "Request.hpp"
# include "InternalError.hpp"
# include "Route.hpp"
# include "ErrorCGI.hpp"
# include "ErrorFdCGI.hpp"


# define BUFFER_SIZE_CGI 4096

class CGIprocess
{
	public:
		CGIprocess(void);
		~CGIprocess(void);

		bool				done;
		bool				c;
		unsigned int		step;
		int					fds[2];
		struct sockaddr_in	info;
		socklen_t			size;
		struct epoll_event	event[2];
		int					clientFd;
		unsigned int		error;
		std::string			response;

		void	runCGI(Request &req, const Route &target);
		void	addHeaders(void);
		void	sendBody(Request &req);
		void	readResponse(Request &req);
		void	endCGI(bool err);
		bool	isError(void) const;

	private:
		std::map<std::string, std::string>	_env;
		char								**_envExec;
		char								**_args;
		std::string							_body;

		std::string							_cgiPath;
		std::string							_scriptPath;

		int									_inPipe[2];
		int									_outPipe[2];

		pid_t								_pid;
		int									_exitStatus;

		void	_setupEnv(Request &req, const Route &target);
		void	_getEnvExec(Request &req);
		void	_createArgs(Request &req);
		void	_clearAlloc(void);
		size_t	_getBodyLength(void);
};

#endif
