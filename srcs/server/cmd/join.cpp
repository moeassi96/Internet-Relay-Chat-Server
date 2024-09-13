/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 12:58:36 by moassi            #+#    #+#             */
/*   Updated: 2024/08/11 15:59:04 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::newChannel(strVector args, int fd)
{
    Client *client = getClient(fd);
    if (countClientinChannels(fd) >= 10)
        { std::cout << countClientinChannels(fd) << std::endl; sendResponse(ERR_TOOMANYCHANNELS(client->getNickname(), args[0]), fd); return ; }
    
    Channel channel;
    channel.setName(args[0]);
    channel.addOperator(*client);
    _channels.push_back(channel);

    std::stringstream response;
    response << ":" << client->getNickname() << "!~" << client->getUsername() << "@" << client->getIpadd() << " JOIN " << channel.getName() << "\r\n";
    channel.sendResponseToall(response.str());
    sendResponse(RPL_NAMREPLY(client->getNickname(), channel.getName(), channel.getClientNames()), fd);
    sendResponse(RPL_ENDOFNAMES(client->getNickname(), channel.getName()), fd);
}

void    Server::joinChannel(strVector args, int fd)
{
    Channel *channel = getChannel(args[0]);
    Client *client = getClient(fd);
    
    if (channel->getClient(fd))
        return ;
    if (countClientinChannels(fd) >= 10)
        { sendResponse(ERR_TOOMANYCHANNELS(client->getNickname(), channel->getName()), fd); return ; }
    if (channel->getInviteOnly() && !channel->verifyInvitation(fd))
        sendResponse(ERR_INVITEONLYCHAN(client->getNickname(), channel->getName()), fd);
    else if (channel->getKey() && ( args.size() <= 1 || args[1] != channel->getPassword()))
        sendResponse(ERR_BADCHANNELKEY(client->getNickname(), channel->getName()), fd);
    else if (channel->getLimit() != -1 && channel->getClientNum() >= (size_t)channel->getLimit())
        sendResponse(ERR_CHANNELISFULL(client->getNickname(), channel->getName()), fd);
    else
    {
        channel->addClient(*client);
        channel->removeInvite(client->getFd());
        std::stringstream response;
        response << ":" << client->getNickname() << "!~" << client->getUsername() << "@" << client->getIpadd() << " JOIN " << channel->getName() << "\r\n";
        if (!channel->getTopic().empty())
            response << RPL_TOPIC(client->getNickname(), channel->getName(), channel->getTopic());
        response << RPL_NAMREPLY(client->getNickname(), channel->getName(), channel->getClientNames());
        response << RPL_ENDOFNAMES(client->getNickname(), channel->getName());
        sendResponse(response.str(), fd);
        std::stringstream cresponse;
        cresponse << ":" << client->getNickname() << "!~" << client->getUsername() << "@" << client->getIpadd() << " JOIN " << channel->getName() << "\r\n";
        channel->sendResponseToallOthers(cresponse.str(), fd);
    }
}

void    Server::join(std::string command, int fd)
{
    Client *client = getClient(fd);
    size_t startPos = command.find_first_not_of("\t ", 4);
    if (startPos == std::string::npos)
        startPos = command.length();
    command = command.substr(startPos);
    if (command.empty())
        { sendResponse(ERR_NOTENOUGHPARAM(client->getNickname()), fd); return ; }

    strVector joinTokens = splitOnCommas(command);
    for (size_t i = 0; i < joinTokens.size(); i++)
    {
        strVector token_args = splitCMD(joinTokens[i]);
        if (token_args[0][0] != '#') // wrong channel name
            { sendResponse(ERR_NOSUCHCHANNEL(client->getNickname(), token_args[0]), fd); continue; }
        if (!getChannel(token_args[0])) // if channel does not exit
            { newChannel(token_args, fd); continue ; }
        joinChannel(token_args, fd);
    }
}