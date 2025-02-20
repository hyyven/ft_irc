/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dferjul <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:33:57 by afont             #+#    #+#             */
/*   Updated: 2025/02/20 03:41:05 by dferjul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/all.hpp"

int	main(int ac, char **av)
{
	Server	server;
	// t_cmd	dataCmd;
	
	parse_argv(&server, ac, av);
	// dataCmd = new t_cmd();
	server._port = std::atoi(av[1]);
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

//to do : ping pong