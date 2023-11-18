/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets_process.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 12:39:57 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/18 04:18:00 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Sockets.hpp"

void	Sockets::_checkBodyEmpty(void)
{
	Request	&req = this->oRequest;
	if (req.getBody().empty() == false)
	{
		req.setCodeMsg(400, "Your request must have an empty body");
		throw Sockets::Error();
	}
}

void	Sockets::_checkBodySize(const ConfigurationObject &currentConfig)
{
	Request	&req = this->oRequest;

	if (currentConfig.isBodySize && currentConfig.bodySize < req.getBody().size())
	{
		req.setCodeMsg(413, "The size of your request's body is too big based on the config");
		throw Sockets::Error();
	}
}

Route	Sockets::_getRealTarget(Request &req, const ConfigurationObject &currentConfig)
{
	const std::string &targetTmp = req.getTarget();
	Route realTarget;
	std::size_t	sizeRoute = 0;

	//currentConfig.printParameters();

	realTarget.location = "." + targetTmp;
	realTarget.directoryListing = true;
	realTarget.empty = true;

	for (std::map<std::string, Route>::const_iterator it = currentConfig.routes.begin();
			it != currentConfig.routes.end(); ++it)
	{
		if (targetTmp == it->first || targetTmp == it->first + "/")
			realTarget.dir = true;
		std::size_t found = targetTmp.find(it->first);
		if (found != std::string::npos && found == 0 && it->first.size() > sizeRoute)
		{
			realTarget = it->second;
			sizeRoute = it->first.size();
		}
	}
	if (sizeRoute)
	{

		std::string dirListAdd = targetTmp.substr(sizeRoute);
		if (dirListAdd.size() && realTarget.directoryListing == false && dirListAdd != "/")
		{
			req.setCodeMsg(403, "Directory listing has been disactivated in the config");
			throw Sockets::Error();
		}
		else
		{
			if (dirListAdd.size() && dirListAdd[0] != '/')
				realTarget.location += "/";
			realTarget.location += dirListAdd;
		}
	}
	return (realTarget);
}

void	Sockets::_checkMethodAuthorized(const Route &target, const std::string m)
{
	if (std::find(target.methods.begin(), target.methods.end(), m) == target.methods.end())
	{
		if (target.empty == false)
		{
			this->oRequest.setCodeMsg(405, m + " is not allowed by the config");
			throw Sockets::Error();
		}
	}
}

std::string Sockets::_getExtFile(const std::string &filename)
{
	std::string fileExtension;

    // Get the position of the last dot in the filename
    size_t dotPosition = filename.find_last_of('.');

    if (dotPosition != std::string::npos) {
        // Extract the file extension using substr() from the dot position
        fileExtension = filename.substr(dotPosition);
    } else {
        fileExtension = ".txt";
    }
	return (fileExtension);
}

std::string Sockets::_generateHTTPResponseHeader(const Route &target) {
    std::ostringstream response;

    // Status line: HTTP version, status code, and status message
    response << "HTTP/1.1 " << this->oRequest.getErrorCode() << " "
		<< DefaultErrorPages::statusMap[this->oRequest.getErrorCode()] << "\r\n";

	std::string ext = this->_getExtFile(target.location);
    // Headers
    response << "Content-Type: " << DefaultErrorPages::getContentType(ext) << "\r\n";
    response << "Content-Length: " << this->response.size() << "\r\n";
	response << "Server: webserv/0.1\r\n";
    // Add other headers as needed

    // Blank line indicating the end of the headers
    response << "\r\n";

    return response.str();
}

void	Sockets::_getRootFileDir(Route &target, bool isGet)
{
	if (target.dir)
	{
		if (target.root.empty() && isGet)
		{
			this->oRequest.setCodeMsg(404, "No ressource");
			throw Sockets::Error();
		}
		else
		{
			if (target.location[target.location.size() - 1] != '/')
				target.location += "/";
			target.location += target.root;
		}
	}
}

