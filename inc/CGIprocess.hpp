/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIprocess.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 17:44:50 by hboissel          #+#    #+#             */
/*   Updated: 2023/10/09 20:19:11 by hboissel         ###   ########.fr       */
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

	private:
		std::map<std::string, std::string>	_env;
		char								**_envExec;
		char								**_args;

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
