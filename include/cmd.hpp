/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dferjul <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:06:00 by dravaono          #+#    #+#             */
/*   Updated: 2025/02/05 20:10:45 by dferjul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "all.hpp"

typedef struct s_cmd
{
	std::string					_message;
	std::vector<std::string>	_cmd;
}	t_cmd;

void	cmdJoin(Client *client, std::string channel, Server *serv);
void    checkCmd(Client *client, std::vector<std::string> cmd, Server *server);
void	verifyPassword(std::vector<std::string> cmd, Server *serv, Client *cli);
void    cmdPrivmsg(Client *sender, const std::string& target, const std::string& message, Server *server);
void	cmdPart(Client *client, const std::string& channel, Server *server);