/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dravaono <dravaono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:22:29 by dravaono          #+#    #+#             */
/*   Updated: 2025/01/28 13:42:11 by dravaono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include "../include/client.hpp"
#include "../include/server.hpp"

class   Channel
{
	private:
		std::map<std::string, std::vector<Client> > _Channel;
	public:
		Channel();
		~Channel();
		void createChannel(std::string channel, Client client);
		void leaveChannel(std::string channel, Client client);
};