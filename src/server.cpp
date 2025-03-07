/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:32:15 by afont             #+#    #+#             */
/*   Updated: 2025/03/07 03:31:42 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/all.hpp"


bool Server::_signal = false;
Server::Server()
{
	_lastConnectionCheck = time(NULL);
}

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
	while (i < _pfds.size())
	{
		// std::cout << "\\" << std::endl;
		if (_pfds[i].fd == fd)
		{
			_pfds.erase(_pfds.begin() + i);
			break;
		}
		i++;
	}
	_clients.erase(fd);
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
	if (_clients.find(fd) != _clients.end())
			return (fd);
	return (-1);
}

void	Server::processData(int fd)
{
	std::string buf(1, 0);
	size_t	bytes;

	if (_clients.find(fd) == _clients.end())
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
		if (parserCmd(&_clients[fd], buf) == 1) //si c'est la fin du message
		{
			std::cout << "Message: [" << _clients[fd]._dataCmd._message << "]" << std::endl;
			// std::cout << "i: " << i << std::endl;
			// printvector(dataCmd->_cmd);
			if (initCliValue(&_clients[fd], this))
			{
				checkCmd(&_clients[fd], _clients[fd]._dataCmd._cmd, this);
			}
			if (_clients.find(fd) != _clients.end() && _clients[fd]._fd == fd)
			{
				//a tester
				_clients[fd]._dataCmd._message.clear();
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
		pingPong();
		status = poll(&this->_pfds[0], this->_pfds.size(), 1000);
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
					processData(this->_pfds[i].fd);
					clientIndex = getClientIndex(this->_pfds[i].fd);
					if (clientIndex != -1)
						tryWelcome(&_clients[clientIndex]);
				}
			}
			i++;
		}
	}
	closeFd();
}

void	Server::pingPong()
{
	size_t				i;
	time_t				currentTime;
	std::vector<int>	clientsToRemove;
	
	currentTime = time(NULL);
	// std::cout << "wait:" << currentTime - _lastConnectionCheck << std::endl;
	if (currentTime - _lastConnectionCheck < 10) //grande valeur, a changer
		return;
	// std::cout << "Checking clients" << std::endl;
	_lastConnectionCheck = currentTime;
	std::map<int, Client>::iterator it = _clients.begin();
	while (it != _clients.end())
	{
		// std::cout << "current time - last activity:" << currentTime - it->second._lastActivity << std::endl;
		if (it->second._isWaitingPong)
		{
			std::cout << "Client " << it->second._nickname << " timed out" << std::endl;
			clientsToRemove.push_back(it->first);
		}
		else if (currentTime - it->second._lastActivity > 10)
		{
			std::string message = "PING :server\r\n";
			it->second.sendMessage(message);
			it->second._lastPingSent = currentTime;
			it->second._isWaitingPong = true;
		}
		it++;
	}
	i = 0;
	while (i < clientsToRemove.size())
	{
		// std::cout << "Client " << _clients[clientsToRemove[i]]._nickname << " disconnected" << std::endl;
		close(clientsToRemove[i]);
		removeClient(clientsToRemove[i]);
		i++;
	}
}