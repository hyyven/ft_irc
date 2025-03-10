/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:05:17 by dravaono          #+#    #+#             */
/*   Updated: 2025/03/10 14:17:11 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/all.hpp"

void	processPongCmd(Client *cli)
{
	cli->_isWaitingPong = false;
	cli->_lastActivity = time(NULL);
}

void	processQuitCmd(Client *cli, std::vector<std::string> cmd, Server *server)	
{
	if (cmd[1] == ":Leaving")
	{
		server->_channelManager.removeClientFromAllChannels(cli);
		close(cli->_fd);
		server->removeClient(cli->_fd);
	}
}

void processJoinCmd(Client *cli, std::vector<std::string> cmd, Server *server)
{
	if (cmd[1][0] == '#')
		cmdJoin(cli, cmd[1], server);
	else
		sendError(cli, "403", cmd[1], "No such channel");
}

void processPrivmsgCmd(Client *cli, std::vector<std::string> cmd, Server *server)
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

void processTopicCmd(Client *cli, std::vector<std::string> cmd, Server *server)
{
	std::string topic = "";
	if (cmd.size() >= 3)
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

void    checkCmd(Client *cli, std::vector<std::string> cmd, Server *server)
{
	size_t size = cmd.size();
	if (size <= 1)
		return ;
	if (cmd[0] == "NICK" && cli->_isWelcomed)
		cmdChangeNickname(cli, server, cmd);
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
			cmdPart(cli, cmd[1], server);
		else if (cmd[0] == "JOIN")
			processJoinCmd(cli, cmd, server);
		else if (cmd[0] == "PRIVMSG" && size >= 3)
			processPrivmsgCmd(cli, cmd, server);
		else if (cmd[0] == "KICK" && size >= 3)
			cmdKick(cli, cmd[1], cmd[2], server);
		else if (cmd[0] == "PONG")
			processPongCmd(cli);
		else if (cmd[0] == "MODE" && size >= 3)
		{
			std::string target = (size >= 4) ? cmd[3] : "";
			cmdMode(cli, cmd[1], cmd[2], target, server);
		}
		else if (cmd[0] == "INVITE" && size >= 2)
			cmdInvite(cli, cmd[1], cmd[2], server);
		else if (cmd[0] == "TOPIC" && size >= 2)
			processTopicCmd(cli, cmd, server);
	}
}

void cmdJoin(Client *cli, std::string channel, Server *serv)
{
	std::string joinMessage = ":" + cli->_nickname + "!" + cli->_username + "@" + cli->_ip + " JOIN " + channel + "\r\n";

	if (serv->_channelManager.channelExists(channel))
	{
		if (serv->_channelManager.hasPassword(channel))
		{
			if (cli->_dataCmd._cmd.size() < 3)
			{
				sendError(cli, "475", channel, "Cannot join channel (+k)");
				return;
			}
			std::string password = cli->_dataCmd._cmd[2];
			if (!serv->_channelManager.checkChannelPassword(channel, password))
			{
				sendError(cli, "475", channel, "Cannot join channel (+k)");
				return;
			}
		}
		if (serv->_channelManager.isInviteOnly(channel))
		{		
			if (!serv->_channelManager.isInvited(channel, cli->_nickname) && 
				!serv->_channelManager.isOperator(channel, cli))
			{
				std::cout << "User " << cli->_nickname << " is not invited to " << channel << std::endl;
				sendError(cli, "473", channel, "Cannot join channel (+i)");
				return;
			}
		}
		if (serv->_channelManager.hasUserLimit(channel) && 
			serv->_channelManager.isChannelFull(channel))
		{
			sendError(cli, "471", channel, "Cannot join channel (+l)");
			return;
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
		sendError(cli, "331", channel, "No topic is set");
	cli->sendMessage(":server 353 " + cli->_nickname + " = " + channel + " :" + serv->_channelManager.getChannelUsers(channel) + "\r\n");
	cli->sendMessage(":server 366 " + cli->_nickname + " " + channel + " :End of NAMES list\r\n");
}

void	verifyPassword(std::vector<std::string> cmd, Server *serv, Client *cli)
{
	if (cli->_isRegistered)
		sendError(cli, "462", "", "You may not reregister");
	else if (cmd.size() != 2)
	{
		sendError(cli, "461", "", "Not enough parameters");
		close(cli->_fd);
		serv->removeClient(cli->_fd);
	}
	else if (cmd[1] != serv->_password)
	{
		sendError(cli, "464", "", "Password incorrect");
		close(cli->_fd);
		serv->removeClient(cli->_fd);
	}
	else
		cli->_isRegistered = true;
}

void cmdPrivmsg(Client *sender, const std::string& target, const std::string& message, Server *server)
{
	int	i;
	
	if (target[0] == '#')
	{
		if (!checkChannelExists(sender, target, server))
			return;
		std::string formattedMessage = createFormattedMessage(sender, "PRIVMSG", target + " :" + message);
		server->_channelManager.broadcastMessage(target, formattedMessage, sender);
		if (target == "#bot")
			processBotCommand(message, server);
	}
	else
	{
		std::cout << "Private message from " << sender->_nickname << " to " << target << ": " << message << std::endl;
		std::string formattedMessage = createFormattedMessage(sender, "PRIVMSG", target + " :" + message);
		
		i = nickExists(target, server);
		if (i)
			send(server->_clients[i]._fd, formattedMessage.c_str(), formattedMessage.length(), 0);
	}
}

void cmdPart(Client *cli, const std::string& channel, Server *server)
{
	if (!checkChannelExists(cli, channel, server))
		return;
	std::string partMessage = createFormattedMessage(cli, "PART", channel + " :Leaving");
	cli->sendMessage(partMessage);
	cli->sendMessageToChannel(partMessage, server->_channelManager._Channel[channel]);
	server->_channelManager.removeClientFromChannel(channel, cli);
}

void	cmdChangeNickname(Client *cli, Server *server, std::vector<std::string> cmd)
{
	if (cmd.size() != 2)
	{
		cli->sendMessage(":server 431 " + cli->_nickname + " :No nickname given\r\n");
		return;
	}
	else if (nickExists(cmd[1], server))
		cli->sendMessage(":server 433 " + cmd[1] + " :Nickname already taken\r\n");
	else if (cli->_nickname != "Unknown")
	{
		for (size_t i = 0; i < server->_clients.size(); ++i)
			server->_clients[i].sendMessage(":" + cli->_nickname + "!" + cli->_username + "@" + cli->_ip + " NICK :" + cmd[1] + "\r\n");
		cli->_nickname = cmd[1];
	}
	else
		cli->_nickname = cmd[1];
}

void	cmdKick(Client *cli, std::string channel, std::string nickname, Server *server)
{
	if (!server->_channelManager.channelExists(channel))
	{
		sendError(cli, "403", channel, "No such channel");
		return;
	}
	if (!server->_channelManager.isOperator(channel, cli))
	{
		sendError(cli, "482", channel, "You're not channel operator");
		return;
	}
	if (!nickExists(nickname, server))
	{
		sendError(cli, "401", nickname, "No such nick");
		return;
	}
	Client *target = server->_channelManager.getClientFromChannel(channel, nickname);
	if (!target)
	{
		sendError(cli, "441", nickname, "They aren't on that channel");
		return;
	}
	std::string kickMessage = createFormattedMessage(cli, "KICK", channel + " " + nickname + " :Kicked by operator");
	cli->sendMessage(kickMessage);
	cli->sendMessageToChannel(kickMessage, server->_channelManager._Channel[channel]);

	server->_channelManager.removeClientFromChannel(channel, target);
}

void cmdMode(Client *cli, std::string channel, std::string mode, std::string target, Server *server)
{
	if (!validateModeRequest(cli, channel, mode, target, server))
		return;

	Client *targetClient = server->_channelManager.getClientFromChannel(channel, target);
	std::string modeMessage;

	if (mode == "+o" || mode == "-o")
	{
		if (mode == "+o")
			server->_channelManager.addOperator(channel, targetClient);
		else
			server->_channelManager.removeOperator(channel, targetClient);
		modeMessage = createModeMessage(cli, channel, mode, target);
	}
	else if (mode == "+i" || mode == "-i")
	{
		server->_channelManager.setInviteOnly(channel, mode == "+i");
		modeMessage = createModeMessage(cli, channel, mode, "");
	}
	else if (mode == "+t" || mode == "-t")
	{
		server->_channelManager.setTopicRestriction(channel, mode == "+t");
		modeMessage = createModeMessage(cli, channel, mode, "");
	}
	else if (mode == "+k" || mode == "-k")
	{
		if (mode == "+k")
			server->_channelManager.setChannelPassword(channel, target);
		else
			server->_channelManager.removeChannelPassword(channel);
		modeMessage = createModeMessage(cli, channel, mode, mode == "+k" ? target : "");
	}
	else if (mode == "+l" || mode == "-l")
	{
		if (mode == "+l")
		{
			if (target.empty())
			{
				sendError(cli, "461", "MODE", "Not enough parameters");
				return;
			}
			size_t limit = std::atoi(target.c_str());
			if (limit <= 0)
			{
				sendError(cli, "472", mode, "Invalid limit");
				return;
			}
			server->_channelManager.setUserLimit(channel, limit);
			modeMessage = createModeMessage(cli, channel, mode, target);
		}
		else
		{
			server->_channelManager.removeUserLimit(channel);
			modeMessage = createModeMessage(cli, channel, mode, "");
		}
	}
	else
	{
		sendError(cli, "472", mode, "is unknown mode char to me");
		return;
	}

	cli->sendMessageToAllChannel(modeMessage, server->_channelManager._Channel[channel]);
}

void cmdInvite(Client *cli, std::string nickname, std::string channel, Server *server)
{
	int	i;
	if (!checkChannelExists(cli, channel, server))
		return;
	if (!server->_channelManager.isOperator(channel, cli))
	{
		sendError(cli, "482", channel, "You're not channel operator");
		return;
	}
	if (!nickExists(nickname, server))
	{
		sendError(cli, "401", nickname, "No such nick");
		return;
	}
	server->_channelManager.inviteUser(channel, nickname);
	i = nickExists(nickname, server);
	if (i)
	{
		std::string inviteMessage = createFormattedMessage(cli, "INVITE", nickname + " " + channel);
		server->_clients[i].sendMessage(inviteMessage);
	}
	sendError(cli, "341", nickname + " " + channel, "");
}

void cmdTopic(Client *cli, std::string channel, std::string topic, Server *server)
{
	if (!checkChannelExists(cli, channel, server))
		return;
	if (topic.empty())
	{
		std::string currentTopic = server->_channelManager.getChannelTopic(channel);
		if (currentTopic.empty())
			sendError(cli, "331", channel, "No topic is set");
		else
			sendError(cli, "332", channel, currentTopic);
		return;
	}
	if (server->_channelManager.isTopicRestricted(channel) && !server->_channelManager.isOperator(channel, cli))
	{
		sendError(cli, "482", channel, "You're not channel operator");
		return;
	}
	server->_channelManager.setChannelTopic(channel, topic);
	std::string topicMessage = createFormattedMessage(cli, "TOPIC", channel + " :" + topic);
	cli->sendMessageToAllChannel(topicMessage, server->_channelManager._Channel[channel]);
}
