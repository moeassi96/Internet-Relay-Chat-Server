/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 12:58:36 by moassi            #+#    #+#             */
/*   Updated: 2024/08/12 09:43:29 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::mode(std::string command, int fd)
{
    Client *client = getClient(fd);
    strVector args = splitCMD(command);
    
    if (args.size() < 2)
        { sendResponse(ERR_NOTENOUGHPARAM(client->getNickname()), fd); return ; }
        
    Channel *channel = getChannel(args[1]);
    if (!channel)
        { sendResponse(ERR_NOSUCHCHANNEL(client->getNickname(), args[1]), fd); return ; }
    if (!channel->getClient(fd) && !channel->getOperator(fd))
        { sendResponse(ERR_NOTONCHANNEL(client->getNickname(), args[1]), fd); return ; }
    
    if (args.size() == 2)
    {
        std::stringstream res;
        res << ":42-IRC 324 " << client->getNickname() << " " << channel->getName() << " ";
        if (channel->getInviteOnly())
            res << "+i ";
        else
            res << "-i ";
        if (channel->getLimit() != -1)
            res << "+l " << channel->getLimit() << " ";
        else
            res << "-l ";
        if (channel->getTopicRestriction())
            res << "+t ";
        else
            res << "-t ";
        if (channel->getKey())
            res << "+k " << channel->getPassword() << " ";
        else
            res << "-k ";
        sendResponse(res.str(), fd);
        return ;
    }
    
    if (!channel->getOperator(fd))
        { sendResponse(ERR_CHANOPRIVSNEEDED(client->getNickname(), args[1]), fd); return ; }
        
    std::string mode = args[2];
    std::stringstream res;
    res << ":42-IRC " << client->getNickname() << "!" << client->getUsername() << "@" << client->getIpadd() << " MODE " << channel->getName() << " ";
    if (mode == "+i")
    {
        channel->setInviteOnly(true);
        res << mode;
    }
    else if (mode == "-i")
    {
        channel->setInviteOnly(false);
        res << mode;
    }
    else if (mode == "+t")
    {
        channel->setTopicRestriction(true);
        res << mode;
    } 
    else if (mode == "-t")
    {
        channel->setTopicRestriction(false);
        res << mode;
    }
    else if (mode == "+k")
    {
        channel->setKey(true);
        if (args.size() < 4)
            { sendResponse(ERR_NOTENOUGHPARAM(std::string("MODE")), fd); return ; }
        channel->setPassword(args[3]);
        res << mode;
    }
    else if (mode == "-k")
    {
        channel->setKey(false);
        channel->setPassword("");
        res << mode;
    }
    else if (mode == "+o")
    {
        if (args.size() < 4)
            { sendResponse(ERR_NOTENOUGHPARAM(std::string("MODE")), fd); return ; }
            
        Client *user = getClientByName(args[3]);
        if (!user)
            { sendResponse(ERR_NOSUCHNICK(client->getNickname(), user->getNickname()), fd); return ; }
        if (!channel->getClient(user->getFd()) && !channel->getOperator(user->getFd()))
            { sendResponse(ERR_USERNOTINCHANNEL(client->getNickname(), user->getNickname(), channel->getName()), fd); return ; }
        if (!channel->getOperator(user->getFd()))
        {
            channel->makeOperator(user->getFd());
            res << mode << " " << user->getNickname();
        }
        else
            return ;
    }
    else if (mode == "-o")
    {
        if (args.size() < 4)
            { sendResponse(ERR_NOTENOUGHPARAM(std::string("MODE")), fd); return ; }
            
        Client *user = getClientByName(args[3]);
        if (!user)
            { sendResponse(ERR_NOSUCHNICK(client->getNickname(), user->getNickname()), fd); return ; }
        if (!channel->getClient(user->getFd()) && !channel->getOperator(user->getFd()))
            { sendResponse(ERR_USERNOTINCHANNEL(client->getNickname(), user->getNickname(), channel->getName()), fd); return ; }
        if (!channel->getClient(user->getFd()))
        {
            channel->makeClient(user->getFd());
            res << mode << " " << user->getNickname();
        }
        else
            return ;
    }
    else if (mode == "+l")
    {
        if (args.size() < 4)
            { sendResponse(ERR_NOTENOUGHPARAM(std::string("MODE")), fd); return ; }
        int limit;
        std::stringstream ss(args[3]);
        ss >> limit;
        if (ss.fail() || !ss.eof() || limit <= 0)
            { sendResponse(ERR_INVALIDMODEPARAM(client->getNickname(), channel->getName(), mode, args[3]), fd); return ; }
        channel->setLimit(limit);
        res << mode << " " << limit;
    }
    else if (mode == "-l")
    {
        channel->setLimit(-1);
        res << mode;
    }
    else
        { sendResponse(ERR_UNKNOWNMODE(client->getNickname(), mode), fd); return ; }
    
    res << "\r\n";
    channel->sendResponseToall(res.str());
}