/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:13:32 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/16 06:24:15 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Request.hpp"

Request::Request(void): _errorCode(0), _errorMsg("Error") {}

Request	&Request::operator=(const Request& rhs)
{
	if (this == &rhs)
		return *this;

	this->_method = rhs._method;
	this->_target = rhs._target;
	this->_query = rhs._query;
	this->_pVersion = rhs._pVersion;
	this->_host = rhs._host;
	this->_port = rhs._port;
	this->_headers = rhs._headers;
	this->_body = rhs._body;
	this->_errorCode = rhs._errorCode;
	this->_errorMsg = rhs._errorMsg;

	return (*this);
}
void Request::printAttributes(void) const
{
	std::cout << "Method: " << _method << std::endl;
	std::cout << "Target: " << _target << std::endl;
	std::cout << "Query: " << _query << std::endl;
	std::cout << "HTTP Version: " << _pVersion << std::endl;
	std::cout << "Host: " << _host << std::endl;
	std::cout << "Port: " << _port << std::endl;

	std::cout << "Headers:" << std::endl;
	for (std::map<std::string,
			std::string>::const_iterator it = _headers.begin();
			it != _headers.end(); ++it)
	{
		std::cout << "	" << it->first << ": " << it->second << std::endl;
	}

	std::cout << "Body: " << _body << std::endl;
	std::cout << "Error Code: " << _errorCode << std::endl;
}

void	Request::_getElemRequestLine(std::string requestLine)
{
	std::string *tab[3] = {&this->_method, &this->_target, &this->_pVersion};
	for (unsigned int i = 0; i < 3; i++)
	{
		*tab[i] = requestLine.substr(0, requestLine.find(" "));
		if (i == 2)
			break ;
		if (tab[i]->empty() || tab[i]->compare(requestLine) == 0)
		{
			this->_errorCode = 400;
			return ;
		}
		requestLine.erase(0, tab[i]->length() + 1);
		if (requestLine.empty())
		{
			this->_errorCode = 400;
			return ;
		}
	}
}

void	Request::_checkWhitespaceElemRequestLine(void)
{
	if (this->_method.find(" ") != std::string::npos)
		this->_errorCode = 400;
	else if (this->_target.find(" ") != std::string::npos)
		this->_errorCode = 400;
	else if (this->_pVersion.find(" ") != std::string::npos)
		this->_errorCode = 400;
}

void	Request::_getRequestLine(std::string &r)
{
	std::string	requestLine = r.substr(0, r.find("\r\n"));
	if (requestLine.empty() || requestLine.compare(r) == 0)
	{
		this->_errorCode = 400;
		return ;
	}
	r.erase(0, requestLine.length() + 2);
	this->_getElemRequestLine(requestLine);
	if (this->_errorCode)
		return ;
	this->_checkWhitespaceElemRequestLine();
	std::string method[3] = {"GET", "POST", "DELETE"};
	bool found = (std::find(method, method + 3, this->_method) != method + 3);
	if (found == false)
	{
		this->_errorCode = 501;
		return ;
	}
	//check target: format and get query
	std::string tmp = this->_target.substr(0, this->_target.find("?"));
	if (tmp.empty())
	{
		this->_errorCode = 400;
		return ;
	}
	else if (tmp.compare(this->_target) == 0)
		tmp.clear();
	else
	{
		this->_query = this->_target.substr(tmp.size() + 1);
		this->_target = tmp;
	}
}

static inline std::string& rtrim(std::string& s, const char* t)
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

static inline std::string& ltrim(std::string& s, const char* t)
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

static inline std::string& trim(std::string& s, const char* t)
{
	return ltrim(rtrim(s, t), t);
}

static bool isStringOnlyDigits(const std::string& str)
{
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (!std::isdigit(str[i])) {
			return false;
		}
	}
	return true;
}

static void	toUpperString(std::string &str)
{
	for (std::string::iterator it = str.begin(); it != str.end(); ++it)
		*it = std::toupper(*it);
}

