/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 13:34:47 by afont             #+#    #+#             */
/*   Updated: 2025/01/31 14:12:54 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/all.hpp"

Client::Client()
{
}

Client::~Client()
{
}

void Client::sendWelcome()
{
    int status;
    
    std::vector<std::string> messages;
    messages.push_back(":server 001 " + _nickname + " :Welcome to the " + SERVER_NAME + " Network " + _nickname + "!" + _username + "@" + _ip + "\r\n");
    messages.push_back(":server 002 " + _nickname + " :Your host is " + _ip + ", running version " + VERSION + "\r\n");
    messages.push_back(":server 003 " + _nickname + " :This server was created " + __DATE__ + " " + __TIME__ + "\r\n");
    messages.push_back(":server 004 " + _nickname + " " + _ip + " " + VERSION + " None it kol\r\n");
    
    while (!messages.empty())
    {
        status = send(_fd, messages[0].c_str(), messages[0].length(), 0);
        if (status == -1)
            std::cout << "send() failed" << std::endl;
        messages.erase(messages.begin());
    }
}