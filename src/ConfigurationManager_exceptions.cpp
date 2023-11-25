/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationManager_exceptions.cpp                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 09:41:38 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/25 09:41:43 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ConfigurationManager.hpp"

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
