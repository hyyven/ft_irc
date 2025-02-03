/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:05:17 by dravaono          #+#    #+#             */
/*   Updated: 2025/02/03 18:53:16 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/all.hpp"

void    checkCmd(Client *client, std::vector<std::string> cmd, Server *server)
{
	size_t size;

	size = cmd.size(); 	                    // protection segfault
	if (size >= 1)
	{
		if (cmd[0] == "PASS")
		{
			verifyPassword(cmd, server, client);
		}
	}
	if (size >= 2)
	{
		if ((cmd[0] == "NICK" || cmd[0] == "USER") && !client->_isRegistered)
		{
			client->sendMessage(":server 464 " + client->_nickname + " :Password required\r\n");
			server->removeClient(client->_fd);
			close(client->_fd);
		}
		if (cmd[0] == "NICK")
		{
			client->_nickname = cmd[1];
		}
		else if (cmd[0] == "USER")
		{
			client->_username = cmd[1];
			if (client->_isRegistered)
				client->sendWelcome();
		}
		else if (cmd[0] == "QUIT" && cmd[1] == ":Leaving")
		{
			std::cout << "Client " << client->_nickname << " disconnected" << std::endl;
			server->removeClient(client->_fd);
			close(client->_fd);
		}
		else if (cmd[0] == "JOIN" && cmd[1][0] != '#')
		{
			std::string mess = cmd[1] + " :No such channel" + "\r\n";
			send(client->_fd, mess.c_str(), mess.length(), 0);
		}
		else if (cmd[0] == "JOIN" && cmd[1][0] == '#')
		{
			cmdJoin(client, cmd[1]);
		}
	}
}

void cmdJoin(Client *cli, std::string channel)
{
	static Channel channelManager;
	std::string joinMessage = ":" + cli->_nickname + "!" + cli->_username + "@" + cli->_ip + " JOIN " + channel + "\r\n";

	if (channelManager.channelExists(channel))
		cli->sendMessageToChannel(joinMessage, channelManager.getChannelClients(channel));

	channelManager.createChannel(channel, *cli);

	std::cout << "test" << std::endl;
	cli->sendMessage(joinMessage);
	cli->sendMessage(":server 332 " + cli->_nickname + " " + channel + " :\r\n");
	cli->sendMessage(":server 353 " + cli->_nickname + " = " + channel + " :" + channelManager.getChannelUsers(channel) + "\r\n");
	cli->sendMessage(":server 366 " + cli->_nickname + " " + channel + " :End of NAMES list\r\n");
}

void	verifyPassword(std::vector<std::string> cmd, Server *serv, Client *cli)
{
	if (cli->_isRegistered)
		cli->sendMessage(":server 462 " + cli->_nickname + " :You may not reregister\r\n");
	else if (cmd.size() != 2)
	{
		cli->sendMessage(":server 461 " + cli->_nickname + " PASS :Not enough parameters\r\n");
		serv->removeClient(cli->_fd);
		close(cli->_fd);
	}
	else if (cmd[1] != serv->_password)
	{
		cli->sendMessage(":server 464 " + cli->_nickname + " :Password incorrect\r\n");
		serv->removeClient(cli->_fd);
		close(cli->_fd);
	}
	else
	{
		cli->_isRegistered = true;
		// cli->sendWelcome();
	}
}
