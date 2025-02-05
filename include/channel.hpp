/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dferjul <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:22:29 by dravaono          #+#    #+#             */
/*   Updated: 2025/02/05 00:57:27 by dferjul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "all.hpp"

class   Channel
{
	private:
		std::map<std::string, std::vector<Client> > _Channel;
	public:
		Channel();
		~Channel();
		void createChannel(std::string channel, Client client);
		void leaveChannel(std::string channel, Client client);
		std::string getChannelUsers(std::string channel);
		bool channelExists(std::string channel);
		std::vector<Client> getChannelClients(std::string channel);
};