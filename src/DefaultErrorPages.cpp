/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultErrorPages.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 15:09:16 by hboissel          #+#    #+#             */
/*   Updated: 2023/10/07 18:07:22 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "DefaultErrorPages.hpp"

std::string	DefaultErrorPages::genByCode(std::string code, std::string name, std::string details)
{
	std::string gen = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><met";
	gen += "a name=\"viewport\" content=\"width=device-width,initial-scale=1.0\">";
	gen += "<title>";
	gen += code + " - " + name
		+ "</title><style>body{font-family:Arial,sans-serif;background-";
	gen += "color:#f5f5f5;text-align:center;}.";
	gen += "container{background-color:#fff;border-radius:8px;box-shadow:";
	gen	+= "0 4px 8px rgba(0,0,0,.2);padding:20px;margin:0 ";
	gen	+= "auto;max-width:400px;}h1{color:#e74c3c;}p{color:#333;";
	gen	+= "font-size:18px;}</style></head><body><div ";
	gen	+= "class=\"container\"><h1>";
	gen += code + " - " + name;
	gen += "</h1><p>" + details + "</p></div></body></html>";
	
	std::string headers = "HTTP/1.1 " + code + " " + name + "\r\n";
	headers += "Content-Type: text/html\r\nContent-Length: "
		+ SSTR(gen.length()) + "\r\n\r\n";
	return (headers + gen);
}

const std::string	DefaultErrorPages::generate(unsigned int code, std::string details)
{
	std::string page;

	switch (code)
	{
		// 4xx Client Errors
		case 400:
			page = genByCode("400", "Bad Request", details);
			break;
		case 401:
			page = genByCode("401", "Unauthorized", details);
			break;
		case 402:
			page = genByCode("402", "Payment Required", details);
			break;
		case 403:
			page = genByCode("403", "Forbidden", details);
			break;
		case 404:
			page = genByCode("404", "Not Found", details);
			break;
		case 405:
			page = genByCode("405", "Method Not Allowed", details);
			break;
		case 406:
			page = genByCode("406", "Not Acceptable", details);
			break;
		case 407:
			page = genByCode("407", "Proxy Authentication Required", details);
			break;
		case 408:
			page = genByCode("408", "Request Timeout", details);
			break;
		case 409:
			page = genByCode("409", "Conflict", details);
			break;
		case 410:
			page = genByCode("410", "Gone", details);
			break;
		case 411:
			page = genByCode("411", "Length Required", details);
			break;
		case 412:
			page = genByCode("412", "Precondition Failed", details);
			break;
		case 413:
			page = genByCode("413", "Payload Too Large", details);
			break;
		case 414:
			page = genByCode("414", "URI Too Long", details);
			break;
		case 415:
			page = genByCode("415", "Unsupported Media Type", details);
			break;
		case 416:
			page = genByCode("416", "Range Not Satisfiable", details);
			break;
		case 417:
			page = genByCode("417", "Expectation Failed", details);
			break;
		case 418:
			page = genByCode("418", "I'm a teapot", details);
			break;
		case 421:
			page = genByCode("421", "Misdirected Request", details);
			break;
		case 422:
			page = genByCode("422", "Unprocessable Entity", details);
			break;
		case 423:
			page = genByCode("423", "Locked", details);
			break;
		case 424:
			page = genByCode("424", "Failed Dependency", details);
			break;
		case 425:
			page = genByCode("425", "Too Early", details);
			break;
		case 426:
			page = genByCode("426", "Upgrade Required", details);
			break;
		case 428:
			page = genByCode("428", "Precondition Required", details);
			break;
		case 429:
			page = genByCode("429", "Too Many Requests", details);
			break;
		case 431:
			page = genByCode("431", "Request Header Fields Too Large", details);
			break;
		case 451:
			page = genByCode("451", "Unavailable For Legal Reasons", details);
			break;

			// 5xx Server Errors
		case 500:
			page = genByCode("500", "Internal Server Error", details);
			break;
		case 501:
			page = genByCode("501", "Not Implemented", details);
			break;
		case 502:
			page = genByCode("502", "Bad Gateway", details);
			break;
		case 503:
			page = genByCode("503", "Service Unavailable", details);
			break;
		case 504:
			page = genByCode("504", "Gateway Timeout", details);
			break;
		case 505:
			page = genByCode("505", "HTTP Version Not Supported", details);
			break;
		case 506:
			page = genByCode("506", "Variant Also Negotiates", details);
			break;
		case 507:
			page = genByCode("507", "Insufficient Storage", details);
			break;
		case 508:
			page = genByCode("508", "Loop Detected", details);
			break;
		case 510:
			page = genByCode("510", "Not Extended", details);
			break;
		case 511:
			page = genByCode("511", "Network Authentication Required", details);
			break;

		default:
			page = genByCode("500", "Internal Server Error", "An unexpected error occurred.");
			break;
	}


	return (page);
}
