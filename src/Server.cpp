/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:32:15 by afont             #+#    #+#             */
/*   Updated: 2025/01/21 14:54:45 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/client.hpp"
#include "../include/server.hpp"


bool Server::_signal = false;
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

void	Server::setFd(int fd)
{
	this->_socketFd = fd;
}

int		Server::getFd()
{
	return (this->_socketFd);
}

void	Server::signalHandler(int signum)
{
	(void)signum;
	Server::_signal = true;
}

void	Server::closeFd()
{
	size_t	i;

	i = 0;
	while (i < this->_clients.size())
	{
		std::cout << "Client " << this->_clients[i].getIp() << " disconnected" << std::endl;
		close(this->_clients[i++].getFd());
	}
	if (getFd() != -1)
	{
		std::cout << "Server closed" << std::endl;
		close(getFd());
	}
}

void	Server::removeClient(int fd)
{
	size_t	i;

	i = 0;
	while (i < this->_pfds.size())
	{
		if (this->_pfds[i].fd == fd)
		{
			this->_pfds.erase(this->_pfds.begin() + i);
			break;
		}
	}
	i = 0;
	while (i < this->_clients.size())
	{
		if (this->_clients[i].getFd() == fd)
		{
			this->_clients.erase(this->_clients.begin() + i);
			break;
		}
	}
}

void	Server::newClient()
{
	Client				cli;
	struct sockaddr_in	cli_addr;
	struct pollfd		pfd;
	socklen_t			len;
	int					cli_fd;
	int					status;

	len = sizeof(cli_addr);
	cli_fd = accept(getFd(), (struct sockaddr *)&cli_addr, &len);
	if (cli_fd == -1)
	{
		std::cout << "accept() client failed" << std::endl;
		return;
	}
	status = fcntl(cli_fd, F_SETFL, O_NONBLOCK);
	if (status == -1)
	{
		std::cout << "fcntl() client failed" << std::endl;
		return;
	}
	pfd.fd = cli_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	cli.setFd(cli_fd);
	cli.setIp(inet_ntoa(cli_addr.sin_addr));
	this->_clients.push_back(cli);
	this->_pfds.push_back(pfd);
}

void	Server::initSocket()
{
	int		status;
	int		opt = 1;
	struct	sockaddr_in	serv_addr;
	struct	pollfd		pfd;

	serv_addr.sin_family = AF_INET;												//IPv4
	serv_addr.sin_addr.s_addr = INADDR_ANY;										//accept only local connections 127.0.0.1
	serv_addr.sin_port = htons(getPort());										//set port
	setFd(socket(AF_INET, SOCK_STREAM, 0));										//SOCK_STREAM for TCP, 0 for ???
	if (getFd() == -1)
		throw std::runtime_error("Error: socket creation failed");
	status = bind(getFd(), (struct sockaddr *)&serv_addr, sizeof(serv_addr));	//bind socket to address
	if (status == -1)
		throw std::runtime_error("Error: bind failed");
	status = fcntl(getFd(), F_SETFL, O_NONBLOCK);								//set socket to non-blocking
	if (status == -1)
		throw std::runtime_error("Error: fcntl failed");
	status = setsockopt(getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));	//set socket options to reuse address
	if (status == -1)
		throw std::runtime_error("Error: setsockopt failed");
	status = listen(getFd(), SOMAXCONN);										//listen for connections, 4096 clients max
	if (status == -1)
		throw std::runtime_error("Error: listen failed");
	pfd.fd = getFd();
	pfd.events = POLLIN;														// to read data
	pfd.revents = 0;															// no return events
	this->_pfds.push_back(pfd);
}

void	Server::processData(int fd)
{
	char	buf[1024];
	ssize_t	bytes;

	memset(buf, 0, 1024);
	bytes = recv(fd, buf, 1024, 0);
	if (bytes <= 0)
	{
		std::cout << "recv() failed" << std::endl;
		removeClient(fd);
		close(fd);
		return;
	}
	else
	{
		buf[bytes] = '\0';
		std::cout << "Data received: " << buf << std::endl;
	}
}

void	Server::initServer()
{
	int	status;
	size_t	i;

	setPort(4242);
	initSocket();
	std::cout << "Server started on port " << getPort() << std::endl;
	while (!this->_signal)
	{
		status = poll(&this->_pfds[0], this->_pfds.size(), -1);
		if (status == -1 && !this->_signal)
			throw std::runtime_error("Error: poll failed");
		i = 0;
		while (i < this->_pfds.size())
		{
			if (this->_pfds[i].revents & POLLIN)	// Vérifie si des données sont disponibles en lecture
			{
				if (this->_pfds[i].fd == getFd())
				{
					newClient();
					std::cout << "New client connected" << std::endl;
				}
				else
				{
					std::cout << "Data received from client" << std::endl;
					processData(this->_pfds[i].fd);
				}
			}
			i++;
		}
	}
	closeFd();
}
