/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationObject.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:50:53 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/23 10:19:11 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ConfigurationObject.hpp"

ConfigurationObject::ConfigurationObject(void): isBodySize(false)
{
	return ;
}

ConfigurationObject::~ConfigurationObject(void)
{
	return ;
}

void ConfigurationObject::printParameters() const {
	std::cout << "Host: " << host << std::endl;
	std::cout << "Port: " << port << std::endl;

	std::cout << "Server Names: ";
	for (size_t i = 0; i < server_names.size(); ++i) {
		std::cout << server_names[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "Default Error Pages: ";
	for (std::map<unsigned int, std::string>::const_iterator it = defaultErrorPages.begin();
			it != defaultErrorPages.end(); ++it) {
		std::cout << "{" << it->first << ": " << it->second << "} ";
	}
	std::cout << std::endl;

	if (this->isBodySize)
		std::cout << "Body Size: " << bodySize << std::endl;

	// Printing routes might need a custom implementation of Route's print method
	std::cout << "Routes: " << std::endl;
	for (std::map<std::string, Route>::const_iterator it = routes.begin(); it != routes.end(); ++it) {
		std::cout << "=> Route " << it->first << " <=" << std::endl;
		it->second.printRoute(); // Assuming Route has a print method
	}
}

ConfigurationObject& ConfigurationObject::operator=(const ConfigurationObject& other) {
	if (this != &other) { // Self-assignment check
		this->host = other.host;
		this->port = other.port;
		this->server_names = other.server_names;
		this->defaultErrorPages = other.defaultErrorPages;
		this->isBodySize = other.isBodySize;
		this->bodySize = other.bodySize;
		this->routes = other.routes;
	}
	return *this;
}

const std::string	&ConfigurationObject::getErrorPage(const unsigned int &code) const
{
	return (this->defaultErrorPages.at(code));
}

