/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dferjul <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:05:17 by dravaono          #+#    #+#             */
/*   Updated: 2025/02/05 02:16:19 by dferjul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/all.hpp"

void    checkCmd(Client *client, std::vector<std::string> cmd, Server *server)
{
	size_t size;

	size = cmd.size();                      // protection segfault
	if (size >= 2)
	{
		if (cmd[0] == "NICK")
		{
			client->_nickname = cmd[1];
		}
		else if (cmd[0] == "USER")
		{
			client->_username = cmd[1];
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
			std::cout << "Client " << client->_nickname << " tried to join a non-existing channel" << std::endl;
			std::string mess = cmd[1] + " :No such channel" + "\r\n";
			send(client->_fd, mess.c_str(), mess.length(), 0);
		}
		else if (cmd[0] == "JOIN" && cmd[1][0] == '#')
		{
			std::cout << "Client " << client->_nickname << " joined channel " << cmd[1] << std::endl;
			cmdJoin(client, cmd[1]);
		}
		else if (cmd[0] == "PRIVMSG" && size >= 3)
		{
			std::string target = cmd[1];
			std::string message;
			
			for (size_t i = 2; i < cmd.size(); ++i)
			{
				if (i > 2) 
					message += " ";
				message += cmd[i];
			}			
			if (message[0] == ':')
    			message.erase(0, 1);
			cmdPrivmsg(client, target, message, server);
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

	cli->sendMessage(joinMessage);
	cli->sendMessage(":server 332 " + cli->_nickname + " " + channel + " :\r\n");
	cli->sendMessage(":server 353 " + cli->_nickname + " = " + channel + " :" + channelManager.getChannelUsers(channel) + "\r\n");
	cli->sendMessage(":server 366 " + cli->_nickname + " " + channel + " :End of NAMES list\r\n");
}

void cmdPrivmsg(Client *sender, const std::string& target, const std::string& message, Server *server)
{
	if (target[0] == '#')
	{
		std::cout << "Channel message from " << sender->_nickname << " to " << target << ": " << message << std::endl;

		std::string formattedMessage = ":" + sender->_nickname + "!" + sender->_username + "@" + sender->_ip;
		formattedMessage += " PRIVMSG " + target + " :" + message + "\r\n";

		for (size_t i = 0; i < server->_clients.size(); ++i)
		{
			if (server->_clients[i]._nickname != sender->_nickname)
			{
				std::cout << "Sending to " << server->_clients[i]._nickname << std::endl;
				send(server->_clients[i]._fd, formattedMessage.c_str(), formattedMessage.length(), 0);
			}
		}
	}
	else
	{
		std::cout << "Private message from " << sender->_nickname << " to " << target << ": " << message << std::endl;

		std::string formattedMessage = ":" + sender->_nickname + "!" + sender->_username + "@" + sender->_ip;
		formattedMessage += " PRIVMSG " + target + " :" + message + "\r\n";

		for (size_t i = 0; i < server->_clients.size(); ++i)
		{
			if (server->_clients[i]._nickname == target)
			{
				std::cout << "Sending to " << server->_clients[i]._nickname << std::endl;
				send(server->_clients[i]._fd, formattedMessage.c_str(), formattedMessage.length(), 0);
			}
		}
	}
}