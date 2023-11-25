/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationManager_print.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 09:39:07 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/25 09:39:12 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigurationManager.hpp"

void ConfigurationManager::printConfig(void) const {

	for (std::map<std::string, std::vector<ConfigurationObject> >::const_iterator
        it = this->config.begin(); it != this->config.end(); ++it) {
		std::cerr << "	Key: " << it->first << std::endl;

		const std::vector<ConfigurationObject>& vec = it->second;
		for (size_t i = 0; i < vec.size(); ++i) {
			std::cerr << "	ConfigurationObject " << i + 1 << ":" << std::endl;
			vec[i].printParameters(); // Utilize the printParameters method
			std::cerr << std::endl;
		}

		std::cerr << std::endl;
	}
}

void ConfigurationManager::printConfigData() {
	this->configData.printJsonValue();
}
