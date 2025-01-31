/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:06:00 by dravaono          #+#    #+#             */
/*   Updated: 2025/01/31 14:31:05 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "all.hpp"

typedef struct s_cmd
{
	std::string					_message;
	std::vector<std::string>	_cmd;
}	t_cmd;

void	cmdJoin(int cliFd);
void    checkCmd(Client *client, std::vector<std::string> cmd, Server *server);
