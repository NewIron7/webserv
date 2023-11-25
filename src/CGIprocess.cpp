/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIprocess.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 18:01:04 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/25 09:04:08 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "CGIprocess.hpp"

CGIprocess::CGIprocess(void): done(false), c(true), step(0), error(0),
	_envExec(NULL), _args(NULL), _pid(0), _exitStatus(0)
{
	this->fds[1] = -1;
	this->fds[0] = -1;
}

CGIprocess::~CGIprocess(void)
{
	if (!(this->done == false && this->c == true
				&& this->error == 0 && this->_envExec == NULL
				&& this->_exitStatus == 0))
		this->endCGI(true);
}

size_t	CGIprocess::_getBodyLength(void)
{
	std::string	res = this->response;

	std::string	tmp = res.substr(0, res.find("\r\n"));
	while (tmp.size())
	{
		if (tmp.compare(res) == 0)
		{
			return (res.size());
		}
		res.erase(0, tmp.size() + 2);
		tmp = res.substr(0, res.find("\r\n"));
	}
	return (res.size() - 2);
}

void	CGIprocess::addHeaders(void)
{
	if (this->response.empty())
		return ;
	size_t	l = this->_getBodyLength();
	if (l == this->response.size() || l == (this->response.size() - 2))
	{
		std::string ct = "Content-Type: text/plain\r\n";
		if (l == this->response.size())
			ct += "\r\n";		
		this->response.insert(0, ct);
	}
	std::string	cl = "Content-Length: " + SSTR(l);
	cl += "\r\n";
	this->response.insert(0, cl);
	if (this->response.find("HTTP/1.1") == std::string::npos)
		this->response.insert(0, "HTTP/1.1 200 OK\r\n");
}

static std::string getFilenameFromPath(const std::string& filePath) {
    size_t found = filePath.find_last_of("/\\");
    if (found != std::string::npos) {
        return filePath.substr(found + 1);
    }
    // If no directory separator is found,
	// return the entire path as the filename
	return filePath;
}

void CGIprocess::_setupEnv(Request &req, const Route &target)
{
	this->_scriptPath = getFilenameFromPath(target.location);
	this->_cgiPath = target.cgiPath;

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
	this->_env["SERVER_PROTOCOL"] = req.getVersion();
	this->_env["SCRIPT_FILENAME"] = this->_scriptPath;
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["SERVER_SOFTWARE"] = "webserv";
	this->_env["REQUEST_URI"] = req.getTarget();
	this->_env["HTTP_HOST"] = req.getHeaders()["HOST"];
	this->_env["HTTP_COOKIE"] = req.getHeaders()["COOKIE"];
	this->_env["HTTP_ACCEPT"] = req.getHeaders()["ACCEPT"];

	this->_body = req.getBody();

	this->_getEnvExec(req);
}

void	CGIprocess::_getEnvExec(Request &req)
{
	size_t	j = 0;

	try
	{
		this->_envExec = new char*[this->_env.size() + 1];

		for (std::map<std::string, std::string>::const_iterator
				it = this->_env.begin(); it != this->_env.end(); it++)
		{
			std::string	element = it->first + "=" + it->second;
			this->_envExec[j] = new char[element.size() + 1];
			this->_envExec[j] = strcpy(this->_envExec[j],
									(const char*)element.c_str());
			j++;
		}
	}
	catch (std::bad_alloc &e)
	{
		for (size_t i = 0; i < j; i++)
			delete[] this->_envExec[i];
		delete[] this->_envExec;
		req.setCodeMsg(500, "Error alloc");
		throw;
	}
	this->_envExec[j] = NULL;
}

void	CGIprocess::_createArgs(Request &req)
{
	int	i = -1;
	try
	{
		this->_args = new char*[3];
		i++;

		this->_args[0] = new char[this->_cgiPath.size() + 1];
		this->_args[0] = strcpy(this->_args[0],
							(const char*)this->_cgiPath.c_str());
		i++;

		this->_args[1] = new char[this->_scriptPath.size() + 1];
		this->_args[1] = strcpy(this->_args[1],
							(const char*)this->_scriptPath.c_str());
		this->_args[2] = NULL;
	}
	catch (std::bad_alloc &e)
	{
		if (i == 1)
			delete[] this->_args[0];
		if (i >= 0)
			delete[] this->_args;
		req.setCodeMsg(500, "Error alloc");
		throw;

	}
}

