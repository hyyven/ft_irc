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
#include <iostream>
#include <csignal>
#include <poll.h>
// #include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <cstring>

class	Client
{
	private:
		int			_fd;
		std::string	_ip;
	public:
		Client();
		~Client();
		int				getFd();
		void			setFd(int fd);
		void			setIp(std::string ip);
		std::string		getIp();
};