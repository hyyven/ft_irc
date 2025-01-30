/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afont <afont@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:55:43 by afont             #+#    #+#             */
/*   Updated: 2025/01/30 17:49:51 by afont            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/all.hpp"

void	parser(int ac, char **av)
{
	std::string port;
	std::string::const_iterator it;

	if (ac != 2)
	{
		std::cout << "Usage: " << av[0] << " <port>" << std::endl;
		exit(1);
	}
	port = av[1];
	it = port.begin();
	while (it != port.end() && std::isdigit(*it))
		++it;
	if (!port.empty() && it == port.end())
	{
		int portNumber = std::atoi(port.c_str());
		if (portNumber < 49152 || portNumber > 65535)
		{
			std::cout << "Port must be between 49152 and 65535" << std::endl;
			exit(1);
		}
	}
	else
	{
		std::cout << "Invalid port" << std::endl;
		exit(1);
	}
}

std::vector<std::string> split(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    // std::cout << "Splitting" << std::endl;
    while (std::getline(ss, token, delimiter))
    {
        // std::cout << token << "|";
        tokens.push_back(token);
    }
    // std::cout << std::endl;
	// std::cout << "End of split" << std::endl;
    return tokens;
}

int	parserCmd(t_cmd *dataCmd, std::string buf)
{
	static std::string	last_buf;

	if (buf.find('\r') == 0)
		last_buf = buf;
	else if (last_buf.find('\r') == 0 && buf.find('\n') == 0)
	{
		dataCmd->_cmd = split(dataCmd->_message, ' ');
		return (1);
	}
	else
	{
    	dataCmd->_message += buf;
	}
	return (0);
}


void	printvector(std::vector<std::string> vec)
{
	std::vector<std::string>::iterator it;

	it = vec.begin();
	std::cout << "Vector: ";
	while (it != vec.end())
	{
		std::cout << *it << "|";
		it++;
	}
	std::cout << std::endl;
}
