/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationManager_get.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 09:39:23 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/25 09:40:10 by hboissel         ###   ########.fr       */
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

void	ConfigurationManager::_getContentRoute(Route &routeRef,
		std::map<std::string, JsonValue>::const_iterator &it)
{
	if (it->second.isString() == true)
	{
		if (it->first == "redirection")
		{
			routeRef.redirection = it->second.getString();
		}
		else if (it->first == "location")
		{
			if (it->second.getString().find_first_of('/') != 0 
					&& it->second.getString().find("./") != 0)
			{
				std::cerr << it->second.getString()
					<< ": location must begin by a '/' or './'" << std::endl;
				throw ConfigurationManager::ErrorUserConfig();
			}
			else if (it->second.getString() != "/" && it->second.getString() != "./"
					&& it->second.getString()[it->second.getString().size() - 1] == '/')
			{
				std::cerr << it->second.getString()
					<< ": location must not be terminated by a '/'" << std::endl;
				throw ConfigurationManager::ErrorUserConfig();
			}
			if (it->second.getString() == "/")
				routeRef.location = "";
			else if (it->second.getString() == "./")
				routeRef.location = ".";
			else
				routeRef.location = it->second.getString() + "/";
		}
		else if (it->first == "directoryListing")
		{
			if (it->second.getString() != "true" && it->second.getString() != "false")
				throw ConfigurationManager::ErrorUserConfig();
			else if (it->second.getString() == "true")
				routeRef.directoryListing = true;
		}
		else if (it->first == "directoryAdding")
		{
			if (it->second.getString() != "true" && it->second.getString() != "false")
				throw ConfigurationManager::ErrorUserConfig();
			else if (it->second.getString() == "true")
				routeRef.directoryAdding = true;
		}
		else if (it->first == "root")
		{
			routeRef.root = it->second.getString();
		}
		else if (it->first == "cgiPath")
		{
			routeRef.cgiPath = it->second.getString();
		}
		else if (it->first == "cgiExtension")
		{
			routeRef.cgiExtension = it->second.getString();
		}
		else if (it->first == "uploadPath")
		{
			routeRef.uploadedFile = true;

			if (it->second.getString().find_first_of('/') != 0 
					&& it->second.getString().find("./") != 0)
			{
				std::cerr << it->second.getString()
					<< ": uploadPath must begin by a '/' or './'" << std::endl;
				throw ConfigurationManager::ErrorUserConfig();
			}
			else if (it->second.getString() != "/" && it->second.getString() != "./"
					&& it->second.getString()[it->second.getString().size() - 1] == '/')
			{
				std::cerr << it->second.getString()
					<< ": uploadPath must not be terminated by a '/'" << std::endl;
				throw ConfigurationManager::ErrorUserConfig();
			}
			if (it->second.getString() == "/")
				routeRef.uploadPath = "";
			else if (it->second.getString() == "./")
				routeRef.uploadPath = ".";
			else
				routeRef.uploadPath = it->second.getString() + "/";
		}
		else
		{
			std::cerr << it->first << " bad key" << std::endl;
			throw ConfigurationManager::ErrorUserConfig();
		}
			
	}
	else if (it->second.isArray() == true)
	{
		if (it->first == "methods")
		{
			routeRef.methods = this->_getContentStringArrayJson(it->second);
		}
		else
		{
			std::cerr << it->first << " cannot be an Array" << std::endl;
			throw ConfigurationManager::ErrorUserConfig();
		}
	}
	else
	{
		std::cerr << it->first << ": object are not allowed here" << std::endl;
		throw ConfigurationManager::ErrorUserConfig();
	}
}

void	ConfigurationManager::_getRoute(
		std::map<std::string, JsonValue>::const_iterator &it,
		ConfigurationObject &configTmp)
{
	if (it->first.find_first_of('/') != 0)
	{
		std::cerr << it->first << ": route must begin by a '/'" << std::endl;
		throw ConfigurationManager::ErrorUserConfig();
	}
	else if (it->first != "/" && it->first[it->first.size() - 1] == '/')
	{
		std::cerr << it->first << ": route must not be terminated by a '/'" << std::endl;
		throw ConfigurationManager::ErrorUserConfig();
	}

	Route &routeRef = configTmp.routes[it->first];
	std::map<std::string, JsonValue>	mapJson = it->second.getObject();

	for (std::map<std::string, JsonValue>::const_iterator it = mapJson.begin();
			it != mapJson.end(); ++it)
	{
		this->_getContentRoute(routeRef, it);
	}
	if (routeRef.cgiPath.empty() != routeRef.cgiExtension.empty())
	{
		std::cerr << "The both cgiPath and cgiExtension have to be instanciated" << std::endl;
		throw ConfigurationManager::ErrorUserConfig();
	}
	if (routeRef.location.empty())
	{
		routeRef.location = "." + it->first;
	}		
}

std::vector<std::string>	ConfigurationManager::_getContentStringArrayJson(
		const JsonValue &json)
{
	std::vector<JsonValue>	arrayJson = json.getArray();
	std::vector<std::string>	arrayTmp;

	for (std::vector<JsonValue>::const_iterator it = arrayJson.begin();
			it != arrayJson.end(); ++it)
	{
		if (it->isString() == false)
		{
			std::cerr << "You can only have string in array" << std::endl;
			throw ConfigurationManager::ErrorUserConfig();
		}
		arrayTmp.push_back(it->getString());
	}
	return (arrayTmp);
}

std::map<unsigned int, std::string>	ConfigurationManager::_getContentStringObjectJson(
		const JsonValue	&json)
{
	std::map<std::string, JsonValue>	mapJson = json.getObject();
	std::map<unsigned int, std::string>	mapTmp;

	for (std::map<std::string, JsonValue>::const_iterator it = mapJson.begin();
			it != mapJson.end(); ++it)
	{
		if (isStringOnlyDigits(it->first) == false || it->second.isString() == false)
		{
			std::cerr << "You have to set cgiPath and cgiExtension" << std::endl;
			throw ConfigurationManager::ErrorUserConfig();
		}
		mapTmp[stringToInt(it->first)] = it->second.getString();
	}
	return (mapTmp);
}
