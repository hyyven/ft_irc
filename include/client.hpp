/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:25:28 by afont             #+#    #+#             */
/*   Updated: 2025/01/10 11:25:28 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "all.hpp"

class	Client
{
	private:
	public:
		bool		_isRegistered;
		bool		_isWelcomed;
		int			_fd;
		std::string	_ip;
		std::string	_nickname;
		std::string	_username;
		
		Client();
		~Client();
		void			sendWelcome();
		void			setCliValue(std::string buf);
		void			sendMessage(const std::string& message) const;
		void			sendMessageToChannel(const std::string& message, const std::vector<Client>& clients);
};
