/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_process.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 17:27:29 by hboissel          #+#    #+#             */
/*   Updated: 2023/10/07 17:51:12 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Request.hpp"

std::string	Request::process(void)
{
	std::string response = DefaultErrorPages::generate(404, "Hello World!"); 

	//process the request and return std::string with the server response
	
	return (response);
}
