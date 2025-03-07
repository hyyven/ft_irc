/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:27:39 by dravaono          #+#    #+#             */
/*   Updated: 2025/03/07 18:56:18 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/all.hpp"

Channel::Channel() {}

Channel::~Channel() {}

void Channel::createChannel(std::string channelName, Client *client)
{
	_Channel[channelName].push_back(client);
	_channelOperators[channelName].push_back(client);
	_channelTopics[channelName] = "";
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

Client *Channel::getClientFromChannel(std::string channel, std::string nickname)
{
	if (channelExists(channel))
	{
		for (size_t i = 0; i < _Channel[channel].size(); i++)
		{
			if (_Channel[channel][i]->_nickname == nickname)
				return _Channel[channel][i];
		}
	}
	return (NULL);
}

bool Channel::isInviteOnly(const std::string &channel)
{
	return _inviteOnlyChannels.find(channel) != _inviteOnlyChannels.end();
}

void Channel::setInviteOnly(const std::string &channel, bool inviteOnly)
{
	if (inviteOnly)
		_inviteOnlyChannels.insert(channel);
	else
		_inviteOnlyChannels.erase(channel);
}

bool Channel::isInvited(const std::string &channel, const std::string &nickname)
{
	if (_invitedUsers.find(channel) == _invitedUsers.end())
		return false;
		
	std::vector<std::string> &invitedUsers = _invitedUsers[channel];
	return std::find(invitedUsers.begin(), invitedUsers.end(), nickname) != invitedUsers.end();
}

void Channel::inviteUser(const std::string &channel, const std::string &nickname)
{
	_invitedUsers[channel].push_back(nickname);
}

std::string Channel::getChannelTopic(const std::string &channel)
{
	if (_channelTopics.find(channel) != _channelTopics.end())
		return _channelTopics[channel];
	return "";
}

void Channel::setChannelTopic(const std::string &channel, const std::string &topic)
{
	_channelTopics[channel] = topic;
}

bool Channel::isTopicRestricted(const std::string &channel)
{
	return _topicRestrictedChannels.find(channel) != _topicRestrictedChannels.end();
}

void Channel::setTopicRestriction(const std::string &channel, bool restricted)
{
	if (restricted)
		_topicRestrictedChannels.insert(channel);
	else
		_topicRestrictedChannels.erase(channel);
}

bool Channel::hasPassword(const std::string &channel)
{
	return _channelPasswords.find(channel) != _channelPasswords.end();
}

void Channel::setChannelPassword(const std::string &channel, const std::string &password)
{
	_channelPasswords[channel] = password;
}

void Channel::removeChannelPassword(const std::string &channel)
{
	_channelPasswords.erase(channel);
}

bool Channel::checkChannelPassword(const std::string &channel, const std::string &password)
{
	if (!hasPassword(channel))
		return true;
	return _channelPasswords[channel] == password;
}

bool Channel::hasUserLimit(const std::string &channel)
{
	return _channelLimits.find(channel) != _channelLimits.end();
}

size_t Channel::getUserLimit(const std::string &channel)
{
	if (hasUserLimit(channel))
		return _channelLimits[channel];
	return 0;
}

void Channel::setUserLimit(const std::string &channel, size_t limit)
{
	_channelLimits[channel] = limit;
}

void Channel::removeUserLimit(const std::string &channel)
{
	_channelLimits.erase(channel);
}

bool Channel::isChannelFull(const std::string &channel)
{
	if (!hasUserLimit(channel))
		return false;
	return _Channel[channel].size() >= _channelLimits[channel];
}