void	Request::_checkContentLength(const std::string &r)
{
	std::string cl = "CONTENT-LENGTH";
	std::string te = "TRANSFER-ENCODING";
	if (this->_headers.find(cl) != this->_headers.end())
	{
		if (isStringOnlyDigits(this->_headers[cl]) == false || r.empty())
			this->_errorCode = 400;
	}
	else if (this->_headers.find(te) != this->_headers.end())
	{
		if (r.empty())
			this->_errorCode = 400;
		else
		{
			toUpperString(this->_headers[te]);
			if (this->_headers[te].compare("CHUNKED"))
				this->_errorCode = 411;
		}
	}
	else if (r.empty() == false)
		this->_errorCode = 411;
}

void	Request::_checkHost(void)
{
	std::string	host = "HOST";
	if (this->_headers.find(host) == this->_headers.end())
	{
		if (this->_pVersion.compare("HTTP/1.1") == 0)
			this->_errorCode = 400;
	}
	else
	{
		std::size_t	found = this->_headers[host].find_last_of(":");
		if (found != std::string::npos)
		{
			this->_host = this->_headers[host].substr(0, found);
			if (isStringOnlyDigits(this->_headers[host].substr(found + 1)) == false)
				this->_errorCode = 400;
			else
			{
				std::stringstream tmp;
				tmp << this->_headers[host].substr(found + 1);
				tmp >> this->_port;
			}
		}
		else
			this->_host = this->_headers[host];
	}
}

void	Request::_checkBodyLength(void)
{
	std::stringstream tmp;

	tmp << this->_headers["CONTENT-LENGTH"];
	unsigned long length;
	tmp >> length;
	if (length != this->_body.length())
		this->_errorCode = 400;
	//check body length compared to what has been written in conf file
}

void	Request::_getHeaders(std::string &r)
{
	std::string line;
	while (r.empty() == 0)
	{
		line = r.substr(0, r.find("\r\n"));
		if (line.empty())
			break ;
		if (line.compare(r) == 0)
		{
			this->_errorCode = 400;
			return ;
		}
		r.erase(0, line.length() + 2);
		std::string key = line.substr(0, line.find(":"));
		toUpperString(key);
		if (key.empty() || key.compare(r) == 0
				|| this->_headers.find(key) != this->_headers.end())
		{
			this->_errorCode = 400;
			return ;
		}
		line.erase(0, key.length() + 1);
		if (line.empty())
		{
			this->_errorCode = 400;
			return ;
		}
		std::string value = trim(line, " \t\n\r\f\v");
		if (value.empty())
		{
			this->_errorCode = 400;
			return ;
		}
		if (this->_headers.find(key) != this->_headers.end())
		{
			this->_errorCode = 400;
			return ;
		}
		this->_headers[key] = value;
	}
	if (r.empty())
	{
		this->_errorCode = 400;
		return ;
	}
	else if (line.empty())
		r.erase(0, 2);
	this->_checkContentLength(r);
	if (this->_errorCode)
		return ;
	this->_checkHost();
	this->_body = r;
}

Request::Request(std::string r): _port(80), _errorCode(0)
{
	this->_getRequestLine(r);
	if (this->_errorCode)
		return ;
	this->_getHeaders(r);
}

Request::~Request(void) {}

const std::string& Request::getMethod() const {
    return this->_method;
}

const std::string& Request::getTarget() const {
    return this->_target;
}

const std::string& Request::getQuery() const {
    return this->_query;
}

const std::string& Request::getVersion() const {
    return this->_pVersion;
}

const std::string& Request::getHost() const {
    return this->_host;
}

unsigned int Request::getPort() const {
    return this->_port;
}

std::map<std::string, std::string>& Request::getHeaders() {
    return this->_headers;
}

const std::string& Request::getBody() const {
    return this->_body;
}

unsigned int Request::getErrorCode() const {
    return this->_errorCode;
}

void	Request::setErrorCode(const unsigned int err)
{
	this->_errorCode = err;
}
const std::string &Request::getErrorMsg() const {
    return this->_errorMsg;
}

void	Request::setErrorMsg(const std::string &txt)
{
	this->_errorMsg = txt;
}

