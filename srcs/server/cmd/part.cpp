/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:05:04 by moassi            #+#    #+#             */
/*   Updated: 2024/08/11 15:32:08 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::part(std::string command, int fd)
{
    strVector args;
    std::stringstream ss(command);
    std::string token;
    int count = 0;
    while (count < 2 && ss >> token)
    {
        args.push_back(token);
        count++;
    }
    std::string rest;
    if (ss >> std::ws && std::getline(ss, rest))
        args.push_back(rest);
    Client *client = getClient(fd);
    if (args.size() < 2)
        { sendResponse(ERR_NOTENOUGHPARAM(client->getNickname()), fd); return ; }
        
    strVector channels = splitOnCommas(args[1]);
    for (size_t i = 0; i < channels.size(); i++)
    {
        Channel *channel = getChannel(channels[i]);
        if (!channel)
        { sendResponse(ERR_NOSUCHCHANNEL(client->getNickname(), args[1]), fd); continue ; }
        if (!channel->getClient(fd) && !channel->getOperator(fd))
            { sendResponse(ERR_NOTONCHANNEL(client->getNickname(), channel->getName()), fd); continue ; }
        
        std::stringstream ss;
        ss << ":" << client->getNickname() << "!~" << client->getUsername() << "@" + client->getIpadd() + " PART " << channel->getName();
        if (args.size() > 2)
        {
            if (args[2][0] == ':')
                args[2].erase(args[2].begin());
            ss << " :" << args[2];
        }
        ss << "\r\n";
        channel->sendResponseToall(ss.str());
        channel->removeClient(client->getFd());
        channel->removeOperator(client->getFd());
        if (channel->getClientNum() == 0 && channel->getOperatorNum() == 0)
            { removeChannel(channel->getName()); continue ; }
        if (channel->getOperatorNum() == 0) 
            channel->assignNewOperator();
    }
}
