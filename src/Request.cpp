/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:13:32 by hboissel          #+#    #+#             */
/*   Updated: 2023/10/05 19:56:09 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Request.hpp"

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
	//std::cout << requestLine << std::endl;
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
		this->_errorCode = 501;
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

void	Request::_checkContentLength(void)
{
	std::string cl = "CONTENT-LENGTH";
	std::string te = "TRANSFER-ENCODING";
	if (this->_headers.find(cl) !=
}

void	Request::_getHeaders(std::string &r)
{
	while (r.empty() == 0)
	{
		std::string	line = r.substr(0, r.find("\r\n"));
		if (line.empty())
			return ;
		if (line.compare(r) == 0)
		{
			this->_errorCode = 400;
			return ;
		}
		r.erase(0, line.length() + 2);
		std::string key = line.substr(0, line.find(":"));
		for (std::string::iterator it = key.begin(); it != key.end(); ++it)
			*it = std::toupper(*it);
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
		this->_headers[key] = value;
	}
}

Request::Request(std::string r): _errorCode(0)
{
	this->_getRequestLine(r);
	if (this->_errorCode)
	{
		//std::cout << "error: " << this->_errorCode << std::endl;
		return ;
	}
	this->_getHeaders(r);

	std::cout << "Contents of the headers:" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin();
			it != this->_headers.end(); ++it)
	{
		std::cout << it->first << "=" << it->second << "$" << std::endl;
	}
}

Request::~Request(void) {}
