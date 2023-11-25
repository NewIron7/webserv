/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets_redirection.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 11:11:50 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/19 11:58:32 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Sockets.hpp"

static std::string create301Response(const std::string& newLocation) {
	std::ostringstream response;
	response << "HTTP/1.1 301 Moved Permanently\r\n";
	response << "Location: " << newLocation << "\r\n";
	response << "Connection: close\r\n";
	response << "Content-Type: text/html\r\n";
	response << "Content-Length: 114\r\n\r\n";
	response << "<html><head><title>301 Moved Permanently</title></head><body>\r\n";
	response << "<h1>Moved Permanently</h1>\r\n";
	response << "<p>The document has moved <a href=\"" << newLocation << "\">here</a>.</p>\r\n";
	response << "</body></html>\r\n";

	return response.str();
}

void	Sockets::_processRedirection(const ConfigurationObject &currentConfig)
{
	std::string targetTmp = this->oRequest.getTarget();
	Route realTarget;
	std::size_t sizeRoute = 0;

	realTarget.empty = true;

	this->_getRealRoute(currentConfig, targetTmp, realTarget, sizeRoute);
	if (realTarget.empty == false && realTarget.redirection.empty() == false)
	{
		this->response = create301Response(realTarget.redirection);
		//std::cout << this->response << std::endl;
	}
}
