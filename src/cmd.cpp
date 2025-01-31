/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:05:17 by dravaono          #+#    #+#             */
/*   Updated: 2025/01/31 14:32:53 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/all.hpp"

void    checkCmd(Client *client, std::vector<std::string> cmd, Server *server)
{
    size_t size;

    size = cmd.size();                      // protection segfault
    if (size >= 2)
    {
        if (cmd[0] == "NICK")
        {
            client->_nickname = cmd[1];
        }
        else if (cmd[0] == "USER")
        {
            client->_username = cmd[1];
            client->sendWelcome();
        }
        else if (cmd[0] == "QUIT" && cmd[1] == ":Leaving")
        {
            std::cout << "Client " << client->_nickname << " disconnected" << std::endl;
            server->removeClient(client->_fd);
		    close(client->_fd);
        }
    }
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