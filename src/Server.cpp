/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:32:15 by afont             #+#    #+#             */
/*   Updated: 2025/01/10 13:06:41 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/client.hpp"
#include "../include/server.hpp"

Server::Server()
{
}

Server::~Server()
{
}

void	Server::setPort(int port)
{
	this->_port = port;
}

int		Server::getPort()
{
	return (this->_port);
}

void	Server::initSocket()
{
	int		status;
	struct	sockaddr_in	serv_addr;
	struct	pollfd		pfd;

	serv_addr.sin_family = AF_INET;			//IPv4
	serv_addr.sin_addr.s_addr = INADDR_ANY; //accept only local connections
	serv_addr.sin_port = htons(getPort());	//set port
	this->_socketFd = socket(AF_INET, SOCK_STREAM, 0); //SOCK_STREAM for TCP, 0 for ???
	if (this->_socketFd == -1)
		throw std::runtime_error("Error: socket creation failed");
	status = bind(this->_socketFd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (status == -1)
		throw std::runtime_error("Error: bind failed");
	
}

void	Server::initServer()
{
	setPort(4242);
	initSocket();
}
