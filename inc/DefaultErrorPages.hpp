/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultErrorPages.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 14:59:28 by hboissel          #+#    #+#             */
/*   Updated: 2023/10/01 18:49:32 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef DEFAULTERRORPAGES_HPP
# define DEFAULTERRORPAGES_HPP

# include <string>

class DefaultErrorPages
{
	public:
		static const std::string	generate(unsigned int code, std::string details);
	private:
		static std::string	genByCode(std::string name, std::string details);
};

#endif
