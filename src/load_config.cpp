#include "Server.hpp"
#include <fstream>

bool	verifTypeStrings(std::string str)
{
}

bool	verifTypeIP(std::string str)
{
}

bool	verifTypeBool(std::string str)
{
}

bool	verifTypePath(std::string str)
{
}

bool	verifTypeURL(std::string str)
{
}

bool	verifTypeExtension(std::string str)
{
}

bool	verifTypeNumber(std::string str)
{
}

bool	verifTypeMethods(std::string str)
{
}

bool	verifSingleType(int type, std::string str)
{
	switch (type)
	{
		case TYPE_STRINGS:
			return verifTypeStrings(str);
			break;
		case TYPE_NUMBER:
			return verifTypeNumber(str);
			break;
		case TYPE_IP:
			return verifTypeIP(str);
			break;
		case TYPE_BOOL:
			return verifTypeBool(str);
			break;
		case TYPE_PATH:
			return verifTypePath(str);
			break;
		case TYPE_URL:
			return verifTypeURL(str);
			break;
		case TYPE_EXTENSION:
			return verifTypeExtension(str);
			break;
		case TYPE_METHODS:
			return verifTypeMethods(str);
			break;
		default:
			return false;
	}
}

bool	verifDoubleType(int ltype, int rtype, std::string str)
{
	std::string::size_type	sep;

	sep = str.find_first_of('=');
	if (sep == std::string::npos || sep == str.length())
		return false;
	return verifSingleType(ltype, str.substr(0, sep)) && verifSingleType(rtype, str.substr(sep + 1, str.length() - sep - 1);
}

std::vector	load_config(char *filename)
{
	std::ifstream	file;
	const std::string	options = {
		"server=",
		"port=",
		"ip=",
		"default=",
		"err_code.",
		"body_size=",
		"location=",
		"location.limit_except=",
		"location.proxy_pass=",
		"location.alias=",
		"location.autoindex=",
		"location.index=",
		"location.cgi.",
		"location.upload_store."
	}

	file.open(filename);
	if (!file.is_open())
		throw std::exception();
}
