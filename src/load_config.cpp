#include "Server.hpp"
#include "ServerLocation.hpp"
#include <fstream>
#include <iostream>
#include <string>

static ServerLocation	read_location_block(std::ifstream& file, std::string& line, int& i)
{
	ServerLocation	location;

	location.setLocation(line.substr(9));
	getline(file, line);
	i++;
	while (file && line.compare(0, 9, "location.") == 0)
	{
		if (line.compare(9, 13, "limit_except="))
			location.setLimit_except(line.substr(9 + 13));
		else if (line.compare(9, 11, "proxy_pass="))
			location.setProxy_pass(line.substr(9 + 11));
		else if (line.compare(9, 6, "alias="))
			location.setAlias(line.substr(9 + 6));
		else if (line.compare(9, 10, "autoindex="))
			location.setAutoindex(line.substr(9 + 10));
		else if (line.compare(9, 6, "index="))
			location.setIndex(line.substr(9 + 6));
		else if (line.compare(9, 4, "cgi."))
			location.addCgi(line.substr(9 + 4));
		else if (line.compare(9, 13, "upload_store."))
			location.addUpload_store(line.substr(9 + 13));
		else
		{
			std::cerr << "Error: Invalid configuration instruction!" << std::endl
				<< "Line " << i << ": " << line << std::endl;
			throw std::exception();
		}
		getline(file, line);
		i++;
	}
	return location;
}

static Server	read_server_block(std::ifstream& file, std::string& line, int& i)
{
	Server	server;
	bool	to_read;

	if (line.compare("[Server]") != 0)
	{
		std::cerr << "Error: All configuration instructions must be placed inside a server block!" << std::endl
			<< "Line " << i << ": " << line << std::endl;
		throw std::exception();
	}
	getline(file, line);
	i++;
	while (file && line.compare("[Server]") != 0)
	{
		to_read = true;
		if (!line.empty() && line[0] != '#')
		{
			if (line.compare(0, 13, "server_names=") == 0)
				server.setServer_names(line.substr(13));
			else if (line.compare(0, 5, "port=") == 0)
				server.setPort(line.substr(5));
			else if (line.compare(0, 5, "host=") == 0)
				server.setHost(line.substr(5));
			else if (line.compare(0, 9, "err_code.") == 0)
				server.addErr_code(line.substr(9));
			else if (line.compare(0, 10, "body_size=") == 0)
				server.setBody_size(line.substr(10));
			else if (line.compare(0, 9, "location=") == 0)
			{
				server.addLocation(read_location_block(file, line, i));
				to_read = false;
			}
			else
			{
				std::cerr << "Error: Invalid configuration instruction!" << std::endl
					<< "Line " << i << ": " << line << std::endl;
				throw std::exception();
			}
		}
		if (to_read)
		{
			getline(file, line);
			i++;
		}
	}
	server.fillDefaults();
	return server;
}

std::vector<Server>	load_config(const char *filename)
{
	std::ifstream	file;
	std::string		line;
	int				i;
	std::vector<Server>	config;

	file.open(filename);
	if (!file.is_open())
		throw std::exception();
	i = 1;
	do
	{
		getline(file, line);
		i++;
	} while (file && (line.empty() || line[0] == '#'));
	if (!file)
	{
		std::cerr << "Error: Empty config!" << std::endl;
		throw std::exception();
	}
	while (file)
	{
		server = read_server_block(file, line, i);
		config.push_back(server);
	}
	file.close();
	return config;
}
