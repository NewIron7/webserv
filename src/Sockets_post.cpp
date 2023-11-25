/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets_post.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 09:33:08 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/18 06:14:11 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Sockets.hpp"

void	Sockets::_canCreateFileInDirectory(const std::string& path) {
	struct stat info;

	if (stat(path.c_str(), &info) != 0) {
		this->oRequest.setCodeMsg(400, "Bad path");
		throw Sockets::Error();
	} else if (!(info.st_mode & S_IFDIR)) {
		this->oRequest.setCodeMsg(400, "Path must be a directory");
		throw Sockets::Error();
	} else if (access(path.c_str(), W_OK) != 0) {
		this->oRequest.setCodeMsg(403, "Cannot create file");
		throw Sockets::Error();
	}
}

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

std::string	Sockets::_getFilenameMultipartFormData(const std::string &headers)
{
	std::string filename;
	size_t pos;

	if ((pos = headers.find("Content-Disposition:")) != std::string::npos)
	{
		if (pos != 0 && headers[pos - 1] != '\n')
		{
			this->oRequest.setCodeMsg(400, "Wrong Multipart/form-data format");
			throw Sockets::Error();
		}
		size_t	endLine = headers.find("\r\n", pos);
		std::string contentDisposition = headers.substr(pos, endLine);
		size_t	posFilename;
		size_t	posName;
		if (contentDisposition.find("form-data;") == std::string::npos
				|| (posName = contentDisposition.find("name=\"")) == std::string::npos)
		{
			this->oRequest.setCodeMsg(400,
					"Wrong Multipart/form-data format: name or form-data is missing");
			throw Sockets::Error();
		}
		posName += 6;
		size_t endName;
		if ((endName = contentDisposition.find("\"", posName)) == std::string::npos)
		{
			this->oRequest.setCodeMsg(400,
					"Wrong Multipart/form-data format: name bad value");
			throw Sockets::Error();
		}
		filename = contentDisposition.substr(posName, endName - posName);
		filename = "multipartFormData_" + filename;
		filename += ".txt";
		if ((posFilename = contentDisposition.find("filename=\""))
				!= std::string::npos)
		{
			posFilename += 10;
			size_t endFilename;
			if ((endFilename = contentDisposition.find("\"", posFilename))
					== std::string::npos)
			{
				this->oRequest.setCodeMsg(400,
						"Wrong Multipart/form-data format: filename bad value");
				throw Sockets::Error();
			}
			std::string filenameTMP = contentDisposition.substr(posFilename,
					endFilename - posFilename);
			if (filenameTMP.empty() == false)
				filename = filenameTMP;
		}
	}
	else
	{
		this->oRequest.setCodeMsg(400, "Wrong Multipart/form-data format");
		throw Sockets::Error();
	}
	return (filename);
}

void	Sockets::_parseContentMultipartFormData(const std::string& content,
		const Route &target)
{
	size_t pos;
	if ((pos = content.find("\r\n\r\n")) != std::string::npos)
	{
		std::string headers = content.substr(0, pos);
		std::string value = content.substr(pos + 4);
		//std::cout << "Headers:\n" << headers << "\nValue:\n" << value << std::endl;
		std::string filename = this->_getFilenameMultipartFormData(headers);
		//std::cout << "FILENAME=" << filename << std::endl;

		std::string location = target.uploadPath;
		if (location.size() && location[location.size() - 1] != '/')
			location += "/";
		location += filename;

		this->_createFile(location);
		this->_appendToFile(location, value);
		std::cerr << location << " created" << std::endl;
	}
	else
	{
		this->oRequest.setCodeMsg(400, "Wrong Multipart/form-data format");
		throw Sockets::Error();
	}
}

void Sockets::_parseMultipartFormData(const std::string& body,
		const std::string& boundary, const Route& target) {
	// Split the body into different parts using the boundary
	std::vector<std::string> parts;
	size_t pos = 0;
	while ((pos = body.find(boundary + "\r\n", pos)) != std::string::npos) {
		size_t nextPos = pos + boundary.size() + 2;
		size_t end;
		if ((end = body.find("\r\n" + boundary, nextPos)) != std::string::npos) {
			parts.push_back(body.substr(nextPos, end - nextPos));
			pos = end;
		}
		else
			break ;
	}

	// Process each part (field or file)
	for (std::vector<std::string>::const_iterator it = parts.begin();
			it != parts.end(); ++it) {
		// logic to handle each part (e.g., extracting field names, file contents, etc.)
		// and creation of files
		this->_parseContentMultipartFormData(*it, target);
	}
}

std::string Sockets::_getBoundaryFromContentType(const std::string& contentType) {
	std::string boundaryString = "boundary=";
	size_t pos = contentType.find(boundaryString);
	std::string	boundary;
	size_t end;

	if (pos != std::string::npos) {
		pos += boundaryString.length();
		if (contentType[pos] == '"'
				&& (end = contentType.find("\"", pos + 1) != std::string::npos))
		{
			boundary = contentType.substr(pos, end - pos);
		}
		else
			boundary = contentType.substr(pos);
		boundary = "--" + boundary;
		return boundary;
	}
	this->oRequest.setCodeMsg(400, "No boundary in Content-Type");
	throw Sockets::Error();
}

void	Sockets::_processUpload(const Route &target)
{
	std::string contentType = this->oRequest.getHeader("CONTENT-TYPE");

	if (contentType.find("text/plain") != std::string::npos
			|| contentType.find("application/x-www-form-urlencoded") != std::string::npos)
	{
		std::string filename = getLastPartOfURI(target.location);
		if (filename.empty())
		{
			this->oRequest.setCodeMsg(400, "No file name");
			throw Sockets::Error();
		}
		std::string location = target.uploadPath;
		if (location.size() && location[location.size() - 1] != '/')
			location += "/";
		location += filename;
		//check if target.uploadPath is a directory and we can create a file there
		//process the upload
		this->_createFile(location);
		this->_appendToFile(location, this->oRequest.getBody());

		this->oRequest.setCodeMsg(201, this->oRequest.getTarget() + " created successfully.");
		throw Sockets::Error();
	}
	else if (contentType.find("multipart/form-data;") != std::string::npos)
	{
		this->_canCreateFileInDirectory(target.uploadPath);

		std::string boundary = this->_getBoundaryFromContentType(contentType);

		//std::cout << this->oRequest.getBody().size() << std::endl;

		this->_parseMultipartFormData(this->oRequest.getBody(), boundary, target);

		this->oRequest.setCodeMsg(202, "Ressources created");
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
		this->oRequest.setCodeMsg(204, this->oRequest.getTarget() + " appended successfully.");
		throw Sockets::Error();
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
