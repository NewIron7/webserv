#ifndef TYPE_VERIF_HPP
# define TYPE_VERIF_HPP

enum Type
{
	TYPE_STRINGS,
	TYPE_NUMBER,
	TYPE_IP,
	TYPE_BOOL,
	TYPE_PATH,
	TYPE_URL,
	TYPE_EXTENSION,
	TYPE_METHODS
}

bool	verifSingleType(Type type, std::string str);
bool	verifDoubleType(Type ltype, Type rtype, std::string str);

#endif
