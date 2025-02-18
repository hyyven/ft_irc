/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dferjul <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:27:39 by dravaono          #+#    #+#             */
/*   Updated: 2025/02/19 00:52:16 by dferjul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/all.hpp"

Channel::Channel() {}

Channel::~Channel() {}

void Channel::createChannel(std::string channelName, Client *client)
{
	_Channel[channelName].push_back(client);
	_channelOperators[channelName].push_back(client);
	// std::cout << "Channel " << channelName << " created by " << client->_nickname << " (operator)" << std::endl;
}

std::string Channel::getChannelUsers(std::string channel)
{
	std::string users;
	
	if (channelExists(channel))
	{
		for (size_t i = 0; i < _Channel[channel].size(); i++)
		{
			if (isOperator(channel, _Channel[channel][i]))
				users += "@";
			users += _Channel[channel][i]->_nickname;
			if (i < _Channel[channel].size() - 1)
				users += " ";
		}
	}
	return users;
}

bool Channel::channelExists(std::string channel)
{
	return _Channel.find(channel) != _Channel.end();
}

void Channel::removeClientFromChannel(const std::string &channelName, Client *client)
{
	if (channelExists(channelName))
	{
		for (size_t i = 0; i < _Channel[channelName].size(); i++)
		{
			if (_Channel[channelName][i]->_nickname == client->_nickname)
			{
				_Channel[channelName].erase(_Channel[channelName].begin() + i);
				std::cout << "Client " << client->_nickname << " removed from channel " << channelName << std::endl;
				break;
			}
		}
		if (_Channel[channelName].empty())
		{
			_Channel.erase(channelName);
			std::cout << "Channel " << channelName << " deleted" << std::endl;
		}
	}
}

void Channel::removeClientFromAllChannels(Client* client)
{
	std::string quitMessage = ":" + client->_nickname + "!" + client->_username + "@" + client->_ip + " QUIT :Leaving\r\n";
	
	std::map<std::string, std::vector<Client*> >::iterator it = _Channel.begin();
	while (it != _Channel.end())
	{
		for (size_t j = 0; j < it->second.size(); j++)
		{
			if (it->second[j] == client)
			{
				client->sendMessageToChannel(quitMessage, it->second);
				it->second.erase(it->second.begin() + j);
				break;
			}
		}
		++it;
	}
}

void Channel::broadcastMessage(const std::string& channelName, const std::string& message, Client* sender)
{
	if (channelExists(channelName))
	{
		for (size_t i = 0; i < _Channel[channelName].size(); i++)
		{
			if (_Channel[channelName][i]->_fd != sender->_fd)
				_Channel[channelName][i]->sendMessage(message);
		}
	}
}

bool Channel::isOperator(const std::string &channel, Client* client)
{
	if (_channelOperators.find(channel) != _channelOperators.end())
	{
		for (size_t i = 0; i < _channelOperators[channel].size(); i++)
			if (_channelOperators[channel][i] == client)
				return true;
	}
	return false;
}

void Channel::addOperator(const std::string &channel, Client* client)
{
	_channelOperators[channel].push_back(client);
}

void Channel::removeOperator(const std::string &channel, Client* client)
{
	for (size_t i = 0; i < _channelOperators[channel].size(); i++)
	{
		if (_channelOperators[channel][i] == client)
		{
			_channelOperators[channel].erase(_channelOperators[channel].begin() + i);
			break;
		}
	}
}