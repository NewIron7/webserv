/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIprocess.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 18:01:04 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/22 12:53:08 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "CGIprocess.hpp"

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
	//std::cout << "body:" << res << std::endl;
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


void CGIprocess::printAllAttributes() {
	std::cout << "Public Attributes:" << std::endl;
	std::cout << "done: " << this->done << std::endl;
	std::cout << "step: " << this->step << std::endl;
	std::cout << "clientFd: " << this->clientFd << std::endl;
	std::cout << "response: " << this->response << std::endl;
	std::cout << "fds[0]: " << this->fds[0] << std::endl;
	std::cout << "fds[1]: " << this->fds[1] << std::endl;

	//std::cout << "event[0].data.fd: " << event[0].data.fd << std::endl;
	//std::cout << "event[1].data.fd: " << event[1].data.fd << std::endl;

	std::cout << "Private Attributes:" << std::endl;
	std::cout << "_env.size(): " << this->_env.size() << std::endl;
	std::cout << "_body: " << this->_body << std::endl;
	std::cout << "_cgiPath: " << this->_cgiPath << std::endl;
}


CGIprocess::CGIprocess(void): done(false), c(true), error(0), _envExec(NULL), _args(NULL), _exitStatus(0)
{

}

CGIprocess::~CGIprocess(void)
{
	//clear all for ctrl c
	//std::cout << "cgi destructor" << std::endl;
	if (!(this->done == false && this->c == true && this->error == 0 && this->_envExec == NULL && this->_exitStatus == 0))
		this->endCGI(true);
}

static std::string getFilenameFromPath(const std::string& filePath) {
    size_t found = filePath.find_last_of("/\\");
    if (found != std::string::npos) {
        return filePath.substr(found + 1);
    }
    // If no directory separator is found, return the entire path as the filename
	return filePath;
}

void CGIprocess::_setupEnv(Request &req, const Route &target)
{
	//std::cout << "[CGI] Setup env" << std::endl;

	//get scriptPath and cgi path
	//this->_scriptPath = "/home/hboissel/webserv/cgi-bin/testGET.php";
	//this->_scriptPath = "/home/hboissel/webserv/cgi-bin/testPOST.php";
	//this->_cgiPath = "/home/hboissel/webserv/cgi-bin/php-cgi";
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

	this->_body = req.getBody();

	this->_getEnvExec(req);
}

// static void printCharDoublePointer(char** strings) {
//     for (int i = 0; strings[i] != NULL; ++i) {
//         std::cerr << strings[i] << std::endl;
//     }
// 	std::cerr << std::endl;
// }

void	CGIprocess::_getEnvExec(Request &req)
{
	//std::cout << "[CGI] Setup exec env" << std::endl;

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
		//throw std::bad_alloc();
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
	//std::cout << "[CGI] Creation args" << std::endl;

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
		req.setCodeMsg(500, "Error alloc");
		throw;

	}
}

void	CGIprocess::_clearAlloc(void)
{
	//std::cout << "[CGI] clear alloc" << std::endl;
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
	//std::cout << "[CGI] end cgi" << std::endl;

	//this->printAllAttributes();
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

std::string getFileDirectory(const std::string& filePath) {
	size_t found = filePath.find_last_of("/\\");
	if (found != std::string::npos) {
		return filePath.substr(0, found);
	}
	return "."; // Or handle error as needed
}

void	CGIprocess::runCGI(Request &req, const Route &target)
{
	//std::cout << "[CGI] Run cgi" << std::endl;

	this->c = false;

	this->_setupEnv(req, target);
	this->_createArgs(req);

	if (pipe(this->_inPipe) < -1)
	{
		this->_clearAlloc();
		req.setCodeMsg(500, "Error while creating pipe for cgi");
		throw InternalError();
	}
	if (pipe(this->_outPipe) < -1)
	{
		this->_clearAlloc();
		close(this->_inPipe[0]);
		close(this->_inPipe[1]);
		req.setCodeMsg(500, "Error while creating pipe for cgi");
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
				this->_clearAlloc();
				exit(1);
			}
			this->_exitStatus = execve(this->_args[0], this->_args, this->_envExec);
			exit(this->_exitStatus);
		}
		else
		{
			// Error changing directory
			this->_clearAlloc();
			exit(1);
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
	if (this->_exitStatus || this->step == 0 || this->response.empty())
		return (true);
	return (false);
}

void	CGIprocess::sendBody(Request &req)
{
	//std::cout << "Sending body to cgi =>" << std::endl;
	//std::cout << "\033[2m" << this->_body << "\033[0m" << std::endl;
	int	err = write(this->fds[0], this->_body.c_str(), this->_body.size());
	if (err == -1)
	{
		//std::cout << "Error while sending body to cgi" << std::endl;
		req.setCodeMsg(500, "Error while sending body from cgi");
		this->endCGI(true);
		//throw InternalError();
	}
	else if (err < static_cast<int>(this->_body.size()))
	{
		//std::cout << "Not the whole body sent" << std::endl;
	}
	else
	{
		//std::cout << "Body sent to cgi" << std::endl;
		this->step = 1;
	}
}

void	CGIprocess::readResponse(Request &req)
{
	char	buf[BUFFER_SIZE_CGI + 1];
	memset((void*)buf, 0, BUFFER_SIZE_CGI + 1);

	int bytesRead = read(this->fds[1], buf, BUFFER_SIZE_CGI);
	if (bytesRead == -1)
	{
		std::cout << "Error while reading response" << std::endl;
		req.setCodeMsg(500, "Error while reading response from cgi");
		this->endCGI(true);
	}
	else
	{
		buf[bytesRead] = '\0';
		this->response += buf;
		//std::cout << bytesRead << " bytes read on cgi" << std::endl;
	}
}
