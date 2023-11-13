/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationObject.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:21:04 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/13 11:50:14 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CONFIGURATIONOBJECT_HPP
# define CONFIGURATIONOBJECT_HPP

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
		unsigned long long int				bodySize;

		std::map<std::string, Route>		routes;
	private:
}

#endif
