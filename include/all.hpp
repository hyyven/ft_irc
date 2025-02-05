/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dferjul <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:04:06 by afont             #+#    #+#             */
/*   Updated: 2025/02/05 14:37:06 by dferjul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#define SERVER_NAME "ft_irc"
#define VERSION "ft_irc-1.0"


#include <string>
#include <sstream>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <algorithm>
#include <iostream>
#include <csignal>
#include <poll.h>
#include <sys/socket.h>
#include <vector>
#include <cstdlib>
#include <map>
#include <unistd.h>
#include <errno.h>

class Server;
class Client;
class Channel;

#include "client.hpp"
#include "cmd.hpp"
#include "channel.hpp"
#include "server.hpp"
#include "utils.hpp"