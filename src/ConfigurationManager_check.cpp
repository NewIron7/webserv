/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationManager_check.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 09:38:08 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/25 09:39:01 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ConfigurationManager.hpp"

static bool isStringOnlyDigits(const std::string& str) {
	for (size_t i = 0; i < str.length(); ++i) {
		if (!isdigit(str[i])) {
			return false;
		}
	}
	return true;
}

static int stringToInt(const std::string& str) {
	std::stringstream ss(str);
	unsigned int result;
	ss >> result;
	return result;
}

static std::string intToString(unsigned int number) {
	std::stringstream ss;
	ss << number;
	return ss.str();
}

void	ConfigurationManager::_checkGetContentServer(
		std::map<std::string, JsonValue>::const_iterator &it,
		ConfigurationObject &configTmp)
{
	if (it->first.empty())
	{
		std::cerr << "Empty field /!\\" << std::endl;
		throw ConfigurationManager::ErrorUserConfig();
	}
	else if (it->first == "host")
	{
		if (it->second.isString() == false)
		{
			std::cerr << "host must be a string" << std::endl;
			throw ConfigurationManager::ErrorUserConfig();
		}
		configTmp.host = it->second.getString();
	}
	else if (it->first == "port")
	{
		if (it->second.isString() == false)
		{
			std::cerr << "port must be a number" << std::endl;
			throw ConfigurationManager::ErrorUserConfig();
		}
		std::string portTmp = it->second.getString();
		if (isStringOnlyDigits(portTmp) == false)
		{
			std::cerr << "port must be a number" << std::endl;
			throw ConfigurationManager::ErrorUserConfig();
		}
		configTmp.port = stringToInt(portTmp);
	}
	else if (it->first == "server_names")
	{
		if (it->second.isArray() == false)
		{
			std::cerr << "server_names must be an array" << std::endl;
			throw ConfigurationManager::ErrorUserConfig();
		}
		configTmp.server_names = this->_getContentStringArrayJson(it->second);
	}
	else if (it->first == "defaultErrorPages")
	{
		if (it->second.isObject() == false)
		{
			std::cerr << "defaultErrorPages must be an object" << std::endl;
			throw ConfigurationManager::ErrorUserConfig();
		}
			
		configTmp.defaultErrorPages = this->_getContentStringObjectJson(it->second);
	}
	else if (it->first == "bodySize")
	{
		if (it->second.isString() == false
				|| isStringOnlyDigits(it->second.getString()) == false)
		{
			std::cerr << "Body size must be a number" << std::endl;
			throw ConfigurationManager::ErrorUserConfig();
		}
		configTmp.isBodySize = true;
		configTmp.bodySize = stringToInt(it->second.getString());
	}
	else if (it->second.isObject() == true)
	{
		//std::cout << "[new route]" << std::endl;
		this->_getRoute(it, configTmp);
	}
	else
	{
		std::cerr << "Error bad key" << std::endl;
		throw ConfigurationManager::ErrorUserConfig();
	}
}


void	ConfigurationManager::_checkServerJson(std::map<std::string,
		JsonValue> serverContentMap)
{
	if (serverContentMap.find("host") == serverContentMap.end()
			|| serverContentMap.find("port") == serverContentMap.end())
	{
		std::cerr << "Both host and port have to be instanciated for each server"
			<< std::endl;
		throw ConfigurationManager::ErrorUserConfig();
	}
		
	ConfigurationObject	configTmp;
	for (std::map<std::string, JsonValue>::const_iterator it = serverContentMap.begin();
			it != serverContentMap.end(); ++it)
	{
		this->_checkGetContentServer(it, configTmp);
	}

	std::string key = configTmp.host + ":";
	key += intToString(configTmp.port);
	this->config[key].push_back(configTmp);
}

void	ConfigurationManager::_checkEachServerJson(std::map<std::string,
		JsonValue> &serversMap)
{
	for (std::map<std::string, JsonValue>::const_iterator it = serversMap.begin();
			it != serversMap.end(); ++it)
	{
		//std::cout << "-> Checking server: " << it->first << std::endl;
		this->_checkServerJson(it->second.getObject());
	}
}

void	ConfigurationManager::checkJson(void)
{
	//std::cout << "[] check Json" << std::endl;
	if (this->configData.isObject() == false)
	{
		std::cerr << "Your config has to be in a object {}" << std::endl;
		throw ConfigurationManager::ErrorUserConfig();
	}
		
	std::map<std::string, JsonValue>	serversMap = this->configData.getObject();
	if (serversMap.empty() == true)
	{
		std::cerr << "There no server is your config" << std::endl;
		throw ConfigurationManager::ErrorUserConfig();
	}
	this->_checkEachServerJson(serversMap);
}
