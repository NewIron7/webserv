/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:52:15 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/13 11:53:35 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Route.hpp"

Route::Route(void): directoryListing(false), uploadedFile(false)
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
    }
