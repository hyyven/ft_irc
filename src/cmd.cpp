/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dferjul <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:05:17 by dravaono          #+#    #+#             */
/*   Updated: 2025/01/28 20:43:58 by dferjul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cmd.hpp"

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter))
        tokens.push_back(token);
    return tokens;
}

std::vector<std::string> parser(std::string rawCmd)
{
    std::vector<std::string> cmd;

    std::cout << "rawCmd : " << rawCmd << std::endl;
    cmd = split(rawCmd, ' ');
    for (std::vector<std::string>::iterator it = cmd.begin(); it != cmd.end(); ++it)
        std::cout << *it << std::endl;
    return (cmd);
}



void cmdJoin(int cliFd)
{
    int i;
    std::string mess = ":dravaono!dravaono@127.0.0.1 JOIN #test";
    i = send(cliFd, mess.c_str(), mess.length(), 0);
    if (i == -1)
        std::cout << "send() failed" << std::endl;
    mess = ":127.0.0.1 332 dravaono #test :Bienvenue sur ce canal!";
    i = send(cliFd, mess.c_str(), mess.length(), 0);
    if (i == -1)
        std::cout << "send() failed" << std::endl;
    mess = ":127.0.0.1 353 dravaono = #test :Alice Bob Charlie";
    i = send(cliFd, mess.c_str(), mess.length(), 0);
    if (i == -1)
        std::cout << "send() failed" << std::endl;
    mess = ":127.0.0.1 366 dravaono #test :End of /NAMES list";
    i = send(cliFd, mess.c_str(), mess.length(), 0);
    if (i == -1)
        std::cout << "send() failed" << std::endl;
    std::cout << "JOIN command sent" << std::endl;
}