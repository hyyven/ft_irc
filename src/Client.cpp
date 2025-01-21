/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 13:34:47 by afont             #+#    #+#             */
/*   Updated: 2025/01/21 13:36:29 by afont            ###   ########.fr       */
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