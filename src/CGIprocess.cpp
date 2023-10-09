/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIprocess.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 18:01:04 by hboissel          #+#    #+#             */
/*   Updated: 2023/10/09 20:23:14 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "CGIprocess.hpp"

CGIprocess::CGIprocess(const Request &req): _envExec(NULL), _exitStatus(0)
{
	this->_setupEnv(req);
}

CGIprocess::~CGIprocess(void)
{
}

void CGIprocess::_setupEnv(const Request &req)
{
	this->_env["REQUEST_METHOD"] = req.getMethod();
	this->_env["QUERY_STRING"] = req.getQuery();
	this->_env["CONTENT_TYPE"] = req.getHeaders()["CONTENT-TYPE"];
	this->_env["CONTENT_LENGTH"] = req.getHeaders()["CONTENT-LENGTH"];
	this->_env["HTTP_USER_AGENT"] = req.getHeaders()["USER-AGENT"];
	this->_env["HTTP_REFERER"] = req.getHeaders()["REFERER"];
	this->_env["HTTP_ACCEPT_LANGUAGE"] = req.getHeaders()["ACCEPT-LANGUAGE"];
	this->_env["HTTP_ACCEPT_ENCODING"] = req.getHeaders()["ACCEPT-ENCODING"];
	this->_env["HTTP_CONNECTION"] = req.getHeaders()["CONNECTION"];
	this->_env["SERVER_NAME"] = "";
	this->_env["SERVER_PORT"] = "";
	this->_env["REMOTE_ADDR"] = "";
	this->_env["REMOTE_PORT"] = "";
	this->_env["SERVER_PROTOCOL"] = req.getVersion();
	this->_env["SCRIPT_NAME"] = "";
	this->_env["SCRIPT_FILENAME"] = this->_scriptPath;
	this->_env["DOCUMENT_ROOT"] = "";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["SERVER_SOFTWARE"] = "webserv";
	this->_env["REQUEST_URI"] = req.getTarget();
	this->_env["PATH_INFO"] = "";
	this->_env["AUTH_TYPE"] = "";
	this->_env["REMOTE_USER"] = "";
	this->_env["REMOTE_IDENT"] = "";
	this->_env["HTTP_HOST"] = req.getHeaders()["HOST"];
	this->_env["HTTP_COOKIE"] = req.getHeaders()["COOKIE"];
	this->_env["HTTP_ACCEPT"] = req.getHeaders()["ACCEPT"];
	
	this->_getEnvExec();
}

void	CGIprocess::_getEnvExec(void)
{
	int	j = 0;

	try
	{
		this->_envExec = new char*[this->_env.size() + 1];

		for (std::map<std::string, std::string>::const_iterator it = this->_env.begin();
				it != this->_env.end(); it++)
		{
			std::string	element = it->first + "=" + it->second;
			this->_envExec[j] = new char[element.size() + 1];
			this->_envExec[j] = strcpy(this->_envExec[j], (const char*)element.c_str());
			j++;
		}
	}
	catch (std::bad_alloc &e)
	{
		for (size_t i = 0; i < j; i++)
			delete[] this->_envExec[i];
		delete[] this->_envExec;
		throw;
	}
	this->envExec[j] = NULL;
}

void	CGIprocess::_createArgs(void)
{
	int	i = -1;
	try
	{
		this->_args = new char*[3];
		i++;

		this->_args[0] = new char[this->_cgiPath.size() + 1];
		this->_args[0] = strcpy(this->_args[0], (const char*)this->_cgiPath.c_str());
		i++;

		this->_args[1] = new char[this->_scriptPath.size() + 1];
		this->_args[1] = strcpy(this->_args[1], (const char*)this->_scriptPath.c_str());
		this->_args[2] = NULL;
	}
	catch (std::bad_alloc &e)
	{
		if (i == 1)
			delete[] this->_args[0];
		if (i >= 0)
			delete[] this->_args;
		throw;
			
	}
}

void	CGIprocess::_clearAlloc(void)
{
		delete[] this->_args[0];
		delete[] this->_args[1];
		delete[] this->_args;

		int	end = this->_env.size();
		for (size_t i = 0; i <= end; i++)
			delete[] this->_envExec[i];
		delete[] this->_envExec;
}

void	CGIprocess::_runCGI(void)
{
	if (pipe(this->_inPipe) < -1)
	{
		this->_clearAlloc();
		throw InternalError();
	}
	if (pipe(this->_outPipe) < -1)
	{
		this->_clearAlloc();
		close(this->_inPipe[0]);
		close(this->_inPipe[1]);
		throw InternalError();
	}
	this->_pid = fork();
	if (this->_pid == -1)
	{
		this->_clearAlloc();
		throw InternalError();
	}
	else if (this->_pid == 0)
	{
		dup2(this->_inPipe[0], STDIN_FILENO);
		dup2(this->_outPipe[1], STDOUT_FILENO);
		close(this->_inPipe[0]);
		close(this->_inPipe[1]);
		close(this->_outPipe[0]);
		close(this->_outPipe[1]);
		this->_exitStatus = execve(this->_args[0], this->_args, this->_envExec);
		exit(this->_exitStatus);
	}
	else
	{
		this->_clearAlloc();
	}
	
}
