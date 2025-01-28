/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:21:51 by afont             #+#    #+#             */
/*   Updated: 2025/01/10 11:21:51 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include "client.hpp"

class	Server
{
	private:
		int							_port;
		int							_socketFd;
		static bool					_signal;
		std::vector<struct pollfd>	_pfds;
		std::vector<Client>			_clients;
	public:
		Server();
		~Server();
		int			getPort();
		void		setPort(int port);
		int			getFd();
		void		setFd(int fd);
		void		initServer();
		void		initSocket();
		void		closeFd();
		void		newClient();
		void		processData(int fd);
		void		removeClient(int fd);
		static void	signalHandler(int signum);
};