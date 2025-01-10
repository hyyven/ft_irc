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

class	Server
{
	private:
		int							_port;
		int							_socketFd;
		static bool					_signal;
		std::vector<struct pollfd>	_pfds;
	public:
		Server();
		~Server();
		int			getPort();
		void		setPort(int port);
		void		initServer();
		void		initSocket();
		static void	signalHandler(int signum);
};