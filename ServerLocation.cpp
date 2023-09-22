#include "ServerLocation.hpp"

ServerLocation()
{
	_limit_except = POST | GET | DELETE;
	_proxy_pass = "";
	_alias = "";
	_autoindex = false;
	_index = "";
	_cgi = "";
	_location = "";
	_upload_store = "";
}

~ServerLocation() {}

ServerLocation(const ServerLocation& old)
{
	_limit_except = old.getLimit_except();
	_proxy_pass = old.getProxy_pass();
	_alias = old.getAlias();
	_autoindex = old.getAutoindex();
	_index = old.getIndex();
	_cgi = old.getCgi();
	_location = old.getLocation();
	_upload_store = old.getUpload_store();
}

ServerLocation& operator=(const ServerLocation& old)
{
	if (*this == old)
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

int				ServerLocation::getLimit_except()
{
	return _limit_except;
}

std::string		ServerLocation::getProxy_pass()
{
	return _proxy_pass;
}

std::string		ServerLocation::getAlias()
{
	return _alias;
}

bool			ServerLocation::getAutoindex()
{
	return _autoindex;
}

std::string		ServerLocation::getIndex()
{
	return _index;
}

std::string		ServerLocation::getCgi()
{
	return _cgi;
}

std::string		ServerLocation::getLocation()
{
	return _location;
}

std::string		ServerLocation::getUpload_store()
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

void	ServerLocation::setCgi(std::string cgi)
{
	_cgi = cgi;
}

void	ServerLocation::setLocation(std::string location)
{
	_location = location;
}

void	ServerLocation::setUpload_store(std::string upload_store)
{
	_upload_store = upload_store;
}
