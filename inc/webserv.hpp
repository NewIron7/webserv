/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 12:00:41 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/19 15:57:56 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <errno.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <poll.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <netinet/in.h>
# include <iostream>
# include <vector>
# include <algorithm>

int	tcp_server(int port);

#endif
