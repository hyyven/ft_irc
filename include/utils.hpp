/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:59:58 by afont             #+#    #+#             */
/*   Updated: 2025/01/30 17:49:08 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "all.hpp"

void						parser(int ac, char **av);
void						printvector(std::vector<std::string> vec);
int							parserCmd(t_cmd *dataCmd, std::string buf);
std::vector<std::string>	split(const std::string& str, char delimiter);