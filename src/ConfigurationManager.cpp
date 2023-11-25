/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationManager.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:19:39 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/17 11:53:47 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigurationManager.hpp"

static std::string intToString(unsigned int number) {
	std::stringstream ss;
	ss << number;
	return ss.str();
}

static bool fileExists(const std::string& filename) {
	struct stat buffer;
	return (stat(filename.c_str(), &buffer) == 0);
}

ConfigurationManager::ConfigurationManager(const std::string &filename) {
	if (!loadConfigFile(filename)) {
		// Handle error, maybe throw an exception or set default values
		//std::cerr << "Error with configuration file: " << filename << std::endl;
		throw ConfigurationManager::ErrorUserConfig();
	}
	this->checkJson();
	//this->printConfig();
}

std::vector<ConfigurationObject>	&ConfigurationManager::getServersHostPort(
	std::string &host, unsigned int port) 
{
	std::string	key = host;
	key += intToString(port);
	return (this->config[key]);
}

bool ConfigurationManager::loadConfigFile(const std::string& filename) {
	if (!fileExists(filename)) {
		std::cerr << filename << ": doesnt exist" << std::endl;
		return false;
	}

	std::ifstream configFile(filename.c_str());
	if (!configFile.is_open()) {
		// other errors
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

bool ConfigurationManager::parseConfig(const std::string &content,
JsonValue& jsonValue) {
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
