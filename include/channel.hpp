/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dferjul <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:22:29 by dravaono          #+#    #+#             */
/*   Updated: 2025/02/25 04:16:08 by dferjul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "all.hpp"

class   Channel
{
	private:
	public:
		std::map<std::string, std::vector<Client*> > _Channel;
		std::map<std::string, std::vector<Client*> > _channelOperators;
		
		Channel();
		~Channel();
		void createChannel(std::string channel, Client *client);
		void leaveChannel(std::string channel, Client client);
		std::string getChannelUsers(std::string channel);
		bool channelExists(std::string channel);
		void removeClientFromChannel(const std::string &channelName, Client *client);
		void removeClientFromAllChannels(Client* client);
		void broadcastMessage(const std::string& channelName, const std::string& message, Client* sender);
		bool isOperator(const std::string &channel, Client* client);
		void addOperator(const std::string &channel, Client* client);
		void removeOperator(const std::string &channel, Client* client);
		Client *getClientFromChannel(std::string channel, std::string nickname);
};