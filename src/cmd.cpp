/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:05:17 by dravaono          #+#    #+#             */
/*   Updated: 2025/02/07 14:56:40 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/all.hpp"

void    checkCmd(Client *cli, std::vector<std::string> cmd, Server *server)
{
	size_t size;

	size = cmd.size(); 	                    // protection segfault
	if (size >= 1)
	{
		if (cmd[0] == "NICK")
		{
			cmdChangeNickname(cli, server, cmd);
		}
	}
	if (size >= 2)
	{
		if (cmd[0] == "QUIT" && cmd[1] == ":Leaving")
		{
			std::cout << "Client " << cli->_nickname << " disconnected" << std::endl;
			close(cli->_fd);
			server->removeClient(cli->_fd);
		}
		else if (cmd[0] == "PART" && size >= 2)
		{
			cmdPart(cli, cmd[1], server);
		}
		else if (cmd[0] == "JOIN" && cmd[1][0] != '#')
		{
			std::string mess = cmd[1] + " :No such channel" + "\r\n";
			send(cli->_fd, mess.c_str(), mess.length(), 0);
		}
		else if (cmd[0] == "JOIN" && cmd[1][0] == '#')
		{
			cmdJoin(cli, cmd[1], server);
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
			cmdPrivmsg(cli, target, message, server);
		}
	}
}

void cmdJoin(Client *cli, std::string channel, Server *serv)
{
	std::string joinMessage = ":" + cli->_nickname + "!" + cli->_username + "@" + cli->_ip + " JOIN " + channel + "\r\n";

	if (serv->_channelManager.channelExists(channel))
		cli->sendMessageToChannel(joinMessage, serv->_channelManager.getChannelClients(channel));
	serv->_channelManager.createChannel(channel, cli);

	cli->sendMessage(joinMessage);
	cli->sendMessage(":server 332 " + cli->_nickname + " " + channel + " :\r\n");
	cli->sendMessage(":server 353 " + cli->_nickname + " = " + channel + " :" + serv->_channelManager.getChannelUsers(channel) + "\r\n");
	cli->sendMessage(":server 366 " + cli->_nickname + " " + channel + " :End of NAMES list\r\n");
}

void	verifyPassword(std::vector<std::string> cmd, Server *serv, Client *cli)
{
	if (cli->_isRegistered)
		cli->sendMessage(":server 462 " + cli->_nickname + " :You may not reregister\r\n");
	else if (cmd.size() != 2)
	{
		cli->sendMessage(":server 461 " + cli->_nickname + " PASS :Not enough parameters\r\n");
		close(cli->_fd);
		serv->removeClient(cli->_fd);
	}
	else if (cmd[1] != serv->_password)
	{
		cli->sendMessage(":server 464 " + cli->_nickname + " :Password incorrect\r\n");
		close(cli->_fd);
		serv->removeClient(cli->_fd);
	}
	else
		cli->_isRegistered = true;
}

void cmdPrivmsg(Client *sender, const std::string& target, const std::string& message, Server *server)
{
	if (target[0] == '#')
	{
		if (!server->_channelManager.channelExists(target))
		{
			sender->sendMessage(":server 403 " + sender->_nickname + " " + target + " :No such channel\r\n");
			return;
		}
		std::string formattedMessage = ":" + sender->_nickname + "!" + sender->_username + "@" + sender->_ip 
										+ " PRIVMSG " + target + " :" + message + "\r\n";
		server->_channelManager.broadcastMessage(target, formattedMessage, sender);
	}
	else
	{
		std::cout << "Private message from " << sender->_nickname << " to " << target << ": " << message << std::endl;
		std::string formattedMessage = ":" + sender->_nickname + "!" + sender->_username + "@" + sender->_ip 
										+ " PRIVMSG " + target + " :" + message + "\r\n";
		for (size_t i = 0; i < server->_clients.size(); ++i)
		{
			if (server->_clients[i]._nickname == target)
			{
				send(server->_clients[i]._fd, formattedMessage.c_str(), formattedMessage.length(), 0);
			}
		}
	}
}

void cmdPart(Client *client, const std::string& channel, Server *server)
{
    if (!server->_channelManager.channelExists(channel))
    {
        client->sendMessage(":server 403 " + client->_nickname + " " + channel + " :No such channel\r\n");
        return;
    }

    std::string partMessage = ":" + client->_nickname + "!" + client->_username + "@" + client->_ip
								+ " PART " + channel + " :Leaving\r\n";
	client->sendMessage(partMessage);
    client->sendMessageToChannel(partMessage, server->_channelManager.getChannelClients(channel));
    server->_channelManager.removeClientFromChannel(channel, client);
}

void	cmdChangeNickname(Client *cli, Server *server, std::vector<std::string> cmd)
{
	if (cmd.size() != 2)
	{
		cli->sendMessage(":server 431 " + cli->_nickname + " :No nickname given\r\n");
		return;
	}
	else if (nickExists(cmd[1], server))
	{
		cli->sendMessage(":server 433 " + cmd[1] + " :Nickname already taken\r\n");
	}
	else if (cli->_nickname != "Unknown")
	{
		for (size_t i = 0; i < server->_clients.size(); ++i)
			server->_clients[i].sendMessage(":" + cli->_nickname + "!" + cli->_username + "@" + cli->_ip + " NICK :" + cmd[1] + "\r\n");
		cli->_nickname = cmd[1];
	}
	else
	{
		cli->_nickname = cmd[1];
	}
}