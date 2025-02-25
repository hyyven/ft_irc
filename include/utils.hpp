/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dferjul <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:59:58 by afont             #+#    #+#             */
/*   Updated: 2025/02/25 04:27:13 by dferjul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "all.hpp"

int							nickExists(std::string nickname, Server *server);
int							parserCmd(Client *cli, std::string buf);
int							initCliValue(Client *cli, Server *serv);
void						parse_argv(Server *server, int ac, char **av);
void						tryWelcome(Client *cli);
void						printvector(std::vector<std::string> vec);
std::vector<std::string>	split(const std::string& str, char delimiter);
std::string createModeMessage(Client *client, const std::string &channel, const std::string &mode, const std::string &target);
bool validateModeRequest(Client *client, const std::string &channel, const std::string &mode, const std::string &target, Server *server);