void	Sockets::_processGET(const ConfigurationObject &currentConfig)
{
	Request	&req = this->oRequest;
	//get realTarget(path) and check if directoryListing
	Route	target = this->_getRealTarget(req, currentConfig);
	
	this->_checkMethodAuthorized(target, "GET");
	this->_checkBodyEmpty();

	this->response.clear();
	//check if it's a directory and add root to location
	this->_getRootFileDir(target, true);
	
	std::cout << "Current route: " << std::endl;
	target.printRoute();


	//check if it is a CGI

	//get hold of content file
	this->response = this->_readFile(target.location);

	//generate proper header
	this->response = this->_generateHTTPResponseHeader(target) + this->response;

	//this->response = DefaultErrorPages::generate( 418, "Test GET");
	(void)req;
}

void	Sockets::_processPOST(const ConfigurationObject &currentConfig)
{
	Request	&req = this->oRequest;
	Route	target = this->_getRealTarget(req, currentConfig);

	this->_checkMethodAuthorized(target, "POST");
	this->_checkBodySize(currentConfig);
	
	this->response.clear();
	//check if it's a directory and add root to location
	this->_getRootFileDir(target, false);
	
	//check if it is a CGI
	
	//deal wtih the post resquest
	this->_processPOSTMethod(target);

	//this->response = DefaultErrorPages::generate( 418, "Test POST");
	(void)req;
}

void	Sockets::_processDELETE(const ConfigurationObject &currentConfig)
{
	Request	&req = this->oRequest;
	Route	target = this->_getRealTarget(req, currentConfig);
	
	std::cout << "Current route: " << std::endl;
	target.printRoute();

	this->_checkMethodAuthorized(target, "DELETE");
	this->_checkBodyEmpty();
	
	this->response = DefaultErrorPages::generate( 418, "Test DELETE");
	(void)req;
}


void	Sockets::_processMethod(const ConfigurationObject &currentConfig)
{
	if (this->oRequest.getMethod() == "GET")
		this->_processGET(currentConfig);
	else if (this->oRequest.getMethod() == "POST")
		this->_processPOST(currentConfig);
	else
		this->_processDELETE(currentConfig);
}

bool	Sockets::_isCGI(void)
{
	return (false);
}

void	Sockets::_processCGI(void)
{
	//execute CGI
	//std::cout << "*****[CGI CALLED]******" << std::endl;
	this->cgi.runCGI(this->oRequest);
	this->CGIrun = true;
}

const ConfigurationObject &Sockets::_getCurrentConfig(void)
{
	const std::string &server_name = this->oRequest.getHost();
	for (std::vector<ConfigurationObject>::const_iterator it = this->config.begin();
			it != this->config.end(); ++it)
	{
		if (std::find(it->server_names.begin(), it->server_names.end(),
					server_name) != it->server_names.end())
			return (*it);
	}
	return (*this->config.begin());
}

void	Sockets::process(void)
{
	this->oRequest = Request(this->request);
	if (this->oRequest.getErrorCode())
	{
		this->response = DefaultErrorPages::generate(
			this->oRequest.getErrorCode(), "Error while parsing the request");
		std::cout << this->oRequest.getMethod() << std::endl;
		return ;
	}
	//this->oRequest.printAttributes();
	this->oRequest.setErrorCode(200);
	
	const ConfigurationObject &currentConfig = this->_getCurrentConfig();
	//std::cout << "***Config found: " << std::endl;
	//currentConfig.printParameters();
	try
	{
		//before check if its a CGI call
		if (this->_isCGI())
			this->_processCGI();
		else
			this->_processMethod(currentConfig);
	}
	catch(const std::exception& e)
	{
		this->response = DefaultErrorPages::generate(
			this->oRequest.getErrorCode(), this->oRequest.getErrorMsg());
	}
}

const char* Sockets::Error::what() const throw()
{
		return ("Error");
}
