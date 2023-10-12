/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIprocess.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 17:44:50 by hboissel          #+#    #+#             */
/*   Updated: 2023/10/12 09:04:54 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CGIPROCESS_HPP
# define CGIPROCESS_HPP

# include <cstring>

# include <string>
# include <map>

# include "Request.hpp"
# include "InternalError.hpp"

class CGIprocess
{
	public:
		CGIprocess(const Request &req);
		~CGIprocess(void);

		unsigned int		step;
		int					fds[2];
		struct sockaddr_in	info;
		socklen_t			size;
		struct epoll_event	event;
		
		int					clientFd;
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

		void	_runCGI(void);
		void	_setupEnv(const Request &req);
		void	_getEnvExec(void);
		void	_createArgs(void);

		void	_clearAlloc(void);
};

#endif
