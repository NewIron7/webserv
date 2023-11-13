/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationManager.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:19:39 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/13 11:10:42 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigurationManager.hpp"

ConfigurationManager::ConfigurationManager(const std::string &filename) {
	if (!loadConfigFile(filename)) {
		// Handle error, maybe throw an exception or set default values
		std::cerr << "Error with configuration file: " << filename << std::endl;
	}
	this->checkJson();
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

    std::cout << "Before parsing: " << cleanContent << std::endl;

	this->parseJsonValue(cleanContent, pos, jsonValue);

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

void	ConfigurationManager::checkJson(void)
{
	if (this->configData.isObject() == false)
        throw ConfigurationManager::ErrorUserConfig();

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


