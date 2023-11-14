/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationObject.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:21:04 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/14 06:44:34 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CONFIGURATIONOBJECT_HPP
# define CONFIGURATIONOBJECT_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>

# include "Route.hpp"

class ConfigurationObject
{
	public:
		ConfigurationObject(void);
		~ConfigurationObject(void);

		std::string							host;
		unsigned int						port;
		std::vector<std::string>			server_names;
		std::map<unsigned int, std::string>	defaultErrorPages;
		bool								isBodySize;
		unsigned long int					bodySize;

		std::map<std::string, Route>		routes;

		void printParameters() const;
	private:
};

#endif
