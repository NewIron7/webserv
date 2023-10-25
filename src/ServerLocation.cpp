#include "ServerLocation.hpp"

ServerLocation::ServerLocation()
{
	_location = "";
	_limit_except = POST | GET;
	_proxy_pass = "";
	_alias = "";
	_autoindex = false;
	_index = "";
}

ServerLocation::~ServerLocation() {}

ServerLocation::ServerLocation(const ServerLocation& old)
{
	_location = old.getLocation();
	_limit_except = old.getLimit_except();
	_proxy_pass = old.getProxy_pass();
	_alias = old.getAlias();
	_autoindex = old.getAutoindex();
	_index = old.getIndex();
	_cgi = old.getCgi();
	_upload_store = old.getUpload_store();
}

ServerLocation& ServerLocation::operator=(const ServerLocation& old)
{
	if (this == &old)
		return *this;

	_limit_except = old.getLimit_except();
	_proxy_pass = old.getProxy_pass();
	_alias = old.getAlias();
	_autoindex = old.getAutoindex();
	_index = old.getIndex();
	_cgi = old.getCgi();
	_location = old.getLocation();
	_upload_store = old.getUpload_store();
	return *this;
}

int				ServerLocation::getLimit_except() const
{
	return _limit_except;
}

std::string		ServerLocation::getProxy_pass() const
{
	return _proxy_pass;
}

std::string		ServerLocation::getAlias() const
{
	return _alias;
}

bool			ServerLocation::getAutoindex() const
{
	return _autoindex;
}

std::string		ServerLocation::getIndex() const
{
	return _index;
}

std::map<std::string, std::string>	ServerLocation::getCgi() const
{
	return _cgi;
}

std::string		ServerLocation::getLocation() const
{
	return _location;
}

std::map<std::string, std::string>	ServerLocation::getUpload_store() const
{
	return _upload_store;
}

void	ServerLocation::setLimit_except(int limit_except)
{
	_limit_except = limit_except;
}

void	ServerLocation::setProxy_pass(std::string proxy_pass)
{
	_proxy_pass = proxy_pass;
}

void	ServerLocation::setAlias(std::string alias)
{
	_alias = alias;
}

void	ServerLocation::setAutoindex(bool autoindex)
{
	_autoindex = autoindex;
}

void	ServerLocation::setIndex(std::string index)
{
	_index = index;
}

void	ServerLocation::addCgi(std::string ext, std::string exe)
{
	if (_cgi.count(ext))
		throw std::exception();
	_cgi[ext] = exe;
}

void	ServerLocation::setLocation(std::string location)
{
	_location = location;
}

void	ServerLocation::addUpload_store(std::string ext, std::string dir)
{
	if (_upload_store.count(ext))
		throw std::exception();
	_upload_store[ext] = dir;
}
