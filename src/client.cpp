/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 13:34:47 by afont             #+#    #+#             */
/*   Updated: 2025/02/06 16:05:20 by afont            ###   ########.fr       */
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
			std::cout << "send() failed welcome" << std::endl;
		messages.erase(messages.begin());
	}
}

void Client::sendMessage(const std::string& message) const
{
	if (send(_fd, message.c_str(), message.length(), 0) == -1)
		std::cout << "send() failed to fd: " << _fd << "with message: " << message << std::endl;
}

void Client::sendMessageToChannel(const std::string& message, const std::vector<Client*>& clients)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i]->_fd != _fd)
			clients[i]->sendMessage(message);
	}
}