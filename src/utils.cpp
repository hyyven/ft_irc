/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dferjul <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:55:43 by afont             #+#    #+#             */
/*   Updated: 2025/03/04 12:13:00 by dferjul          ###   ########.fr       */
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
			sendError(cli, "451", "", "You have not registered");
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
		sendError(client, "403", channel, "No such channel");
		return false;
	}
	if (mode.empty())
	{
		sendError(client, "461", "", "Not enough parameters");
		return false;
	}	
	if (!server->_channelManager.isOperator(channel, client))
	{
		sendError(client, "482", channel, "You're not channel operator");
		return false;
	}

	// Vérifications spécifiques selon le mode
	if (mode == "+i" || mode == "-i" || mode == "+t" || mode == "-t")
	{
		return true;
	}
	else if (mode == "+k" || mode == "-k")
	{
		if (mode == "+k" && target.empty())
		{
			sendError(client, "461", "", "Not enough parameters");
			return false;
		}
		return true;
	}
	else if (mode == "+l" || mode == "-l")
	{
		if (mode == "+l" && target.empty())
		{
			sendError(client, "461", "", "Not enough parameters");
			return false;
		}
		return true;
	}
	else if (mode == "+o" || mode == "-o")
	{
		if (target.empty())
		{
			sendError(client, "461", "", "Not enough parameters");
			return false;
		}
		if (!nickExists(target, server))
		{
			sendError(client, "401", target, "No such nick");
			return false;
		}
		if (!server->_channelManager.getClientFromChannel(channel, target))
		{
			sendError(client, "441", target, "They aren't on that channel");
			return false;
		}
	}
	else
	{
		sendError(client, "472", mode, "is unknown mode char to me");
		return false;
	}

	return true;
}

bool checkChannelExists(Client *client, const std::string &channel, Server *server)
{
	if (!server->_channelManager.channelExists(channel))
	{
		sendError(client, "403", channel, "No such channel");
		return false;
	}
	return true;
}

std::string createFormattedMessage(const Client *client, const std::string &command, const std::string &params)
{
	return ":" + client->_nickname + "!" + client->_username + "@" + client->_ip + 
		   " " + command + " " + params + "\r\n";
}

void sendError(Client *client, const std::string &code, const std::string &target, const std::string &message)
{
	client->sendMessage(":server " + code + " " + client->_nickname + " " + target + " :" + message + "\r\n");
}