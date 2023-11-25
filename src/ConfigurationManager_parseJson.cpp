/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationManager_parseJson.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 09:40:35 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/25 09:41:17 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ConfigurationManager.hpp"

std::string ConfigurationManager::removeSpacesAndNewlines(const std::string& str) {
	std::string result;
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (!std::isspace(*it)) {
			result += *it;
		}
	}
	return result;
}

void ConfigurationManager::parseJsonValue(const std::string& content,
		size_t& pos, JsonValue& jsonValue) {
	pos = content.find_first_not_of(" \t\n\r", pos);

	if (content[pos] == '{') {
		std::map<std::string, JsonValue> object;
		parseJsonObject(content, pos, object);
		jsonValue.setObject(object);
	} else if (content[pos] == '[') {
		std::vector<JsonValue> array;
		parseJsonArray(content, pos, array);
		jsonValue.setArray(array);
	} else if (content[pos] == '"') {
		std::string value = parseJsonString(content, pos);
		jsonValue.setString(value);
	} else if (std::isdigit(content[pos]) || content[pos] == '-') {
		std::string value = parseJsonNumber(content, pos);
		jsonValue.setString(value);
	} else if (content.compare(pos, 4, "true") == 0
			|| content.compare(pos, 5, "false") == 0) {
		std::string value = content.substr(pos,
				content.compare(pos, 4, "true") == 0 ? 4 : 5);
		jsonValue.setString(value);
		pos += value.length();
	} else if (content.compare(pos, 4, "null") == 0) {
		std::string value = content.substr(pos, 4);
		jsonValue.setString(value);
		pos += value.length();
	} else {
		std::string value = parseJsonString(content, pos);
		jsonValue.setString(value);
	}
}

std::string ConfigurationManager::parseJsonNumber(
		const std::string& content, size_t& pos) {
	size_t endPos = pos;

	while (endPos < content.length() && std::isdigit(content[endPos])) 
    {
		endPos++;
	}

	std::string value = content.substr(pos, endPos - pos);
	pos = endPos;

	return value;
}

std::string ConfigurationManager::parseJsonString(
		const std::string& content, size_t& pos) {
	pos++;

	size_t endPos = content.find('"', pos);
	if (endPos == std::string::npos) {
		throw ConfigurationManager::ErrorJsonString();
	}

	std::string value = content.substr(pos, endPos - pos);

	pos = endPos + 1;

	return value;
}

void ConfigurationManager::parseJsonObject(const std::string& content,
    size_t& pos, std::map<std::string, JsonValue>& object)
{
	pos++;

	while (content[pos] != '}') {
		std::string key = parseJsonString(content, pos);
		if (content.size() <= pos)
			break ;
		pos = content.find_first_not_of(" \t\n\r", pos);
		if (content[pos] != ':') {
			throw ConfigurationManager::ErrorJsonColon();
		}
		pos++;
		JsonValue value;
		parseJsonValue(content, pos, value);
		if (content.size() <= pos)
			break ;
		object[key] = value;
		pos = content.find_first_not_of(" \t\n\r", pos);
		if (content[pos] == ',') {
			pos++;
		}
	}
	if (content.size() <= pos) {
		throw ConfigurationManager::ErrorJsonObject();
    }
	pos++;
}

void ConfigurationManager::parseJsonArray(const std::string& content,
    size_t& pos, std::vector<JsonValue>& array) {
	pos++;

	while (content[pos] != ']') {
		JsonValue element;
		parseJsonValue(content, pos, element);
		array.push_back(element);
		pos = content.find_first_not_of(" \t\n\r", pos);
		if (content[pos] == ',') {
			pos++;
		}
	}
	pos++;
}
