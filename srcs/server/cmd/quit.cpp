/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:09:12 by moassi            #+#    #+#             */
/*   Updated: 2024/08/10 13:09:39 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void            Server::quit(std::string command, int fd)
{
    strVector args;
    std::stringstream ss(command);
    std::string token;
    int count = 0;
    while (count < 1 && ss >> token)
    {
        args.push_back(token);
        count++;
    }
    std::string rest;
    if (ss >> std::ws && std::getline(ss, rest))
        args.push_back(rest);
        
    Client *client = getClient(fd);
    std::stringstream res;
    res << ":42-IRC " << client->getNickname() << "!~" << client->getUsername() << "@" + client->getIpadd() + " QUIT";
    if (args.size() > 1 && !args[1].empty())
        res << " :" << args[1];
    res << "\r\n";
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i].getClient(fd))
        {
            _channels[i].sendResponseToallOthers(res.str(), fd);
            _channels[i].removeClient(fd);
            if (_channels[i].getClientNum() + _channels[i].getOperatorNum() == 0)
            {
                _channels.erase(_channels.begin() + i);
                i--;
            }    
        }
        else if (_channels[i].getOperator(fd))
        {
            _channels[i].sendResponseToallOthers(res.str(), fd);
            _channels[i].removeOperator(fd);
            if (_channels[i].getClientNum() + _channels[i].getOperatorNum() == 0)
            {
                _channels.erase(_channels.begin() + i);
                i--;
            }
            else if (_channels[i].getOperatorNum() == 0)
            {
                _channels[i].assignNewOperator();
            }  
        }
    }
    std::cout << "Client <" << fd << "> Disconnected" << std::endl;
	removeClient(fd);
	removeFd(fd);
	close(fd);
}
