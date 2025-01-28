/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dravaono <dravaono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:06:00 by dravaono          #+#    #+#             */
/*   Updated: 2025/01/28 15:15:57 by dravaono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server.hpp"
#include "client.hpp"
#include <string>
#include <sstream>

// struct cmd
// {
	
// };

std::vector<std::string>    parser(std::string rawCmd);
void cmdJoin(int cliFd);
