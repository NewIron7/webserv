#include "Server.hpp"

Server::Server()
{
	_port = -1;
	_host = -1;
	_servers_names = NULL;
	_default = false;
	_err_codes = NULL;
	_body_size = -1;
	_locations = NULL;
}

Server::~Server()
{
	delete[] _server_names;
	delete[] _err_codes;
	delete[] _locations;
}

Server::Server(const Server& old)
{
	_port = old.getPort();
	_host = old.getHost();
	_server_names = old.getServer_Names();
	_default = old.getDefault();
	_err_codes = old.getErr_codes();
	_body_size = old.getBody_size();
	_locations = old.getLocations();
}

Server&	Server::operator=(const Server& old)
{
	if (*this == old)
		return *this;

	_port = old.getPort();
	_host = old.getHost();
	_server_names = old.getServer_Names();
	_default = old.getDefault();
	_err_codes = old.getErr_codes();
	_body_size = old.getBody_size();
	_locations = old.getLocations();
	return *this;
}

int				Server::getPort()
{
	return	_port;
}

int				Server::getHost()
{
	return	_host;
}

std::string*	Server::getServers_names()
{
	return_	
}

bool			Server::getDefault()
{
	return	_default;
}

std::map<int, std::string>*	Server::getErr_codes()
{
	return	_err_codes;
}

int				Server::getBody_size()
{
	return	_body_size;
}

ServerLocation*	Server::getLocations()
{
	return	_locations;
}


void	setPort(int port)
{
	_port = port;
}

void	setHost(int host)
{
	_host = host;
}

void	setServers_names(std::string* servers_names)
{
	delete[] _servers_names;
	_servers_names = servers_names;
}

void	setDefault(bool dfl)
{
	_default = dfl;
}

void	setErr_codes(std::map<int, std::string>* err_codes)
{
	delete[] _err_codes;
	_err_codes = err_codes;
}

void	setBody_size(int body_size)
{
	_body_size = body_size;
}

void	setLocations(ServerLocations* locations)
{
	delete[] _locations;
	_locations = locations;
}
