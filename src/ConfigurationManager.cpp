/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationManager.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:19:39 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/15 10:24:43 by hboissel         ###   ########.fr       */
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

ConfigurationManager::ConfigurationManager(const std::string &filename) {
	if (!loadConfigFile(filename)) {
		// Handle error, maybe throw an exception or set default values
		std::cerr << "Error with configuration file: " << filename << std::endl;
	}
	this->checkJson();
	//this->printConfig();
}

std::vector<ConfigurationObject>	&ConfigurationManager::getServersHostPort(std::string &host, unsigned int port) 
{
	std::string	key = host;
	key += intToString(port);
	return (this->config[key]);
}

void ConfigurationManager::printConfig(void) const {

	for (std::map<std::string, std::vector<ConfigurationObject> >::const_iterator it = this->config.begin(); it != this->config.end(); ++it) {
		std::cout << "	Key: " << it->first << std::endl;

		const std::vector<ConfigurationObject>& vec = it->second;
		for (size_t i = 0; i < vec.size(); ++i) {
			std::cout << "	ConfigurationObject " << i + 1 << ":" << std::endl;
			vec[i].printParameters(); // Utilize the printParameters method
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}
}


void ConfigurationManager::printConfigData() {
	this->configData.printJsonValue();
}
bool ConfigurationManager::loadConfigFile(const std::string& filename) {
	std::ifstream configFile(filename.c_str());
	if (!configFile.is_open()) {
		// Handle file not found or other errors
		std::cerr << "Error opening configuration file: " << filename << std::endl;
		return false;
	}

	// Read the file content into a string
	std::ostringstream buffer;
	buffer << configFile.rdbuf();
	configFile.close();

	if (!parseConfig(buffer.str(), this->configData)) {
		std::cerr << "Error parsing configuration file: " << filename << std::endl;
		return false;
	}

	return true;
}

bool ConfigurationManager::parseConfig(const std::string &content, JsonValue& jsonValue) {
	if (content.size() == 0){
		std::cerr << "Error, empty configuration file" << std::endl;
		return false;
	}

	size_t pos = 0;

	// Remove spaces and newlines for simplicity
	std::string cleanContent = removeSpacesAndNewlines(content);

	//std::cout << "Before parsing: " << cleanContent << std::endl;

	this->parseJsonValue(cleanContent, pos, jsonValue);
	//std::cout << "Parsing json done: " << std::endl;
	//this->printConfigData();
	//std::cout << std::endl;

	return (true);
}

std::string ConfigurationManager::removeSpacesAndNewlines(const std::string& str) {
	std::string result;
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (!std::isspace(*it)) {
			result += *it;
		}
	}
	return result;
}

void ConfigurationManager::parseJsonValue(const std::string& content, size_t& pos, JsonValue& jsonValue) {
	// Skip leading whitespaces
	pos = content.find_first_not_of(" \t\n\r", pos);

	// Determine the type of JSON value
	if (content[pos] == '{') {
		// Parse object
		std::map<std::string, JsonValue> object;
		parseJsonObject(content, pos, object);
		jsonValue.setObject(object);
	} else if (content[pos] == '[') {
		// Parse array
		std::vector<JsonValue> array;
		parseJsonArray(content, pos, array);
		jsonValue.setArray(array);
	} else if (content[pos] == '"') {
		// Parse string
		std::string value = parseJsonString(content, pos);
		jsonValue.setString(value);
	} else if (std::isdigit(content[pos]) || content[pos] == '-') {
		// Parse number
		std::string value = parseJsonNumber(content, pos);
		jsonValue.setString(value);  // Store numbers as strings for simplicity
	} else if (content.compare(pos, 4, "true") == 0 || content.compare(pos, 5, "false") == 0) {
		// Parse boolean
		std::string value = content.substr(pos, content.compare(pos, 4, "true") == 0 ? 4 : 5);
		jsonValue.setString(value);
		pos += value.length();  // Move past the boolean
	} else if (content.compare(pos, 4, "null") == 0) {
		// Parse null
		std::string value = content.substr(pos, 4);
		jsonValue.setString(value);
		pos += value.length();  // Move past null
	} else {
		// Handle other types as strings
		std::string value = parseJsonString(content, pos);
		jsonValue.setString(value);
	}
}

std::string ConfigurationManager::parseJsonNumber(const std::string& content, size_t& pos) {
	size_t endPos = pos;

	while (endPos < content.length() && (std::isdigit(content[endPos]) || content[endPos] == '.' || content[endPos] == 'e' || content[endPos] == 'E' || content[endPos] == '+' || content[endPos] == '-')) {
		endPos++;
	}

	std::string value = content.substr(pos, endPos - pos);
	pos = endPos;

	return value;
}

std::string ConfigurationManager::parseJsonString(const std::string& content, size_t& pos) {
	pos++;  // Skip the opening double quote '"'

	size_t endPos = content.find('"', pos);
	if (endPos == std::string::npos) {
		throw ConfigurationManager::ErrorJsonString();
	}

	std::string value = content.substr(pos, endPos - pos);

	pos = endPos + 1;  // Move past the closing double quote '"'

	return value;
}

