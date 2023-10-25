#include "Server.hpp"

Server::Server()
{
	_port = -1;
	_host = "";
	_default = false;
	_body_size = -1;
}

Server::~Server() {}

Server::Server(const Server& old)
{
	_port = old.getPort();
	_host = old.getHost();
	_server_names = old.getServer_names();
	_default = old.getDefault();
	_err_codes = old.getErr_codes();
	_body_size = old.getBody_size();
	_locations = old.getLocations();
}

Server&	Server::operator=(const Server& old)
{
	if (this == &old)
		return *this;

	_port = old.getPort();
	_host = old.getHost();
	_server_names = old.getServer_names();
	_default = old.getDefault();
	_err_codes = old.getErr_codes();
	_body_size = old.getBody_size();
	_locations = old.getLocations();
	return *this;
}

int				Server::getPort() const
{
	return	_port;
}

std::string	Server::getHost() const
{
	return	_host;
}

std::vector<std::string>	Server::getServer_names() const
{
	return _server_names;
}

bool			Server::getDefault() const
{
	return	_default;
}

std::map<int, std::string>	Server::getErr_codes() const
{
	return	_err_codes;
}

int				Server::getBody_size() const
{
	return	_body_size;
}

std::vector<ServerLocation>	Server::getLocations() const
{
	return	_locations;
}

void	Server::setPort(int port)
{
	if (_port != -1)
		throw std::exception();
	_port = port;
}

void	Server::setHost(std::string host)
{
	if (_host != "")
		throw std::exception();
	_host = host;
}

void	Server::addServer_name(std::string server_name)
{
	_server_names.push_back(server_name);
}

void	Server::setDefault()
{
	_default = true;
}

void	Server::addErr_code(int code, std::string file)
{
	if (_err_codes.count(code))
		throw std::exception();
	_err_codes[code] = file;
}

void	Server::setBody_size(int body_size)
{
	if (_body_size != -1)
		throw std::exception();
	_body_size = body_size;
}

void	Server::addLocation(ServerLocation location)
{
	for (size_t i = 0; i < _locations.size(); i++)
	{
		if (_locations[i].getLocation() == location.getLocation())
			throw std::exception();
	}
	_locations.push_back(location);
}
