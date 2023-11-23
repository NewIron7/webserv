/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultErrorPages.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 14:59:28 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/16 07:39:11 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef DEFAULTERRORPAGES_HPP
# define DEFAULTERRORPAGES_HPP

# include <sstream>

# include "ConfigurationObject.hpp"

#define SSTR( x ) static_cast< std::ostringstream & >( \
		( std::ostringstream() << std::dec << x ) ).str()

# include <string>
# include <map>
# include <fstream>

class DefaultErrorPages
{
	public:
		static const std::string	generate(unsigned int code,
			std::string details);
		static const std::string	generate(unsigned int code,
			std::string details, const ConfigurationObject &currentConfig);

		static const std::string &getContentType(const std::string &ext);

		static std::map<unsigned int, std::string>	statusMap;
		static std::map<std::string, std::string> contentTypeMap;

	private:
		static std::string	genByCode(std::string code,
			std::string name, std::string details);
		static std::string addHeader(std::string code,
			std::string &name, std::string &gen);

		static std::map<unsigned int, std::string> initStatusMap(void);
		static std::map<std::string, std::string> initContentTypeMap(void);
};

#endif
