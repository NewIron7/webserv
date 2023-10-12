/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets_process.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 12:39:57 by hboissel          #+#    #+#             */
/*   Updated: 2023/10/12 08:11:10 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Sockets.hpp"

void	Sockets::_checkBodyEmpty(void)
{
	Request	&req = this->oRequest;
	if (req.getBody().empty() == false)
	{
		req.setErrorCode(400);
		throw Sockets::Error();
	}
}

void	Sockets::_processGET(void)
{
	Request	&req = this->oRequest;
	
	this->_checkBodyEmpty();
	(void)req;
}

void	Sockets::_processPOST(void)
{
	Request	&req = this->oRequest;
	(void)req;
}

void	Sockets::_processDELETE(void)
{
	Request	&req = this->oRequest;
	
	this->_checkBodyEmpty();
	(void)req;
}


void	Sockets::_processMethod(void)
{
	if (this->oRequest.getMethod() == "GET")
		this->_processGET();
	else if (this->oRequest.getMethod() == "POST")
		this->_processPOST();
	else
		this->_processDELETE();
}

bool	Sockets::_isCGI(void)
{
	return (false);
}

void	Sockets::_processCGI(void)
{
	//execute CGI
}

void	Sockets::process(void)
{
	this->oRequest = Request(this->request);
	if (this->oRequest.getErrorCode())
		this->response = DefaultErrorPages::generate(
			this->oRequest.getErrorCode(), "Error while parsing the request");
	//process the request and return std::string with the server response
	try
	{
		//before check if its a CGI call
		if (this->_isCGI())
			this->_processCGI();
		else
			this->_processMethod();
	}
	catch(const std::exception& e)
	{
		this->response = DefaultErrorPages::generate(
			this->oRequest.getErrorCode(), "Error while parsing the request");
	}
}

const char* Sockets::Error::what() const throw()
{
		return ("Error");
}
