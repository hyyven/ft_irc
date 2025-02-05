/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dferjul <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:06:00 by dravaono          #+#    #+#             */
/*   Updated: 2025/02/05 02:17:30 by dferjul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once
#include "all.hpp"

typedef struct s_cmd
{
	std::string					_message;
	std::vector<std::string>	_cmd;
}	t_cmd;

void	cmdJoin(Client *client, std::string channel);
void    checkCmd(Client *client, std::vector<std::string> cmd, Server *server);
void    cmdPrivmsg(Client *sender, const std::string& target, const std::string& message, Server *server);
void	verifyPassword(std::vector<std::string> cmd, Server *serv, Client *cli);
