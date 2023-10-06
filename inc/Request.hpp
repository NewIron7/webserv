/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:54:23 by hboissel          #+#    #+#             */
/*   Updated: 2023/10/06 19:31:34 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <map>
# include <iostream>
# include <algorithm>
# include <sstream>

class Request
{
	public:
		Request(void);
		Request(std::string r);
		~Request(void);

		Request &operator=(const Request &rhs);

		void	printAttributes(void) const;
	private:
		std::string	_method;
		std::string _target;
		std::string	_pVersion;
		std::string _host;
		unsigned int	_port;
		std::map<std::string, std::string>	_headers;
		std::string	_body;

		unsigned int	_errorCode;

		void	_getRequestLine(std::string &r);
		void	_getElemRequestLine(std::string requestLine);
		void    _checkWhitespaceElemRequestLine(void);
		void	_getHeaders(std::string &r);
		void    _checkContentLength(const std::string &r);
		void	_checkHost(void);
		void	_checkBodyLength(void);
};

#endif
