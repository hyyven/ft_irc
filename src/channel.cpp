/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dferjul <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:27:39 by dravaono          #+#    #+#             */
/*   Updated: 2025/02/05 02:10:45 by dferjul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/all.hpp"

Channel::Channel() {}

Channel::~Channel() {}

void Channel::createChannel(std::string channelName, Client client)
{
	// if (!channelExists(channelName))
	if (_Channel.find(channelName) == _Channel.end())
	{
		_Channel[channelName].push_back(client);
		// std::vector<Client> newChannel;
		// newChannel.push_back(client);
		// _Channel[channelName] = newChannel;
	}
	else
	{
		_Channel[channelName].push_back(client);
	}
}

std::string Channel::getChannelUsers(std::string channel)
{
	std::string users;
	
	if (_Channel.find(channel) != _Channel.end())
	{
		for (size_t i = 0; i < _Channel[channel].size(); i++)
		{
			users += _Channel[channel][i]._nickname;
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

std::vector<Client> Channel::getChannelClients(std::string channel)
{
	if (_Channel.find(channel) != _Channel.end())
		return _Channel[channel];
	return std::vector<Client>();
}
