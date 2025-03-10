/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 12:30:46 by afont             #+#    #+#             */
/*   Updated: 2025/03/10 14:16:59 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/all.hpp"

void	Server::botSendMessage(std::string message)
{
	if (_botClientFd != -1)
	{
		message += "\r\n";
		if (send(_botClientFd, message.c_str(), message.length(), 0) == -1)
			std::cerr << "send() bot failed" << std::endl;
	}
}

void	Server::createBotClient()
{
	struct	pollfd pfd;
	Client	bot;
	int		sockets[2];
	int		status;

	status = socketpair(AF_UNIX, SOCK_STREAM, 0, sockets);
	if(status == -1)
	{
		std::cerr << "socketpair() bot failed" << std::endl;
		return;
	}
	status = fcntl(sockets[0], F_SETFL, O_NONBLOCK);
	status += fcntl(sockets[1], F_SETFL, O_NONBLOCK);
	if(status != 0)
	{
		std::cerr << "fcntl() bot failed" << std::endl;
		close(sockets[0]);
		close(sockets[1]);
		return;
	}
	pfd.fd = sockets[0];
	pfd.events = POLLIN;
	pfd.revents = 0;
	bot._fd = sockets[0];
	bot._nickname = "Bot";
	bot._username = "Bot___";
	bot._isRegistered = true;
	bot._isWelcomed = true;
	bot._ip = "127.0.0.1";
	bot._lastActivity = time(NULL);
	bot._isBot = true;
	_botClientFd = sockets[1];
	_clients[sockets[0]] = bot;
	_pfds.push_back(pfd);
	std::cout << "Bot client connected" << std::endl;
	botSendMessage("JOIN #bot");
	botSendMessage("TOPIC #bot :Welcome to the Bot Channel! Type !help for commands");
}

std::string getWeather()
{
	std::stringstream ss;
	const std::string conditions[] = {"Sunny", "Cloudy", "Partly Cloudy", "Rainy", "Thunderstorms", "Snowy", "Windy", "Foggy"};
	const std::string locations[] = {"Paris", "New York", "Tokyo", "London", "Sydney", "Moscow", "Cape Town", "Rio de Janeiro"};
	
	srand(time(NULL));
	int conditionIndex = rand() % 8;
	int locationIndex = rand() % 8;
	int temperature = rand() % 40 - 10;
	ss << "Weather in " << locations[locationIndex] << ": " << conditions[conditionIndex] << ", " << temperature << "°C";
	return (ss.str());
}

void	processBotCommand(std::string message, Server *server)
{
	Client *botClient = &server->_clients.begin()->second;
	if (message.empty() || message[0] != '!')
		return;
	if (message == "!help")
	{
		std::string formattedMessage = createFormattedMessage(botClient, "PRIVMSG", "#bot :Available commands: !help, !time, !weather");
		server->_channelManager.broadcastMessage("#bot", formattedMessage, botClient);
	}
	else if (message == "!time")
	{
		time_t now = time(NULL);
		std::string timestr = ctime(&now);
		std::string formattedMessage = createFormattedMessage(botClient, "PRIVMSG", "#bot :Current time: " + timestr);
		server->_channelManager.broadcastMessage("#bot", formattedMessage, botClient);
	}
	else if (message == "!weather")
	{
		std::string weather = getWeather();
		std::string formattedMessage = createFormattedMessage(botClient, "PRIVMSG", "#bot :" + weather);
		server->_channelManager.broadcastMessage("#bot", formattedMessage, botClient);
	}
}