/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:05:17 by dravaono          #+#    #+#             */
/*   Updated: 2025/01/30 16:03:34 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cmd.hpp"

std::vector<std::string> parser(std::string rawCmd, char delimiteur)
{
    std::vector<std::string> cmd;

    // std::string::iterator new_end = std::remove(rawCmd.begin(), rawCmd.end(), '\n');
    // rawCmd.erase(new_end, rawCmd.end());
    // new_end = std::remove(rawCmd.begin(), rawCmd.end(), '\r');
    // rawCmd.erase(new_end, rawCmd.end());
    // std::cout << "rawCmd : " << rawCmd << std::endl;
    cmd = split(rawCmd, delimiteur);
    // for (std::vector<std::string>::iterator it = cmd.begin(); it != cmd.end(); ++it)
    //     std::cout << *it << "|";
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