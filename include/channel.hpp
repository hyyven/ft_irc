/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dferjul <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:22:29 by dravaono          #+#    #+#             */
/*   Updated: 2025/02/26 03:18:31 by dferjul          ###   ########.fr       */
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
		std::set<std::string> _inviteOnlyChannels;
		std::map<std::string, std::vector<std::string> > _invitedUsers;
		
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

		bool isInviteOnly(const std::string &channel);
		void setInviteOnly(const std::string &channel, bool inviteOnly);
		bool isInvited(const std::string &channel, const std::string &nickname);
		void inviteUser(const std::string &channel, const std::string &nickname);
};