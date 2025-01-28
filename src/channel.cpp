/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dravaono <dravaono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:27:39 by dravaono          #+#    #+#             */
/*   Updated: 2025/01/28 14:49:55 by dravaono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/channel.hpp"

Channel::Channel() {}

Channel::~Channel() {}

void Channel::createChannel(std::string channelName, Client client)
{
	// std::vector<Client> clientInChannel;
	if (_Channel.find(channelName) != _Channel.end())
		return;
	_Channel[channelName].push_back(client);
}