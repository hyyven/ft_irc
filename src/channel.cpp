/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dferjul <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:27:39 by dravaono          #+#    #+#             */
/*   Updated: 2025/02/08 21:15:30 by dferjul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/all.hpp"

Channel::Channel()
{
}

Channel::~Channel()
{
}

void Channel::createChannel(std::string channelName, Client *client)
{
	_Channel[channelName].push_back(client);
}

std::string Channel::getChannelUsers(std::string channel)
{
	std::string users;
	
	if (channelExists(channel))
	{
		for (size_t i = 0; i < _Channel[channel].size(); i++)
		{
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
			if (_Channel[channelName][i]->_fd == client->_fd)
			{
				_Channel[channelName].erase(_Channel[channelName].begin() + i);
				std::cout << "Client " << client->_nickname << " removed from channel " << channelName << std::endl;
				break;
			}
		}
		/* if (_Channel[channelName].empty())
		{
			_Channel.erase(channelName);
			std::cout << "Channel " << channelName << " deleted" << std::endl;
		} */
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
