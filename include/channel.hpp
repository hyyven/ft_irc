/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:22:29 by dravaono          #+#    #+#             */
/*   Updated: 2025/02/06 16:37:42 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "all.hpp"

class   Channel
{
	private:
		std::map<std::string, std::vector<Client*> > _Channel;
	public:
		Channel();
		~Channel();
		void createChannel(std::string channel, Client *client);
		void leaveChannel(std::string channel, Client client);
		std::string getChannelUsers(std::string channel);
		bool channelExists(std::string channel);
		std::vector<Client*> getChannelClients(std::string channel);
		void removeClientFromChannel(const std::string &channelName, Client *client);
		void broadcastMessage(const std::string& channelName, const std::string& message, Client* sender);
};