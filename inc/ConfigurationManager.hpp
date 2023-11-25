/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationManager.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:19:55 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/25 08:59:40 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATIONMANAGER_HPP
# define CONFIGURATIONMANAGER_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <map>
# include <algorithm>
# include <vector>
# include <sys/stat.h>

# include "JsonValue.hpp"
# include "ConfigurationObject.hpp"


class ConfigurationManager {
	public:
		ConfigurationManager(const std::string& filename); 

		void	printConfigData(void);
		void	printConfig(void) const;
		bool	loadConfigFile(const std::string& filename);
		bool	parseConfig(const std::string &content, JsonValue& jsonValue);
		std::vector<ConfigurationObject>	&getServersHostPort(
				std::string &host, unsigned int port);

		std::map<std::string, std::vector<ConfigurationObject> >	config;	

	private:
		JsonValue configData;

		std::string	removeSpacesAndNewlines(const std::string& str);

		void		parseJsonValue(const std::string& content, size_t& pos,
			JsonValue& jsonValue);
		std::string parseJsonNumber(const std::string& content, size_t& pos);
		std::string parseJsonString(const std::string& content, size_t& pos);
		void		parseJsonObject(const std::string& content,
			size_t& pos, std::map<std::string, JsonValue>& object);
		void		parseJsonArray(const std::string& content,
			size_t& pos, std::vector<JsonValue>& array);
		void		checkJson(void);

		void		_checkEachServerJson(std::map<std::string, JsonValue> &serversMap);
		void		_checkServerJson(std::map<std::string, JsonValue> serverContentMap);
		void		_checkGetContentServer(std::map<std::string,
				JsonValue>::const_iterator &it, ConfigurationObject &configTmp);
		
		std::vector<std::string>	_getContentStringArrayJson(const JsonValue &json);
		std::map<unsigned int, std::string>	_getContentStringObjectJson(
				const JsonValue &json);
		void	_getRoute(std::map<std::string, JsonValue>::const_iterator &it,
				ConfigurationObject &configTmp);
		void	_getContentRoute(Route &routeRef, std::map<std::string,
				JsonValue>::const_iterator &it);
	

		class ErrorJsonColon : public std::exception
		{
				public:
							virtual const char* what() const throw();
		};

		class ErrorJsonString : public std::exception
		{
				public:
							virtual const char* what() const throw();
		};

		class ErrorJsonObject : public std::exception
		{
				public:
							virtual const char* what() const throw();
		};

		class ErrorUserConfig : public std::exception
		{
				public:
							virtual const char* what() const throw();
		};
};

#endif
