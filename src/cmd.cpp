/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:05:17 by dravaono          #+#    #+#             */
/*   Updated: 2025/01/31 15:17:42 by afont            ###   ########.fr       */
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
        else if (cmd[0] == "JOIN")
        {
            cmdJoin(client, cmd[1]);
        }
    }
}

void cmdJoin(Client *cli, std::string channel)
{
    int status;

    std::vector<std::string> messages;
    messages.push_back(":" + cli->_nickname + "!" + cli->_username + "@" + cli->_ip + " JOIN #" + channel + "\r\n");
    messages.push_back(":server 332 " + cli->_nickname + " #" + channel + " :\r\n");
    messages.push_back(":server 353 " + cli->_nickname + " = #" + channel + " :" + cli->_nickname + "\r\n");
    messages.push_back(":server 366 " + cli->_nickname + " #" + channel + " :End of NAMES list\r\n");
    while (!messages.empty())
    {
        // std::cout << messages[0] << "sent" << std::endl;
        status = send(cli->_fd, messages[0].c_str(), messages[0].length(), 0);
        if (status == -1)
            std::cout << "send() failed" << std::endl;
        messages.erase(messages.begin());
    }
}