void	CGIprocess::_clearAlloc(void)
{
	if (this->_args)
	{
		delete[] this->_args[0];
		delete[] this->_args[1];
		delete[] this->_args;
		this->_args = NULL;
	}
	
	if (this->_envExec)
	{
		size_t	end = this->_env.size();
		for (size_t i = 0; i <= end; i++)
			delete[] this->_envExec[i];
		delete[] this->_envExec;
		this->_envExec = NULL;
	}
	
}

void	CGIprocess::endCGI(bool err)
{
	if (this->_pid)
	{
		if (err)
		{
			kill(this->_pid, 9);
			this->_exitStatus = 1;
		}
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
		this->c = true;
	}
	
	

	this->_inPipe[0] = -1;
	this->_inPipe[1] = -1;
	this->_outPipe[0] = -1;
	this->_outPipe[1] = -1;

	this->_clearAlloc();
	this->_env.clear();
	this->_body.clear();
	this->response.clear();

	this->done = false;
}

static std::string getFileDirectory(const std::string& filePath) {
	size_t found = filePath.find_last_of("/\\");
	if (found != std::string::npos) {
		return filePath.substr(0, found);
	}
	return ".";
}

void	CGIprocess::runCGI(Request &req, const Route &target)
{
	this->c = false;
	this->_exitStatus = 0;

	this->_setupEnv(req, target);
	this->_createArgs(req);

	if (pipe(this->_inPipe) < -1)
	{
		this->_clearAlloc();
		req.setCodeMsg(500, "Error while creating pipe for cgi");
		throw ErrorFdCGI();
	}
	if (pipe(this->_outPipe) < -1)
	{
		this->_clearAlloc();
		close(this->_inPipe[0]);
		close(this->_inPipe[1]);
		req.setCodeMsg(500, "Error while creating pipe for cgi");
		throw ErrorFdCGI();
	}
	this->_pid = fork();
	if (this->_pid == -1)
	{
		std::cerr << "ERROR on fork" << std::endl;
		close(this->_inPipe[0]);
		close(this->_inPipe[1]);
		close(this->_outPipe[0]);
		close(this->_outPipe[1]);
		this->_clearAlloc();
		req.setCodeMsg(500, "Error while launching cgi: on fork");
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
		std::string targetDir = getFileDirectory(target.location);
		if (chdir(targetDir.c_str()) == 0)
		{
			// Directory change successful
			if (signal(SIGINT, SIG_DFL) == SIG_ERR)
			{
				throw ErrorCGI();
			}
			this->_exitStatus = execve(this->_args[0], this->_args,
									this->_envExec);
			throw ErrorCGI();
		}
		else
		{
			// Error changing directory
			throw ErrorCGI();
		}
	}
	else
	{
		close(this->_inPipe[0]);
		close(this->_outPipe[1]);
		this->fds[0] = this->_inPipe[1];
		this->fds[1] = this->_outPipe[0];
		this->c = false;
		if (this->_body.empty())
			this->step = 1;
		else
			this->step = 0;
	}
}

bool	CGIprocess::isError(void) const
{
	if (this->_exitStatus
		|| this->step == 0
		|| this->response.empty())
		return (true);
	return (false);
}

void	CGIprocess::sendBody(Request &req)
{
	int	err = write(this->fds[0], this->_body.c_str(), this->_body.size());
	if (err == -1 || (err == 0 && this->_body.empty() == false))
	{
		req.setCodeMsg(500, "Error while sending body from cgi");
		this->endCGI(true);
	}
	else
		this->step = 1;
}

void	CGIprocess::readResponse(Request &req)
{
	char	buf[BUFFER_SIZE_CGI + 1];
	memset((void*)buf, 0, BUFFER_SIZE_CGI + 1);

	int bytesRead = read(this->fds[1], buf, BUFFER_SIZE_CGI);
	if (bytesRead == -1 || bytesRead == 0)
	{
		req.setCodeMsg(500, "Error while reading response from cgi");
		this->endCGI(true);
	}
	else
	{
		buf[bytesRead] = '\0';
		this->response += buf;
	}
}
