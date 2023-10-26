#include "type_verif.hpp"

static bool	verifTypeStrings(std::string str)
{
	(void) str;
	return true;
}

static bool	verifTypeIP(std::string str)
{
	(void) str;
	return true;
}

static bool	verifTypeBool(std::string str)
{
	(void) str;
	return true;
}

static bool	verifTypePath(std::string str)
{
	(void) str;
	return true;
}

static bool	verifTypeURL(std::string str)
{
	(void) str;
	return true;
}

static bool	verifTypeExtension(std::string str)
{
	(void) str;
	return true;
}

static bool	verifTypeNumber(std::string str)
{
	(void) str;
	return true;
}

bool	verifTypeMethods(std::string str)
{
	(void) str;
	return true;
}

bool	verifSingleType(Type type, std::string str)
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

bool	verifDoubleType(Type ltype, Type rtype, std::string str)
{
	std::string::size_type	sep;

	sep = str.find_first_of('=');
	if (sep == std::string::npos || sep == str.length())
		return false;
	return verifSingleType(ltype, str.substr(0, sep)) && verifSingleType(rtype, str.substr(sep + 1));
}
