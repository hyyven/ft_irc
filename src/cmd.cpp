/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dferjul <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:05:17 by dravaono          #+#    #+#             */
/*   Updated: 2025/03/02 02:20:38 by dferjul          ###   ########.fr       */
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
			server->_channelManager.removeClientFromAllChannels(cli);
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
		else if (cmd[0] == "KICK" && size >= 3)
		{
			cmdKick(cli, cmd[1], cmd[2], server);
		}
		else if (cmd[0] == "MODE" && size >= 3)
		{
			std::string target = (size >= 4) ? cmd[3] : "";
			cmdMode(cli, cmd[1], cmd[2], target, server);
		}
		else if (cmd[0] == "INVITE" && size >= 2)
		{
			cmdInvite(cli, cmd[1], cmd[2], server);
		}
		else if (cmd[0] == "TOPIC" && size >= 2)
		{
			std::string topic = "";
			if (size >= 3)
			{
				for (size_t i = 2; i < cmd.size(); ++i)
				{
					if (i > 2) 
						topic += " ";
					topic += cmd[i];
				}
				if (topic[0] == ':')
					topic.erase(0, 1);
			}
			cmdTopic(cli, cmd[1], topic, server);
		}
	}
}

void cmdJoin(Client *cli, std::string channel, Server *serv)
{
	std::string joinMessage = ":" + cli->_nickname + "!" + cli->_username + "@" + cli->_ip + " JOIN " + channel + "\r\n";

	if (serv->_channelManager.channelExists(channel))
	{
		if (serv->_channelManager.isInviteOnly(channel))
		{		
			if (!serv->_channelManager.isInvited(channel, cli->_nickname) && 
				!serv->_channelManager.isOperator(channel, cli))
			{
				std::cout << "User " << cli->_nickname << " is not invited to " << channel << std::endl;
				cli->sendMessage(":server 473 " + cli->_nickname + " " + channel + " :Cannot join channel (+i)\r\n");
				return;
			}
		}
		serv->_channelManager._Channel[channel].push_back(cli);
		cli->sendMessageToChannel(joinMessage, serv->_channelManager._Channel[channel]);
	}
	else
	{
		serv->_channelManager.createChannel(channel, cli);
	}
	cli->sendMessage(joinMessage);
	std::string topic = serv->_channelManager.getChannelTopic(channel);
	if (!topic.empty())
		cli->sendMessage(":server 332 " + cli->_nickname + " " + channel + " :" + topic + "\r\n");
	else
		cli->sendMessage(":server 331 " + cli->_nickname + " " + channel + " :No topic is set\r\n");
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
	client->sendMessageToChannel(partMessage, server->_channelManager._Channel[channel]);
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

void	cmdKick(Client *client, std::string channel, std::string nickname, Server *server)
{
	if (!server->_channelManager.channelExists(channel))
	{
		client->sendMessage(":server 403 " + client->_nickname + " " + channel + " :No such channel\r\n");
		return;
	}

	if (!server->_channelManager.isOperator(channel, client))
	{
		client->sendMessage(":server 482 " + client->_nickname + " " + channel + " :You're not channel operator\r\n");
		return;
	}
	if (!nickExists(nickname, server))
	{
		client->sendMessage(":server 401 " + client->_nickname + " " + nickname + " :No such nick\r\n");
		return;
	}
	Client *target = server->_channelManager.getClientFromChannel(channel, nickname);
	if (!target)
	{
		client->sendMessage(":server 441 " + client->_nickname + " " + nickname + " :They aren't on that channel\r\n");
		return;
	}
	std::string kickMessage = ":" + client->_nickname + "!" + client->_username + "@" + client->_ip + 
							 " KICK " + channel + " " + nickname + " :Kicked by operator\r\n";
	client->sendMessage(kickMessage);
	client->sendMessageToChannel(kickMessage, server->_channelManager._Channel[channel]);

	server->_channelManager.removeClientFromChannel(channel, target);
}

void cmdMode(Client *client, std::string channel, std::string mode, std::string target, Server *server)
{
	if (!validateModeRequest(client, channel, mode, target, server))
		return;
	Client *targetClient = server->_channelManager.getClientFromChannel(channel, target);
	if (mode == "+o")
	{
		server->_channelManager.addOperator(channel, targetClient);
		client->sendMessageToAllChannel(createModeMessage(client, channel, mode, target), server->_channelManager._Channel[channel]);
	}
	else if (mode == "-o")
	{
		server->_channelManager.removeOperator(channel, targetClient);
		std::string modeMessage = ":" + client->_nickname + "!" + client->_username + "@" + client->_ip + 
								" MODE " + channel + " -o " + target + "\r\n";
		client->sendMessageToAllChannel(modeMessage, server->_channelManager._Channel[channel]);
	}
	else if (mode == "+i")
	{
		server->_channelManager.setInviteOnly(channel, true);
		std::string modeMessage = ":" + client->_nickname + "!" + client->_username + "@" + client->_ip + 
								" MODE " + channel + " +i\r\n";
		client->sendMessageToAllChannel(modeMessage, server->_channelManager._Channel[channel]);
	}
	else if (mode == "-i")
	{
		server->_channelManager.setInviteOnly(channel, false);
		std::string modeMessage = ":" + client->_nickname + "!" + client->_username + "@" + client->_ip + 
								" MODE " + channel + " -i\r\n";
		client->sendMessageToAllChannel(modeMessage, server->_channelManager._Channel[channel]);
	}
	else if (mode == "+t")
	{
		server->_channelManager.setTopicRestriction(channel, true);
		std::string modeMessage = ":" + client->_nickname + "!" + client->_username + "@" + client->_ip + 
								" MODE " + channel + " +t\r\n";
		client->sendMessageToAllChannel(modeMessage, server->_channelManager._Channel[channel]);
	}
	else if (mode == "-t")
	{
		server->_channelManager.setTopicRestriction(channel, false);
		std::string modeMessage = ":" + client->_nickname + "!" + client->_username + "@" + client->_ip + 
								" MODE " + channel + " -t\r\n";
		client->sendMessageToAllChannel(modeMessage, server->_channelManager._Channel[channel]);
	}
	else
	{
		client->sendMessage(":server 472 " + client->_nickname + " " + mode + " :is unknown mode char to me\r\n");
	}
}

void cmdInvite(Client *client, std::string nickname, std::string channel, Server *server)
{
	if (!server->_channelManager.channelExists(channel))
	{
		client->sendMessage(":server 403 " + client->_nickname + " " + channel + " :No such channel\r\n");
		return;
	}
	if (!server->_channelManager.isOperator(channel, client))
	{
		client->sendMessage(":server 482 " + client->_nickname + " " + channel + " :You're not channel operator\r\n");
		return;
	}
	if (!nickExists(nickname, server))
	{
		client->sendMessage(":server 401 " + client->_nickname + " " + nickname + " :No such nick\r\n");
		return;
	}
	server->_channelManager.inviteUser(channel, nickname);
	for (size_t i = 0; i < server->_clients.size(); ++i)
	{
		if (server->_clients[i]._nickname == nickname)
		{
			std::string inviteMessage = ":" + client->_nickname + "!" + client->_username + "@" + client->_ip + 
									" INVITE " + nickname + " " + channel + "\r\n";
			server->_clients[i].sendMessage(inviteMessage);
			break;
		}
	}	
	client->sendMessage(":server 341 " + client->_nickname + " " + nickname + " " + channel + "\r\n");
}

void cmdTopic(Client *client, std::string channel, std::string topic, Server *server)
{
	if (!server->_channelManager.channelExists(channel))
	{
		client->sendMessage(":server 403 " + client->_nickname + " " + channel + " :No such channel\r\n");
		return;
	}
	if (topic.empty())
	{
		std::string currentTopic = server->_channelManager.getChannelTopic(channel);
		if (currentTopic.empty())
			client->sendMessage(":server 331 " + client->_nickname + " " + channel + " :No topic is set\r\n");
		else
			client->sendMessage(":server 332 " + client->_nickname + " " + channel + " :" + currentTopic + "\r\n");
		return;
	}
	if (server->_channelManager.isTopicRestricted(channel) && !server->_channelManager.isOperator(channel, client))
	{
		client->sendMessage(":server 482 " + client->_nickname + " " + channel + " :You're not channel operator\r\n");
		return;
	}
	server->_channelManager.setChannelTopic(channel, topic);
	std::string topicMessage = ":" + client->_nickname + "!" + client->_username + "@" + client->_ip + 
							" TOPIC " + channel + " :" + topic + "\r\n";
	client->sendMessageToAllChannel(topicMessage, server->_channelManager._Channel[channel]);
}