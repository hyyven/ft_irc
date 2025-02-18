/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dferjul <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:32:15 by afont             #+#    #+#             */
/*   Updated: 2025/02/18 12:17:13 by dferjul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/all.hpp"


bool Server::_signal = false;
Server::Server() {}

Server::~Server() {}

void	Server::signalHandler(int signum)
{
	(void)signum;
	Server::_signal = true;
}

void	Server::closeFd()
{
	std::map<int, Client>::iterator it = _clients.begin();
	while (it != _clients.end())
	{
		std::cout << "Client " << it->second._nickname << " disconnected" << std::endl;
		close(it->first);
		++it;
	}
	if (_socketFd != -1)
	{
		std::cout << "Server closed" << std::endl;
		if (close(_socketFd) == -1)
			std::cout << "close() failed" << std::endl;
	}
}

void	Server::removeClient(int fd)
{
	size_t	i;

	i = 0;
	while (i < this->_pfds.size())
	{
		// std::cout << "\\" << std::endl;
		if (this->_pfds[i].fd == fd)
		{
			this->_pfds.erase(this->_pfds.begin() + i);
			break;
		}
		i++;
	}
	i = 0;
	while (i < this->_clients.size())
	{
		if (this->_clients[i]._fd == fd)
		{
			// this->_clients.erase(this->_clients.begin() + i);
			_pfds.erase(_pfds.begin() + i);
			break;
		}
		i++;
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
	cli_fd = accept(_socketFd, (struct sockaddr *)&cli_addr, &len);
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
	cli._fd = cli_fd;
	cli._nickname = "Unknown";
	cli._username = "Unknown";
	cli._isRegistered = false;
	cli._isWelcomed = false;
	cli._ip = inet_ntoa(cli_addr.sin_addr);
	// this->_clients.push_back(cli);
	_clients[cli_fd] = cli;  // Utiliser le fd comme clé
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
	serv_addr.sin_port = htons(_port);											//set port
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);								//SOCK_STREAM for TCP, 0 for ???
	if (_socketFd == -1)
		throw std::runtime_error("Error: socket creation failed");
	status = setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));//set socket options to reuse address
	if (status == -1)
		throw std::runtime_error("Error: setsockopt failed");
	status = bind(_socketFd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));	//bind socket to address
	if (status == -1)
		throw std::runtime_error("Error: bind failed");
	status = fcntl(_socketFd, F_SETFL, O_NONBLOCK);								//set socket to non-blocking
	if (status == -1)
		throw std::runtime_error("Error: fcntl failed");
	status = listen(_socketFd, SOMAXCONN);										//listen for connections, 4096 clients max
	if (status == -1)
		throw std::runtime_error("Error: listen failed");
	pfd.fd = _socketFd;
	pfd.events = POLLIN;														// to read data
	pfd.revents = 0;															// no return events
	this->_pfds.push_back(pfd);
}

int	Server::getClientIndex(int fd)
{
	size_t	i;

	i = 0;
	while (i < this->_clients.size())
	{
		if (this->_clients[i]._fd == fd)
			return (i);
		i++;
	}
	return (-1);
}

void	Server::processData(int fd)
{
	std::string buf(1, 0);
	size_t	bytes;
	size_t	i;	

	i = getClientIndex(fd);
	if (i == size_t(-1))
	{
		std::cout << "Client not found, index = -1" << std::endl;
		return;
	}
	bytes = recv(fd, &buf[0], 1, 0);
	if (bytes <= 0)
	{
		std::cout << "recv() failed / client disconnected " << std::endl;
		close(fd);
		removeClient(fd);
	}
	else
	{
		if (parserCmd(&_clients[i], buf) == 1) //si c'est la fin du message
		{
			std::cout << "Message: [" << _clients[i]._dataCmd._message << "]" << std::endl;
			// std::cout << "i: " << i << std::endl;
			// printvector(dataCmd->_cmd);
			if (initCliValue(&_clients[i], this))
			{
				checkCmd(&_clients[i], _clients[i]._dataCmd._cmd, this);
			}
			else
			{
				
			}
			if (i < _clients.size() && _clients[i]._fd == fd)
			{
				_clients[i]._dataCmd._message.clear();
			}
		}
	}
}

void	Server::initServer()
{
	int	status;
	int clientIndex;
	size_t	i;

	initSocket();
	std::cout << "Server started on port " << _port << std::endl;
	while (!this->_signal)
	{
		status = poll(&this->_pfds[0], this->_pfds.size(), -1);
		if (status == -1 && !this->_signal)
			throw std::runtime_error("Error: poll failed");
		i = 0;
		while (i < this->_pfds.size())
		{
			// std::cout << i << std::endl;
			if (this->_pfds[i].revents & POLLIN)	// Vérifie si des données sont disponibles en lecture
			{
				if (this->_pfds[i].fd == _socketFd)
				{
					newClient();
					std::cout << "New client connected" << std::endl;
				}
				else
				{
					clientIndex = getClientIndex(this->_pfds[i].fd);
					processData(this->_pfds[i].fd);
					if (clientIndex != -1)
						tryWelcome(&_clients[clientIndex]);
				}
			}
			i++;
		}
	}
	closeFd();
}
