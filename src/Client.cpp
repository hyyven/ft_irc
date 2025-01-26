/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 13:34:47 by afont             #+#    #+#             */
/*   Updated: 2025/01/24 12:33:21 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/client.hpp"
#include "../include/server.hpp"

Client::Client()
{
}

Client::~Client()
{
}

int	Client::getFd()
{
	return (this->_fd);
}

std::string	Client::getIp()
{
	return (this->_ip);
}

void	Client::setFd(int fd)
{
	this->_fd = fd;
}

void	Client::setIp(std::string ip)
{
	this->_ip = ip;
}

char	*Client::getService()
{
	return (this->_service);
}

void	Client::setService(char *service)
{
	strcpy(this->_service, service);
}

void	Client::sendWelcome(int cli_fd)
{
	std::cout << "host: " << getIp() << " service: " << getService() << " fd: " << getFd() << std::endl;
	std::string welcome = ":server 001 " + getIp() + " :Welcome to the IRC Network\r\n";
	send(cli_fd, welcome.c_str(), welcome.length(), 0);
	std::string yourhost = ":server 002 " + getIp() + " :Your host is " + getIp() + "\r\n";
	send(cli_fd, yourhost.c_str(), yourhost.length(), 0);
}