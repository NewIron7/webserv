/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:54:23 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/17 12:44:25 by hboissel         ###   ########.fr       */
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

class Request
{
	public:
		Request(void);
		Request(std::string r);
		~Request(void);

		Request &operator=(const Request &rhs);

		void		printAttributes(void) const;

		const std::string& getMethod() const;
		const std::string& getTarget() const;
		const std::string& getQuery() const;
		const std::string& getVersion() const;
		const std::string& getHost() const;
		unsigned int getPort() const;
		std::map<std::string, std::string>& getHeaders();
		std::string	getHeader(const std::string &name);
		const std::string& getBody() const;
		unsigned int getErrorCode() const;
		const std::string& getErrorMsg() const;

		void	setErrorCode(const unsigned int err);
		void	setErrorMsg(const std::string &txt);

		void	setCodeMsg(const unsigned int err, const std::string &txt);
	
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
		std::string		_errorMsg;

		void	_getRequestLine(std::string &r);
		void	_getElemRequestLine(std::string requestLine);
		void    _checkWhitespaceElemRequestLine(void);
		void	_getHeaders(std::string &r);
		void    _checkContentLength(std::string &r);
		void	_checkHost(void);
		void	_checkBodyLength(void);

		void	_processChunkedBody(std::string &body);
};

#endif