void ConfigurationManager::parseJsonObject(const std::string& content, size_t& pos, std::map<std::string, JsonValue>& object) {
	pos++;  // Skip the opening brace '{'

	while (content[pos] != '}') {
		// Parse key
		std::string key = parseJsonString(content, pos);

		if (content.size() <= pos)
			break ;

		// Skip colon
		pos = content.find_first_not_of(" \t\n\r", pos);
		if (content[pos] != ':') {
			throw ConfigurationManager::ErrorJsonColon();
		}
		pos++;  // Move past the colon

		// Parse value
		JsonValue value;
		parseJsonValue(content, pos, value);
		if (content.size() <= pos)
			break ;

		// Add key-value pair to the object
		object[key] = value;

		// Skip comma if present
		pos = content.find_first_not_of(" \t\n\r", pos);
		if (content[pos] == ',') {
			pos++;  // Move past the comma
		}
	}
	if (content.size() <= pos) {
		throw ConfigurationManager::ErrorJsonObject();
	}

	pos++;  // Skip the closing brace '}'
}

void ConfigurationManager::parseJsonArray(const std::string& content, size_t& pos, std::vector<JsonValue>& array) {
	pos++;  // Skip the opening bracket '['

	while (content[pos] != ']') {

		// Parse array element
		JsonValue element;
		parseJsonValue(content, pos, element);

		// Add element to the array
		array.push_back(element);

		// Skip comma if present
		pos = content.find_first_not_of(" \t\n\r", pos);
		if (content[pos] == ',') {
			pos++;  // Move past the comma
		}
	}

	pos++;  // Skip the closing bracket ']'
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
			routeRef.location = it->second.getString();
		}
		else if (it->first == "directoryListing")
		{
			if (it->second.getString() != "true" || it->second.getString() != "false")
				throw ConfigurationManager::ErrorUserConfig();
			else if (it->second.getString() == "true")
				routeRef.directoryListing = true;
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
			routeRef.uploadPath = it->second.getString();
		}
		else
			throw ConfigurationManager::ErrorUserConfig();
	}
	else if (it->second.isArray() == true)
	{
		if (it->first == "methods")
		{
			routeRef.methods = this->_getContentStringArrayJson(it->second);
		}
		else
			throw ConfigurationManager::ErrorUserConfig();
	}
	else
		throw ConfigurationManager::ErrorUserConfig();
}

void	ConfigurationManager::_getRoute(std::map<std::string, JsonValue>::const_iterator &it,
		ConfigurationObject &configTmp)
{
	if (it->first.find_first_of('/') != 0)
	{
		std::cout << it->first << ": route must begin by a '/'" << std::endl;
		throw ConfigurationManager::ErrorUserConfig();
	}
	else if (it->first != "/" && it->first[it->first.size() - 1] == '/')
	{
		std::cout << it->first << ": route must not be terminated by a '/'" << std::endl;
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
		throw ConfigurationManager::ErrorUserConfig();
	if (routeRef.location.empty())
	{
		routeRef.location = "." + it->first;
	}		
}

std::vector<std::string>	ConfigurationManager::_getContentStringArrayJson(const JsonValue &json)
{
	std::vector<JsonValue>	arrayJson = json.getArray();
	std::vector<std::string>	arrayTmp;

	for (std::vector<JsonValue>::const_iterator it = arrayJson.begin();
			it != arrayJson.end(); ++it)
	{
		if (it->isString() == false)
			throw ConfigurationManager::ErrorUserConfig();
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

void	ConfigurationManager::_checkGetContentServer(
		std::map<std::string, JsonValue>::const_iterator &it, ConfigurationObject &configTmp)
{
	//std::cout << "	" << it->first << std::endl;
	if (it->first.empty())
	{
		std::cout << "Empty field /!\\" << std::endl;
		throw ConfigurationManager::ErrorUserConfig();
	}
	else if (it->first == "host")
	{
		if (it->second.isString() == false)
			throw ConfigurationManager::ErrorUserConfig();
		configTmp.host = it->second.getString();
	}
	else if (it->first == "port")
	{
		if (it->second.isString() == false)
			throw ConfigurationManager::ErrorUserConfig();
		std::string portTmp = it->second.getString();
		if (isStringOnlyDigits(portTmp) == false)
			throw ConfigurationManager::ErrorUserConfig();
		configTmp.port = stringToInt(portTmp);
	}
	else if (it->first == "server_names")
	{
		if (it->second.isArray() == false)
			throw ConfigurationManager::ErrorUserConfig();
		configTmp.server_names = this->_getContentStringArrayJson(it->second);
	}
	else if (it->first == "defaultErrorPages")
	{
		if (it->second.isObject() == false)
			throw ConfigurationManager::ErrorUserConfig();
		configTmp.defaultErrorPages = this->_getContentStringObjectJson(it->second);
	}
	else if (it->first == "bodySize")
	{
		if (it->second.isString() == false || isStringOnlyDigits(it->second.getString()) == false)
			throw ConfigurationManager::ErrorUserConfig();
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


void	ConfigurationManager::_checkServerJson(std::map<std::string, JsonValue> serverContentMap)
{
	if (serverContentMap.find("host") == serverContentMap.end()
			|| serverContentMap.find("port") == serverContentMap.end())
		throw ConfigurationManager::ErrorUserConfig();

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

void	ConfigurationManager::_checkEachServerJson(std::map<std::string, JsonValue> &serversMap)
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
		throw ConfigurationManager::ErrorUserConfig();
	std::map<std::string, JsonValue>	serversMap = this->configData.getObject();
	if (serversMap.empty() == true)
		throw ConfigurationManager::ErrorUserConfig();
	this->_checkEachServerJson(serversMap);
}

const char* ConfigurationManager::ErrorJsonColon::what() const throw()
{
	return ("Json error: colon expected");
}

const char* ConfigurationManager::ErrorJsonString::what() const throw()
{
	return ("Json error: Unterminated string");
}

const char* ConfigurationManager::ErrorJsonObject::what() const throw()
{
	return ("Json error: Unterminated object");
}

const char* ConfigurationManager::ErrorUserConfig::what() const throw()
{
	return ("Config file error");
}


