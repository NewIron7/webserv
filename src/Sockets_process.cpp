/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets_process.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 12:39:57 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/22 12:35:36 by hboissel         ###   ########.fr       */
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

void	Sockets::_getRealRoute(const ConfigurationObject &currentConfig,
		const std::string &targetTmp, Route &realTarget, std::size_t &sizeRoute)
{
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

	this->_getRealRoute(currentConfig, targetTmp, realTarget, sizeRoute);
	if (sizeRoute)
	{

		std::string dirListAdd = targetTmp.substr(sizeRoute);
		if (dirListAdd.size() && realTarget.directoryAdding == false && dirListAdd != "/")
		{
			req.setCodeMsg(403, "Directory adding has been disactivated in the config");
			throw Sockets::Error();
		}
		else
		{
			//std::cout << realTarget.location << std::endl;
			if (dirListAdd.size() && dirListAdd[0] != '/'
					&& realTarget.location[realTarget.location.size() - 1] != '/')
				realTarget.location += "/";
			else if (dirListAdd.size() && dirListAdd[0] == '/'
					&& realTarget.location[realTarget.location.size() - 1] == '/')
				dirListAdd.erase(dirListAdd.begin());
			realTarget.location += dirListAdd;
			//std::cout << dirListAdd << std::endl;
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
	size_t dirPosition = filename.find_last_of('/');

	if (dotPosition != std::string::npos && dotPosition > dirPosition) {
		// Extract the file extension using substr() from the dot position
		fileExtension = filename.substr(dotPosition);
	} else {
		fileExtension = "";
	}
	return (fileExtension);
}

static bool isDirectory(const std::string& filename) {
    struct stat buffer;
	if (stat(filename.c_str(), &buffer) != 0) {
		return false;
	}
	return S_ISDIR(buffer.st_mode);
}

std::string Sockets::_generateHTTPResponseHeader(const Route &target) {
	std::ostringstream response;

	// Status line: HTTP version, status code, and status message
	response << "HTTP/1.1 " << this->oRequest.getErrorCode() << " "
		<< DefaultErrorPages::statusMap[this->oRequest.getErrorCode()] << "\r\n";

	
	std::string ext = this->_getExtFile(target.location);
	if (ext.empty() && target.directoryListing
		&& isDirectory(target.location))
		ext = ".html";
	else if (ext.empty())
		ext = ".txt";
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
		if (target.root.empty() && isGet
			&& target.directoryListing == false)
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
	//get realTarget(path) and check if directoryAdding
	Route	target = this->_getRealTarget(req, currentConfig);

	this->_checkMethodAuthorized(target, "GET");
	this->_checkBodyEmpty();

	this->response.clear();
	//check if it's a directory and add root to location
	this->_getRootFileDir(target, true);

	//check if it is a CGI
	if (this->_isCGI(target))
		this->_processCGI(target);
	else
	{
		//get hold of content file
		this->response = this->_readFile(target.location, target);
		//generate proper header
		this->response = this->_generateHTTPResponseHeader(target) + this->response;
	}
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
	if (this->_isCGI(target))
		this->_processCGI(target);
	else //deal wtih the post resquest
		this->_processPOSTMethod(target);
}

static bool fileExists(const std::string& filename) {
	struct stat buffer;
	return (stat(filename.c_str(), &buffer) == 0);
}

static bool isRegularFile(const std::string& filename) {
	struct stat buffer;
	if (stat(filename.c_str(), &buffer) != 0) {
		return false; // Unable to get file status
	}
	return S_ISREG(buffer.st_mode);
}

void	Sockets::_removeFile(const std::string& filePath) {
	// Check if file exists
	if (!fileExists(filePath)) {
		this->oRequest.setCodeMsg(404, "No ressource");
		throw Sockets::Error();
	}

	if (!isRegularFile(filePath)) {
		//std::cerr << "Not a regular file: " << filename << std::endl;
		this->oRequest.setCodeMsg(403, "Not a file");
		throw Sockets::Error();
	}

	// Check permissions for the file
	if (access(filePath.c_str(), W_OK) != 0) {
		this->oRequest.setCodeMsg(403, "The program doesnt have the permission to open this file");
		throw Sockets::Error();
	}

	// Attempt to delete the file
	if (std::remove(filePath.c_str()) != 0) {
		this->oRequest.setCodeMsg(500, "Error while removing the file");
		throw Sockets::Error();
	} else {
		this->oRequest.setCodeMsg(200, "Ressource deleted successfuly");
		throw Sockets::Error();
	}
}

void	Sockets::_processDELETE(const ConfigurationObject &currentConfig)
{
	Request	&req = this->oRequest;
	Route	target = this->_getRealTarget(req, currentConfig);

	this->_checkMethodAuthorized(target, "DELETE");
	this->_checkBodyEmpty();

	this->response.clear();

	//check if it's a directory and add root to location
	this->_getRootFileDir(target, false);

	//check if it is a CGI
	if (this->_isCGI(target))
		this->_processCGI(target);
	else //process delete method
		this->_removeFile(target.location);
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

bool	Sockets::_isCGI(const Route &target)
{
	if (target.cgiPath.empty() == false
			&& target.cgiExtension.empty() == false)
	{
		std::string extTmp = this->_getExtFile(target.location);
		if (extTmp == target.cgiExtension)
			return (true);
		else
			return (false);
	}
	else
		return (false);
}

void	Sockets::_processCGI(const Route &target)
{
	this->cgi.runCGI(this->oRequest, target);
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
				this->oRequest.getErrorCode(), this->oRequest.getErrorMsg());
		return ;
	}
	this->oRequest.setErrorCode(200);

	const ConfigurationObject &currentConfig = this->_getCurrentConfig();
	try
	{
		this->_processMethod(currentConfig);
	}
	catch(const ErrorCGI& e)
	{
		throw;
	}
	catch(const std::bad_alloc& e)
	{
		std::cerr << "\033[41m" << e.what() << "\033[0m" << std::endl;;
		this->response = DefaultErrorPages::generate(
					500, "Memory error", currentConfig);
	}
	catch(const ErrorFdCGI& e)
	{
		std::cerr << "\033[41m" << e.what() << "\033[0m" << std::endl;;
		this->response = DefaultErrorPages::generate(
					500, "FD error", currentConfig);
	}
	catch(const std::exception& e)
	{
		this->response.clear();

		if (this->oRequest.getErrorCode() == 404)
		{
			this->_processRedirection(currentConfig);
		}
		if (this->response.empty())
		{
			this->response = DefaultErrorPages::generate(
					this->oRequest.getErrorCode(), this->oRequest.getErrorMsg(), currentConfig);
		}
	}
}

const char* Sockets::Error::what() const throw()
{
	return ("Error");
}
