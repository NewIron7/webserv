/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:54:23 by hboissel          #+#    #+#             */
/*   Updated: 2023/10/06 05:28:02 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <map>
# include <iostream>
# include <algorithm>

class Request
{
	public:
		Request(std::string r);
		~Request(void);
	private:
		std::string	_method;
		std::string _target;
		std::string	_pVersion;
		std::map<std::string, std::string>	_headers;
		std::string	_body;

		unsigned int	_errorCode;

		void	_getRequestLine(std::string &r);
		void	_getElemRequestLine(std::string requestLine);
		void    _checkWhitespaceElemRequestLine(void);
		void	_getHeaders(std::string &r);
		void    _checkContentLength(void);
		void	_checkHost(void);
};

#endif
