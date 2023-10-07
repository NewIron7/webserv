/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:54:23 by hboissel          #+#    #+#             */
/*   Updated: 2023/10/07 18:42:47 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef REQUEST_HPP
# define REQUEST_HPP

#define SSTR( x ) static_cast< std::ostringstream & >( \
		( std::ostringstream() << std::dec << x ) ).str()

# include <map>
# include <iostream>
# include <algorithm>
# include <sstream>

# include "DefaultErrorPages.hpp"

class Request
{
	public:
		Request(void);
		Request(std::string r);
		~Request(void);

		Request &operator=(const Request &rhs);

		void		printAttributes(void) const;
		std::string	process(void);
	private:
		std::string	_method;
		std::string _target;
		std::string _query;
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
