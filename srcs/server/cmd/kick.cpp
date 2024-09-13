/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:07:18 by moassi            #+#    #+#             */
/*   Updated: 2024/08/10 13:08:56 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::kick(std::string command, int fd)
{
    strVector args;
    std::stringstream ss(command);
    std::string token;
    int count = 0;
    while (count < 3 && ss >> token)
    {
        args.push_back(token);
        count++;
    }
    std::string rest;
    if (ss >> std::ws && std::getline(ss, rest))
        args.push_back(rest);
    Client *client = getClient(fd);
    if (args.size() < 3)
        { sendResponse(ERR_NOTENOUGHPARAM(client->getNickname()), fd); return ; }
    Client *kickedClient = getClientByName(args[2]);
    if (!kickedClient)
        { sendResponse(ERR_NOSUCHNICK(getClient(fd)->getNickname(), args[2]), fd); return ; }
        
    strVector channels = splitOnCommas(args[1]);
    for (size_t i = 0; i < channels.size(); i++)
    {
        Channel *channel = getChannel(channels[i]);

        if (!channel)
        { sendResponse(ERR_NOSUCHCHANNEL(client->getNickname(), args[1]), fd); continue ; }
        if (!channel->getClient(fd) && !channel->getOperator(fd))
            { sendResponse(ERR_NOTONCHANNEL(client->getNickname(), channel->getName()), fd); continue ; }
        if (!channel->getOperator(fd))
            { sendResponse(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName()), fd); continue ; }
        if (!channel->getClient(kickedClient->getFd()) && !channel->getOperator(kickedClient->getFd()))
            { sendResponse(ERR_USERNOTINCHANNEL(client->getNickname(), kickedClient->getNickname() ,channel->getName()), fd); continue ; }
        
        std::stringstream ss;
        ss << ":42-IRC " << client->getNickname() << "!~" << client->getUsername() << "@" + client->getIpadd() + " KICK " << channel->getName() << " " << kickedClient->getNickname();
        if (args.size() > 3)
        {
            if (args[3][0] == ':')
                args[3].erase(args[3].begin());
            ss << " :" << args[3];
        }
        ss << "\r\n";
        channel->sendResponseToall(ss.str());
        channel->removeClient(kickedClient->getFd());
        channel->removeOperator(kickedClient->getFd());
    }
}
