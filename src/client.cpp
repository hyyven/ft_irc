/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 13:34:47 by afont             #+#    #+#             */
/*   Updated: 2025/01/30 16:06:45 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/client.hpp"
#include "../include/server.hpp"
#include "../include/cmd.hpp"

Client::Client()
{
}

Client::~Client()
{
}

void	Client::setCliValue(std::string buf)
{
	_nickname = "test";
}

void Client::sendWelcome(int cli_fd)
{
    std::string nickname = _nickname;
    
    // RPL_WELCOME (001)
    std::string welcome = ":server 001 " + nickname + " :Welcome to the IRC Network " 
                         + nickname + "!" + nickname + "@" + _ip + "\r\n";
    send(cli_fd, welcome.c_str(), welcome.length(), 0);
    
    // RPL_YOURHOST (002)
    std::string yourhost = ":server 002 " + nickname + " :Your host is " + _ip 
                          + ", running version ft_irc-1.0\r\n";
    send(cli_fd, yourhost.c_str(), yourhost.length(), 0);
    
    // RPL_CREATED (003)
    std::string created = ":server 003 " + nickname + " :This server was created "
                         + __DATE__ + " " + __TIME__ + "\r\n";
    send(cli_fd, created.c_str(), created.length(), 0);
    
    // RPL_MYINFO (004)
    std::string myinfo = ":server 004 " + nickname + " " + _ip 
                        + " ft_irc-1.0 DOv woOv itkl\r\n";
    send(cli_fd, myinfo.c_str(), myinfo.length(), 0);
}