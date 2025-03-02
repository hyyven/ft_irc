/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dferjul <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:55:43 by afont             #+#    #+#             */
/*   Updated: 2025/03/01 03:54:30 by dferjul          ###   ########.fr       */
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
	std::map<int, Client>::iterator it = server->_clients.begin();
	while (it != server->_clients.end())
	{
		if (it->second._nickname == nickname)
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

int	initCliValue(Client *cli, Server *serv)
{
	size_t size = cli->_dataCmd._cmd.size();
	if (cli->_isRegistered && cli->_nickname != "Unknown" && cli->_username != "Unknown")
	{
		return (1);
	}
	if (size >= 1)
	{
		if (cli->_dataCmd._cmd[0] == "NICK")
		{
			cmdChangeNickname(cli, serv, cli->_dataCmd._cmd);
		}
	}
	if (size >= 2)
	{
		if (cli->_dataCmd._cmd[0] == "PASS")
		{
			verifyPassword(cli->_dataCmd._cmd, serv, cli);
		}
		else if (cli->_dataCmd._cmd[0] == "USER")
		{
			cli->_username = cli->_dataCmd._cmd[1];
		}
	}
	if (!cli->_isRegistered || cli->_nickname == "Unknown" || cli->_username == "Unknown")
	{
		if (cli->_dataCmd._cmd[0] != "NICK" && cli->_dataCmd._cmd[0] != "USER" && cli->_dataCmd._cmd[0] != "PASS" && cli->_dataCmd._cmd[0] != "CAP")
		{
			cli->sendMessage(":server 451 " + cli->_nickname + " :You have not registered\r\n");
		}
		return (0);
	}
	else
	{
		return (1);
	}
}

std::string createModeMessage(Client *client, const std::string &channel, const std::string &mode, const std::string &target)
{
	return ":" + client->_nickname + "!" + client->_username + "@" + client->_ip + " MODE " + channel + " " + mode + " " + target + "\r\n";
}

bool validateModeRequest(Client *client, const std::string &channel, const std::string &mode, const std::string &target, Server *server)
{
	if (!server->_channelManager.channelExists(channel))
	{
		client->sendMessage(":server 403 " + client->_nickname + " " + channel + " :No such channel\r\n");
		return false;
	}
	if (mode.empty())
	{
		client->sendMessage(":server 461 " + client->_nickname + " MODE :Not enough parameters\r\n");
		return false;
	}	
	if (!server->_channelManager.isOperator(channel, client))
	{
		client->sendMessage(":server 482 " + client->_nickname + " " + channel + " :You're not channel operator\r\n");
		return false;
	}
	if (mode == "+i" || mode == "-i")
	{
		std::cout << "Mode " << mode << " accepted without target" << std::endl;
		return true;
	}
	if ((mode == "+o" || mode == "-o") && target.empty())
	{
		client->sendMessage(":server 461 " + client->_nickname + " MODE :Not enough parameters\r\n");
		return false;
	}
	if (!nickExists(target, server))
	{
		client->sendMessage(":server 401 " + client->_nickname + " " + target + " :No such nick\r\n");
		return false;
	}
	if (!server->_channelManager.getClientFromChannel(channel, target))
	{
		client->sendMessage(":server 441 " + client->_nickname + " " + target + " :They aren't on that channel\r\n");
		return false;
	}
	return true;
}