/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultErrorPages.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 15:09:16 by hboissel          #+#    #+#             */
/*   Updated: 2023/10/01 19:01:32 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "DefaultErrorPages.hpp"

std::string	DefaultErrorPages::genByCode(std::string name, std::string details)
{
	std::string gen = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><meta\
name=\"viewport\" content=\"width=device-width,initial-s\
cale=1.0\"><title>";
	gen += name + "</title><style>body{font-family:Arial,sans-serif;background-\
color:#f5f5f5;text-align:center;}.container{background-color:#fff;borde\
r-radius:8px;box-shadow:0 4px 8px rgba(0,0,0,.2);padding:20px;margin:0 \
auto;max-width:400px;}h1{color:#e74c3c;}p{color:#333;font-size:18px;}</\
style></head><body><div class=\"container\"><h1>";
	gen += name + "</h1><p>" + details + "</p></div></body></html>";
	return (gen);
}

const std::string	DefaultErrorPages::generate(unsigned int code, std::string details)
{
	std::string	page;

	switch (code)
	{
		case 400:
			page = genByCode("400 - Bad Request", details);
			break ;
		case 403:
			page = genByCode("403 - Forbidden", details);
			break ;
		case 404:
			page = genByCode("404 - Not Found", details);
			break ;
	}
	return (page);
}
