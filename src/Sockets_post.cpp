/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets_post.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 09:33:08 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/17 13:19:31 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Sockets.hpp"

void	Sockets::_createFile(const std::string& filePath) {
	std::ofstream file(filePath.c_str());
	if (!file.is_open()) {
		this->oRequest.setCodeMsg(400, "Cannot create the file: " + filePath);
		throw Sockets::Error();
	}

	file.close();
}

static bool fileExists(const std::string& filename) {
	struct stat buffer;
	return (stat(filename.c_str(), &buffer) == 0);
}

static bool isRegularFile(const std::string& filename) {
	struct stat buffer;
	if (stat(filename.c_str(), &buffer) != 0) {
		return false; // Unable to get file status
	}
	return S_ISREG(buffer.st_mode);
}

static bool hasWritePermission(const std::string& filename) {
	return (access(filename.c_str(), W_OK) == 0);
}

bool    Sockets::_checkWritePermission(const std::string &file)
{
	if (!(fileExists(file) && isRegularFile(file) && hasWritePermission(file)))
		return (false);
	return (true);
}

void Sockets::_appendToFile(const std::string& filename, const std::string& content) {
	std::ofstream outputFile;
	outputFile.open(filename.c_str(), std::ios::app); // Open the file in append mode

	if (outputFile.is_open()) {
		outputFile << content; // Append content to the end of the file
		outputFile.close(); // Close the file
		this->oRequest.setCodeMsg(204, this->oRequest.getTarget() + " appended successfully.");
		throw Sockets::Error();
	} else {
		this->oRequest.setCodeMsg(500, "Error while writing in the file");
		throw Sockets::Error();
	}
}

std::string Sockets::_generateHTTPResponseHeaderPOST(void) {
	std::ostringstream response;

	// Status line: HTTP version, status code, and status message
	response << "HTTP/1.1 " << this->oRequest.getErrorCode() << " "
		<< DefaultErrorPages::statusMap[this->oRequest.getErrorCode()] << "\r\n";

	// Headers
	response << "Content-Length: " << this->response.size() << "\r\n";
	response << "Server: webserv/0.1\r\n";
	// Add other headers as needed

	// Blank line indicating the end of the headers
	response << "\r\n";

	return response.str();
}

static std::string getLastPartOfURI(const std::string& uri) {
    size_t found = uri.find_last_of("/"); // Find the last occurrence of '/' or '\'
    if (found != std::string::npos && found != uri.length() - 1) {
        return uri.substr(found + 1); // Return the substring after the last occurrence
    }
    // Return the full URI if no '/' or '\' is found or if it's the last character
    return "";
}

void Sockets::_parseMultipartFormData(const std::string& body,
	const std::string& boundary) {
    // Split the body into different parts using the boundary
    std::vector<std::string> parts;
    size_t pos = 0;
    while ((pos = body.find(boundary, pos)) != std::string::npos) {
		std::cout << "pos:" << pos << std::endl;
        size_t nextPos = pos + boundary.size();
		std::cout << "nextPos:" << nextPos << std::endl;
        if ((nextPos = body.find(boundary, nextPos)) != std::string::npos) {
			std::cout << "nextPos:" << nextPos << std::endl;
            nextPos = body.rfind("\r\n\r\n", nextPos); // Move before the CRLF CRLF
			std::cout << "nextPos:" << nextPos << std::endl;
            size_t endPos = body.find(boundary, nextPos);
            parts.push_back(body.substr(pos + boundary.size(), nextPos));
            pos = endPos;
        }
		else
			break ;
    }

    // Process each part (field or file)
    for (std::vector<std::string>::const_iterator it = parts.begin();
		it != parts.end(); ++it) {
        // TODO: Implement logic to handle each part (e.g., extracting field names, file contents, etc.)
        std::cout << "Part: " << *it << std::endl;
        // Add your parsing logic here
    }
}

std::string Sockets::_getBoundaryFromContentType(const std::string& contentType) {
    std::string boundaryString = "boundary=";
    size_t pos = contentType.find(boundaryString);
    if (pos != std::string::npos) {
        pos += boundaryString.length();
        std::string boundary = contentType.substr(pos);
        return boundary;
    }
    this->oRequest.setCodeMsg(400, "No boundary in Content-Type");
	throw Sockets::Error();
}

void	Sockets::_processUpload(const Route &target)
{
	std::string contentType = this->oRequest.getHeader("CONTENT-TYPE");

	if (contentType == "text/plain" || contentType == "application/x-www-form-urlencoded")
	{
		std::string filename = getLastPartOfURI(target.location);
		if (filename.empty())
		{
			this->oRequest.setCodeMsg(400, "No file name");
			throw Sockets::Error();
		}
		//check if target.uploadPath is a directory and we can create a file there
		//process the upload
		this->_createFile(target.location);
		this->_appendToFile(target.location, this->oRequest.getBody());

		this->oRequest.setCodeMsg(201, this->oRequest.getTarget() + " created successfully.");
		throw Sockets::Error();
	}
	else if (contentType.find("multipart/form-data;") != std::string::npos)
	{
		//this->_parseMultipartFormData(this->oRequest.getBody(),
		//	this->_getBoundaryFromContentType(contentType));
		this->oRequest.setCodeMsg(501, contentType);
		throw Sockets::Error();
	}
	else if (contentType.empty())
	{
		this->oRequest.setCodeMsg(400, "No Content-Type in the request");
		throw Sockets::Error();
	}
	else
	{
		this->oRequest.setCodeMsg(501, contentType + " is not implemented yet");
		throw Sockets::Error();
	}	
}

void	Sockets::_processPOSTMethod(const Route &target)
{
	//check if file is a real file that can be write
	if (this->_checkWritePermission(target.location))
	{
		//if it is, write in it the body
		this->_appendToFile(target.location, this->oRequest.getBody());
	}//else if, check if location is the route (look at dir) and upload accepted
	else if (target.uploadedFile)
	{
		this->_processUpload(target);
	}//else request cannot be processed
	else
	{
		this->oRequest.setCodeMsg(400, "Your post request cannot be processed");
		throw Sockets::Error();
	}

}
