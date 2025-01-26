/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:33:57 by afont             #+#    #+#             */
/*   Updated: 2025/01/22 13:26:34 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/client.hpp"
#include "../include/server.hpp"
#include "../include/utils.hpp"

int	main(int ac, char **av)
{
	Server	server;

	parser(ac, av);
	server.setPort(std::atoi(av[1]));
	try
	{
		signal(SIGINT, Server::signalHandler);
		signal(SIGQUIT, Server::signalHandler);
		server.initServer();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}
