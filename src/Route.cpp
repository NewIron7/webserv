/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:52:15 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/15 09:26:01 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Route.hpp"

Route::Route(void): directoryListing(false), uploadedFile(false), empty(false)
{
	return ;
}

Route::~Route(void)
{
	return ;
}

void Route::printRoute() const {
	std::cout << "Methods: ";
	for (size_t i = 0; i < methods.size(); ++i) {
		std::cout << methods[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "Redirection: " << redirection << std::endl;
	std::cout << "Location: " << location << std::endl;
	std::cout << "Directory Listing: " << (directoryListing ? "true" : "false") << std::endl;
	std::cout << "Root: " << root << std::endl;
	std::cout << "CGI Path: " << cgiPath << std::endl;
	std::cout << "CGI Extension: " << cgiExtension << std::endl;
	std::cout << "Uploaded File: " << (uploadedFile ? "true" : "false") << std::endl;
	std::cout << "Upload Path: " << uploadPath << std::endl;
	std::cout << "Empty: " << (empty ? "true" : "false") << std::endl;
}

Route& Route::operator=(const Route& other) {
	if (this != &other) { // Self-assignment check
		this->methods = other.methods;
		this->redirection = other.redirection;
		this->location = other.location;
		this->directoryListing = other.directoryListing;
		this->root = other.root;
		this->cgiPath = other.cgiPath;
		this->cgiExtension = other.cgiExtension;
		this->uploadedFile = other.uploadedFile;
		this->uploadPath = other.uploadPath;
		this->empty = other.empty;
	}
	return *this;
}
