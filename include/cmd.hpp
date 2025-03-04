/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dferjul <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:06:00 by dravaono          #+#    #+#             */
/*   Updated: 2025/03/03 04:15:10 by dferjul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "all.hpp"

typedef struct s_cmd
{
	std::string					_lastBuf;
	std::string					_message;
	std::vector<std::string>	_cmd;
}	t_cmd;

void	cmdJoin(Client *cli, std::string channel, Server *serv);
void    checkCmd(Client *cli, std::vector<std::string> cmd, Server *server);
void	verifyPassword(std::vector<std::string> cmd, Server *serv, Client *cli);
void    cmdPrivmsg(Client *sender, const std::string& target, const std::string& message, Server *server);
void	cmdPart(Client *cli, const std::string& channel, Server *server);
void	cmdChangeNickname(Client *cli, Server *server, std::vector<std::string> cmd);
void	cmdKick(Client *cli, std::string channel, std::string nickname, Server *server);
void	cmdMode(Client *cli, std::string channel, std::string mode, std::string target, Server *server);
void	cmdInvite(Client *cli, std::string nickname, std::string channel, Server *server);
void	cmdTopic(Client *cli, std::string channel, std::string topic, Server *server);

// Process the command
void	processQuitCmd(Client *cli, std::vector<std::string> cmd, Server *server);
void	processJoinCmd(Client *cli, std::vector<std::string> cmd, Server *server);
void	processPrivmsgCmd(Client *cli, std::vector<std::string> cmd, Server *server);
void	processTopicCmd(Client *cli, std::vector<std::string> cmd, Server *server);