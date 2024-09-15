/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkanaan <hkanaan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 16:15:54 by moassi            #+#    #+#             */
/*   Updated: 2024/09/15 17:31:16 by hkanaan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::msgChannel(std::string channelName, std::string msg, int fd)
{
    Client *client = getClient(fd);
    Channel *channel = getChannel(channelName);
    if (!channel)
        { sendResponse(ERR_NOSUCHCHANNEL(client->getNickname(), channelName), fd); return ; }
    if (!channel->getClient(fd) && !channel->getOperator(fd))
        { sendResponse(ERR_NOTONCHANNEL(client->getNickname(), channelName), fd); return ; }
    
    
    // ss << "PRIVMSG " << channelName << " :" << msg << "\r\n";
    
    std::stringstream ss;
    // ss << ": " << client->getNickname() << " privmsg " << channelName;
    ss << ":" << client->getNickname() << " PRIVMSG " << channelName << " :" << msg << "\r\n";
    std::cout << ss.str() << std::endl;
    channel->sendResponseToallOthers(ss.str(), client->getFd());
}

void    Server::msgClient(std::string targetName, std::string msg, int fd)
{
    Client *client = getClient(fd);
    Client *target = getClientByName(targetName);
    if (!target)
        { sendResponse(ERR_NOSUCHNICK(client->getNickname(), targetName), fd); return ; }
    std::stringstream ss;
    ss << ":" << client->getNickname() << " PRIVMSG " << targetName << " :" << msg << "\r\n";

    sendResponse(ss.str(), target->getFd());
}

void    Server::privmsg(std::string command, int fd)
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
    strVector targets = splitOnCommas(args[1]);

    if (args.size() < 3)
        { sendResponse(ERR_NOTENOUGHPARAM(client->getNickname()), -1); return ; }
    if (args[2][0] == ':')
        args[2].erase(args[2].begin());
    for(size_t i = 0; i < targets.size(); i++)
    {
        if (targets[i][0] == '#')
            msgChannel(targets[i], args[2], fd);
        else
            msgClient(targets[i], args[2], fd);   
    }
}