/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_process.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 17:27:29 by hboissel          #+#    #+#             */
/*   Updated: 2023/10/07 18:05:27 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Request.hpp"

std::string	Request::process(void)
{
	std::string response = DefaultErrorPages::generate(418, "Hello World!"); 
	if (this->_errorCode)
		return (DefaultErrorPages::generate(this->_errorCode,
					"Error while parsing the request"));
	//process the request and return std::string with the server response
	
	return (response);
}
