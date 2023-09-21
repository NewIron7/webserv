/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 16:44:05 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/21 17:30:38 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "webserv.hpp"

int	main(void)
{
	/*
	TcpServer test;
	
	try
	{
		test.create(4243);
		test.create(4244);
		test.create(4245);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
	*/
	tcp_server(4245);
}
