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
		int			_fd;
		int			_nbMess;
		std::string	_ip;
		char		_service[NI_MAXSERV];
		std::string	_nickname;
		std::string	_username;
		
		Client();
		~Client();
		void			sendWelcome(int cli_fd);
		void			setCliValue(std::string buf);
};
