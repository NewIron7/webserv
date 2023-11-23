/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultErrorPages.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 15:09:16 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/23 10:19:01 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "DefaultErrorPages.hpp"

#include "DefaultErrorPages.hpp"

std::map<unsigned int, std::string> DefaultErrorPages::statusMap = initStatusMap();
std::map<std::string, std::string> DefaultErrorPages::contentTypeMap = initContentTypeMap();

std::map<std::string, std::string> DefaultErrorPages::initContentTypeMap(void)
{
	std::map<std::string, std::string> contentTypeMap;

	// Common MIME types and file extensions
	contentTypeMap[".aac"] = "audio/aac";
	contentTypeMap[".abw"] = "application/x-abiword";
	contentTypeMap[".arc"] = "application/octet-stream";
	contentTypeMap[".avi"] = "video/x-msvideo";
	contentTypeMap[".azw"] = "application/vnd.amazon.ebook";
	contentTypeMap[".bin"] = "application/octet-stream";
	contentTypeMap[".bz"] = "application/x-bzip";
	contentTypeMap[".bz2"] = "application/x-bzip2";
	contentTypeMap[".csh"] = "application/x-csh";
	contentTypeMap[".css"] = "text/css";
	contentTypeMap[".csv"] = "text/csv";
	contentTypeMap[".doc"] = "application/msword";
	contentTypeMap[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	contentTypeMap[".eot"] = "application/vnd.ms-fontobject";
	contentTypeMap[".epub"] = "application/epub+zip";
	contentTypeMap[".gif"] = "image/gif";
	contentTypeMap[".htm"] = "text/html";
	contentTypeMap[".html"] = "text/html";
	contentTypeMap[".ico"] = "image/x-icon";
	contentTypeMap[".ics"] = "text/calendar";
	contentTypeMap[".jar"] = "application/java-archive";
	contentTypeMap[".jpeg"] = "image/jpeg";
	contentTypeMap[".jpg"] = "image/jpeg";
	contentTypeMap[".js"] = "text/javascript";
	contentTypeMap[".json"] = "application/json";
	contentTypeMap[".jsonld"] = "application/ld+json";
	contentTypeMap[".mid"] = "audio/midi";
	contentTypeMap[".midi"] = "audio/midi";
	contentTypeMap[".mpeg"] = "video/mpeg";
	contentTypeMap[".mpkg"] = "application/vnd.apple.installer+xml";
	contentTypeMap[".odp"] = "application/vnd.oasis.opendocument.presentation";
	contentTypeMap[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	contentTypeMap[".odt"] = "application/vnd.oasis.opendocument.text";
	contentTypeMap[".oga"] = "audio/ogg";
	contentTypeMap[".ogv"] = "video/ogg";
	contentTypeMap[".ogx"] = "application/ogg";
	contentTypeMap[".otf"] = "font/otf";
	contentTypeMap[".png"] = "image/png";
	contentTypeMap[".pdf"] = "application/pdf";
	contentTypeMap[".ppt"] = "application/vnd.ms-powerpoint";
	contentTypeMap[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	contentTypeMap[".rar"] = "application/x-rar-compressed";
	contentTypeMap[".rtf"] = "application/rtf";
	contentTypeMap[".sh"] = "application/x-sh";
	contentTypeMap[".svg"] = "image/svg+xml";
	contentTypeMap[".swf"] = "application/x-shockwave-flash";
	contentTypeMap[".tar"] = "application/x-tar";
	contentTypeMap[".tif"] = "image/tiff";
	contentTypeMap[".tiff"] = "image/tiff";
	contentTypeMap[".ttf"] = "font/ttf";
	contentTypeMap[".txt"] = "text/plain";
	contentTypeMap[".vsd"] = "application/vnd.visio";
	contentTypeMap[".wav"] = "audio/wav";
	contentTypeMap[".weba"] = "audio/webm";
	contentTypeMap[".webm"] = "video/webm";
	contentTypeMap[".webp"] = "image/webp";
	contentTypeMap[".woff"] = "font/woff";
	contentTypeMap[".woff2"] = "font/woff2";
	contentTypeMap[".xhtml"] = "application/xhtml+xml";
	contentTypeMap[".xls"] = "application/vnd.ms-excel";
	contentTypeMap[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	contentTypeMap[".xml"] = "application/xml";
	contentTypeMap[".xul"] = "application/vnd.mozilla.xul+xml";
	contentTypeMap[".zip"] = "application/zip";
	contentTypeMap[".3gp"] = "video/3gpp";
	contentTypeMap[".3g2"] = "video/3gpp2";
	contentTypeMap[".7z"] = "application/x-7z-compressed";

	return contentTypeMap;
}

std::map<unsigned int, std::string> DefaultErrorPages::initStatusMap(void)
{
	std::map<unsigned int, std::string> statusMap;

	statusMap[100] = "Continue";
	statusMap[101] = "Switching Protocols";
	statusMap[102] = "Processing";
	statusMap[103] = "Early Hints";

	statusMap[200] = "OK";
	statusMap[201] = "Created";
	statusMap[202] = "Accepted";
	statusMap[203] = "Non-Authoritative Information";
	statusMap[204] = "No Content";
	statusMap[205] = "Reset Content";
	statusMap[206] = "Partial Content";
	statusMap[207] = "Multi-Status";
	statusMap[208] = "Already Reported";
	statusMap[226] = "IM Used";

	statusMap[300] = "Multiple Choices";
	statusMap[301] = "Moved Permanently";
	statusMap[302] = "Found";
	statusMap[303] = "See Other";
	statusMap[304] = "Not Modified";
	statusMap[305] = "Use Proxy";
	statusMap[307] = "Temporary Redirect";
	statusMap[308] = "Permanent Redirect";

	statusMap[400] = "Bad Request";
	statusMap[401] = "Unauthorized";
	statusMap[402] = "Payment Required";
	statusMap[403] = "Forbidden";
	statusMap[404] = "Not Found";
	statusMap[405] = "Method Not Allowed";
	statusMap[406] = "Not Acceptable";
	statusMap[407] = "Proxy Authentication Required";
	statusMap[408] = "Request Timeout";
	statusMap[409] = "Conflict";
	statusMap[410] = "Gone";
	statusMap[411] = "Length Required";
	statusMap[412] = "Precondition Failed";
	statusMap[413] = "Payload Too Large";
	statusMap[414] = "URI Too Long";
	statusMap[415] = "Unsupported Media Type";
	statusMap[416] = "Range Not Satisfiable";
	statusMap[417] = "Expectation Failed";
	statusMap[418] = "I'm a teapot";
	statusMap[421] = "Misdirected Request";
	statusMap[422] = "Unprocessable Entity";
	statusMap[423] = "Locked";
	statusMap[424] = "Failed Dependency";
	statusMap[425] = "Too Early";
	statusMap[426] = "Upgrade Required";
	statusMap[428] = "Precondition Required";
	statusMap[429] = "Too Many Requests";
	statusMap[431] = "Request Header Fields Too Large";
	statusMap[451] = "Unavailable For Legal Reasons";

	statusMap[500] = "Internal Server Error";
	statusMap[501] = "Not Implemented";
	statusMap[502] = "Bad Gateway";
	statusMap[503] = "Service Unavailable";
	statusMap[504] = "Gateway Timeout";
	statusMap[505] = "HTTP Version Not Supported";
	statusMap[506] = "Variant Also Negotiates";
	statusMap[507] = "Insufficient Storage";
	statusMap[508] = "Loop Detected";
	statusMap[510] = "Not Extended";
	statusMap[511] = "Network Authentication Required";

	return statusMap;
}

const std::string &DefaultErrorPages::getContentType(const std::string &ext)
{
	if (contentTypeMap.find(ext) != contentTypeMap.end()) {
		return (DefaultErrorPages::contentTypeMap[ext]);
	} else {
		return (DefaultErrorPages::contentTypeMap[".txt"]);
	}

}

std::string DefaultErrorPages::addHeader(std::string code, std::string &name, std::string &gen)
{
	std::string headers = "HTTP/1.1 " + code + " " + name + "\r\n";
	headers += "Content-Type: text/html\r\nContent-Length: "
		+ SSTR(gen.length()) + "\r\n\r\n";
	return (headers + gen);
}

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


	return (addHeader(code, name, gen));
}

static std::string readFileContent(const std::string& filePath) {
	std::ifstream file(filePath.c_str());
	if (!file.is_open()) {
		throw std::runtime_error("Unable to open file");
	}

	try {
		std::ostringstream contentStream;
		contentStream << file.rdbuf();
		file.close();
		return contentStream.str();
	} catch (const std::exception& e) {
		file.close();
		throw std::runtime_error("Error reading file content: " + std::string(e.what()));
	}
}

const std::string	DefaultErrorPages::generate(unsigned int code,
		std::string details, const ConfigurationObject &currentConfig)
{
	try
	{
		const std::string &path = currentConfig.getErrorPage(code);
		try {
			std::string fileContent = readFileContent(path);
			return (DefaultErrorPages::addHeader(SSTR(code), statusMap[code], fileContent));
		} catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			std::cerr << "[err] Cannot open file "
				<< path << ". Default error page used instead" << std::endl;
			return (generate(code, details));
		}
	}
	catch(const std::exception& e)
	{
		return (generate(code, details));
	}

}

const std::string	DefaultErrorPages::generate(unsigned int code, std::string details)
{
	std::string page;

	switch (code)
	{
		case 200:
			page = genByCode("200", "OK", details);
			break;
		case 202:
			page = genByCode("202", "Created", details);
			break;
		case 204:
			page = genByCode("204", "No Content", details);
			break;
		case 201:
			page = genByCode("201", "Created", details);
			break;
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
