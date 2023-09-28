/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 16:44:05 by hboissel          #+#    #+#             */
/*   Updated: 2023/09/28 13:25:16 by hboissel         ###   ########.fr       */
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

int	main(void)
{
	try
	{
		init_signal();

		TcpServer test;
		test.create(4245);
		test.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << "\033[31m" << e.what() << "\033[0m" << std::endl;;
	}
	return (0);
}
