#include "Sockets.hpp"

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

static bool hasReadPermission(const std::string& filename) {
	return (access(filename.c_str(), R_OK) == 0);
}

static bool isDirectory(const std::string& filename) {
    struct stat buffer;
	if (stat(filename.c_str(), &buffer) != 0) {
		return false;
	}
	return S_ISDIR(buffer.st_mode);
}

std::string	Sockets::_processDirListing(const std::string &path)
{
	if (!hasReadPermission(path)) {
		this->oRequest.setCodeMsg(403, "The program doesnt have the permission to open this directory");
		throw Sockets::Error();
	}
	
	std::string result = "<html><head><title>Directory Listing</title>";
    result += "<style>body { font-family: Arial, sans-serif; background-color: #f4f4f4; }";
    result += "h1 { color: #333; text-align: center; } ul { list-style-type: none; margin: 0; padding: 0; }";
    result += "li { padding: 8px; border-bottom: 1px solid #ccc; background-color: #fff; }";
    result += "a { text-decoration: none; display: block; padding: 8px 16px; color: #000; }";
    result += "a:hover { background-color: #f4f4f4; } .dir { color: #007bff; } .file { color: #28a745; }</style>";
    result += "</head><body><h1>Directory Listing</h1><ul>";
    DIR* dir;
    struct dirent* entry;
    
    const char* dirPath = path.c_str();
    
    if ((dir = opendir(dirPath)) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                result += "<li>";
                result += "<a href='";
                result += entry->d_name;
                if (entry->d_type == DT_DIR) {
                    result += "/";
                	result += "' class='dir'>";
                } else {
                    result += "' class='file'>";
                }
                result += entry->d_name;
				if (entry->d_type == DT_DIR) {
                    result += "/";
                }
                result += "</a></li>";
            }
        }
        closedir(dir);
    } else {
        this->oRequest.setCodeMsg(500, "Error while reading the directory");
		throw Sockets::Error();
    }
    
    result += "</ul></body></html>";
    
    return result;
}

std::string Sockets::_readFile(const std::string& filename, const Route &target) {
	Request	&req = this->oRequest;

	if (!fileExists(filename)) {
		req.setCodeMsg(404, "No ressource " + this->oRequest.getTarget());
		throw Sockets::Error();
	}
	
	if (!isRegularFile(filename)) {
		if (target.directoryListing && isDirectory(filename))
			return (this->_processDirListing(filename));
		else
		{
			req.setCodeMsg(404, "Not a file");
			throw Sockets::Error();
		}
	}

	if (!hasReadPermission(filename)) {
		req.setCodeMsg(403, "The program doesnt have the permission to open this file");
		throw Sockets::Error();
	}

	std::ifstream file(filename.c_str());

	if (!file.is_open()) {
		req.setCodeMsg(500, "Error while opening the file");
		throw Sockets::Error();
	}

	std::ostringstream content;
	std::string line;

	// Read the file content line by line
	while (std::getline(file, line)) {
		content << line << '\n';
	}

	// Check for I/O errors during reading
	if (file.bad()) {
		std::cerr << "Error reading file: " << strerror(errno) << std::endl;
		req.setCodeMsg(500, "Error while reading the file");
		throw Sockets::Error();
	}

	return content.str();
}
