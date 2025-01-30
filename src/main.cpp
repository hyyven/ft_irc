/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:33:57 by afont             #+#    #+#             */
/*   Updated: 2025/01/30 17:47:23 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/all.hpp"

int	main(int ac, char **av)
{
	Server	server;
	t_cmd	*dataCmd;
	
	parser(ac, av);
	dataCmd = new t_cmd();
	server._port = std::atoi(av[1]);
	try
	{
		signal(SIGINT, Server::signalHandler);
		signal(SIGQUIT, Server::signalHandler);
		server.initServer(dataCmd);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}


//to do: reparer split qui split pas "asd      qwe a"