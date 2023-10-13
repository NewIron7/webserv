/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIprocess.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 18:01:04 by hboissel          #+#    #+#             */
/*   Updated: 2023/10/13 10:35:13 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "CGIprocess.hpp"

CGIprocess::CGIprocess(void): _envExec(NULL), _exitStatus(0)
{

}

CGIprocess::~CGIprocess(void)
{
}

void CGIprocess::_setupEnv(Request &req)
{
	//get scriptPath
	this->_scriptPath = "/home/ubuntu/webserv/cgi-bin/testGET.php";

	this->_env["REDIRECT_STATUS"] = "200";
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
	size_t	j = 0;

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
	this->_envExec[j] = NULL;
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

		size_t	end = this->_env.size();
		for (size_t i = 0; i <= end; i++)
			delete[] this->_envExec[i];
		delete[] this->_envExec;
}

void	CGIprocess::endCGI(bool err)
{
	if (err)
		kill(this->_pid, 9);
	else
	{
		int status;

		int w = waitpid(this->_pid, &status, WNOHANG);
		if (w == -1)
		{
			if (errno != ECHILD)
				kill(this->_pid, 9);
		}
		else if (w == 0)
		{
			kill(this->_pid, 9);
		}
	}

	close(this->fds[0]);
	close(this->fds[1]);

	this->_clearAlloc();
	this->_env.clear();
	this->_body.clear();
	this->response.clear();
}

void	CGIprocess::runCGI(Request &req)
{
	this->_setupEnv(req);
	this->_createArgs();
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
		close(this->_inPipe[0]);
		close(this->_inPipe[1]);
		close(this->_outPipe[0]);
		close(this->_outPipe[1]);
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
		close(this->_inPipe[0]);
		close(this->_outPipe[1]);
		this->fds[0] = this->_inPipe[1];
		this->fds[1] = this->_outPipe[0];
		if (this->_body.empty())
			this->step = 1;
		else
			this->step = 0;
	}
}

void	CGIprocess::sendBody(void)
{
	int	err = write(this->fds[0], this->_body.c_str(), this->_body.size());
	if (err == -1)
	{
		std::cout << "Error while sending body to cgi" << std::endl;
		this->endCGI(true);
		throw InternalError();
	}
	else if (err < static_cast<int>(this->_body.size()))
	{
		std::cout << "Not the whole body sent" << std::endl;
	}
	else
	{
		std::cout << "Body sent to cgi" << std::endl;
		this->step = 1;
	}
}

void	CGIprocess::readResponse(void)
{
	char	buf[BUFFER_SIZE + 1];
	memset((void*)buf, 0, BUFFER_SIZE + 1);

	int bytesRead = read(this->fds[1], buf, BUFFER_SIZE);
	if (bytesRead == -1)
	{
		std::cout << "Error while reading response" << std::endl;
		this->endCGI(true);
		throw InternalError();
	}
	else
	{
		buf[bytesRead] = '\0';
		this->response += buf;
		std::cout << bytesRead << " bytes read on cgi" << std::endl;

	}
}
