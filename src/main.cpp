/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 16:44:05 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/09 13:25:37 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "webserv.hpp"

static void	sig_handler(int sign)
{
	(void)sign;
	throw InternalError();
}

static void	init_signal(void)
{
	if (signal(SIGINT, sig_handler) == SIG_ERR)
		throw InternalError();
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage ./webserv [configFile]" << std::endl;
		return (0);
	}
	

	try
	{

		std::string filename(argv[1]);
		ConfigurationManager config(filename);

		config.printConfigData();
		std::cout << std::endl;

		init_signal();

		TcpServer test;
		test.create(4245);
		//test.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << "\033[41m" << e.what() << "\033[0m" << std::endl;;
	}
	return (0);
}
