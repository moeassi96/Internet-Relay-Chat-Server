/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:05:14 by moassi            #+#    #+#             */
/*   Updated: 2024/08/11 16:00:07 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::topic(std::string command, int fd)
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
    Channel *channel = getChannel(args[1]);
    if (args.size() < 2)
        { sendResponse(ERR_NOTENOUGHPARAM(client->getNickname()), fd); return ;}
    if (!channel)
        { sendResponse(ERR_NOSUCHCHANNEL(client->getNickname(), args[1]), fd); return ;}
    if (!channel->getClient(fd) && !channel->getOperator(fd))
        { sendResponse(ERR_NOTONCHANNEL(client->getNickname(), channel->getName()), fd); return ;}
    if (channel->getTopicRestriction() && !channel->getOperator(fd) && args.size() > 2)
        { sendResponse(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName()), fd); return ;}
    if (args.size() == 2)
    {
        if (!channel->getTopic().length())
            sendResponse(RPL_NOTOPIC(client->getNickname(), channel->getName()), fd);
        else
        {
            sendResponse(RPL_TOPIC(client->getNickname(), channel->getName(), channel->getTopic()), fd);
            sendResponse(RPL_TOPICWHOTIME(client->getNickname(), channel->getName(), channel->getTopicOwner(), channel->getTopicTime()), fd); 
        }
        return ; 
    }
    
    std::string newTopic = args[2];
    if (newTopic[0] == ':')
        newTopic.erase(newTopic.begin());
    channel->setTopic(newTopic);
    channel->setTopicOwner(client->getNickname());
    channel->setTopicTime();
    std::stringstream res;
    res << ":42-IRC " << client->getNickname() << "!~" << client->getUsername() << "@" + client->getIpadd() + " TOPIC " << channel->getName() << " :" << newTopic << "\r\n"; 
    channel->sendResponseToall(res.str());
}
