/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 12:00:41 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/21 17:41:37 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <cerrno>
# include <iostream>
# include <csignal>
//# include "TcpServer.hpp"

int	tcp_server(int port);

#endif
