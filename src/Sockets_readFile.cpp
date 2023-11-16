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

std::string Sockets::_readFile(const std::string& filename) {
	Request	&req = this->oRequest;

	if (!fileExists(filename)) {
		//std::cerr << "File doesn't exist: " << filename << std::endl;
		req.setErrorCode(404);
		throw Sockets::Error();
	}

	if (!isRegularFile(filename)) {
		//std::cerr << "Not a regular file: " << filename << std::endl;
		req.setErrorCode(404);
		throw Sockets::Error();
	}

	if (!hasReadPermission(filename)) {
		//std::cerr << "No read permission for file: " << filename << std::endl;
		req.setErrorCode(403);
		throw Sockets::Error();
	}

	std::ifstream file(filename.c_str()); // Open file in input mode

	if (!file.is_open()) {
		// Handle error if the file cannot be opened
		//std::cerr << "Error opening file: " << strerror(errno) << std::endl;
		req.setErrorCode(500);
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
		// Handle error if there was a problem reading the file
		//std::cerr << "Error reading file: " << strerror(errno) << std::endl;
		req.setErrorCode(500);
		throw Sockets::Error();
	}

	// Close the file explicitly (optional in C++98, as it's done automatically on destruction)
	file.close();

	// Return the content of the file as a string
	return content.str();
}
