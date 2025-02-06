/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:55:43 by afont             #+#    #+#             */
/*   Updated: 2025/02/06 17:58:40 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/all.hpp"

void	parse_argv(Server *server, int ac, char **av)
{
	std::string::const_iterator it;
	std::string 				port;
	std::string					password;

	if (ac != 3)
	{
		std::cout << "Usage: " << av[0] << " <port> <password>" << std::endl;
		exit(1);
	}
	port = av[1];
	password = av[2];
	it = port.begin();
	while (it != port.end() && std::isdigit(*it))
		++it;
	if (!port.empty() && it == port.end())
	{
		int portNumber = std::atoi(port.c_str());
		if (portNumber < 49152 || portNumber > 65535)
		{
			std::cout << "Port must be between 49152 and 65535" << std::endl;
			exit(1);
		}
	}
	else
	{
		std::cout << "Invalid port" << std::endl;
		exit(1);
	}
	it = password.begin();
	while (it != password.end() && std::isalnum(*it))
		++it;
	if (password.empty() || it != password.end())
	{
		std::cout << "Password must be alphanumeric" << std::endl;
		exit(1);
	}
	else
		server->_password = password;
}

std::vector<std::string> split(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    // std::cout << "Splitting" << std::endl;
    while (std::getline(ss, token, delimiter))
    {
        // std::cout << token << "|";
        tokens.push_back(token);
    }
    // std::cout << std::endl;
	// std::cout << "End of split" << std::endl;
    return tokens;
}

int	parserCmd(Client *cli, std::string buf)
{
	if (buf.find('\r') == 0)
		cli->_dataCmd._lastBuf = buf;
	else if (cli->_dataCmd._lastBuf.find('\r') == 0 && buf.find('\n') == 0)
	{
		cli->_dataCmd._cmd = split(cli->_dataCmd._message, ' ');
		return (1);
	}
	else
	{
    	cli->_dataCmd._message += buf;
	}
	return (0);
}


void	printvector(std::vector<std::string> vec)
{
	std::vector<std::string>::iterator it;

	it = vec.begin();
	std::cout << "Vector: ";
	while (it != vec.end())
	{
		std::cout << *it << "|";
		it++;
	}
	std::cout << std::endl;
}

int	nickExists(std::string nickname, Server *server)
{
	std::vector<Client>::iterator it;

	it = server->_clients.begin();
	while (it != server->_clients.end())
	{
		if (it->_nickname == nickname)
			return (1);
		it++;
	}
	return (0);
}

void	tryWelcome(Client *cli)
{
	if (cli->_nickname != "Unknown" && cli->_username != "Unknown" && cli->_isWelcomed == false)
	{
		cli->sendWelcome();
		cli->_isWelcomed = true;
	}
}
