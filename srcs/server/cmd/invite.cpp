/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:08:05 by moassi            #+#    #+#             */
/*   Updated: 2024/08/11 14:39:06 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::invite(std::string command, int fd)
{
    strVector args = splitCMD(command);
    std::string channelName;
    Channel *channel;
    
    if (args.size() < 3)
        { sendResponse(ERR_NOTENOUGHPARAM(getClient(fd)->getNickname()), fd); return ; }
    channelName = args[1];
    channel = getChannel(channelName);
    if (!channel)
        { sendResponse(ERR_NOSUCHCHANNEL(getClient(fd)->getNickname(), channelName), fd); return ; }
    
    if (!channel->getClient(fd) && !channel->getOperator(fd))
        { sendResponse(ERR_NOTONCHANNEL(getClient(fd)->getNickname(), channelName), fd); return ; }
    Client *invitedClient = getClientByName(args[2]);
    if (!invitedClient)
        { sendResponse(ERR_NOSUCHNICK(getClient(fd)->getNickname(), args[2]), fd); return ; }
    if (channel->getClient(invitedClient->getFd()) || channel->getOperator(invitedClient->getFd()))
        { sendResponse(ERR_USERONCHANNEL(getClient(fd)->getNickname(), invitedClient->getNickname(), channelName), fd); return ; }
    if (!channel->getInviteOnly() && !channel->getOperator(fd))
        { sendResponse(ERR_CHANOPRIVSNEEDED(getClient(fd)->getNickname(), channelName), fd); return ; }
    channel->addInvite(invitedClient->getFd());
    sendResponse(RPL_INVITING(getClient(fd)->getNickname(), invitedClient->getNickname(), channelName), fd);

    std::stringstream inviteMessage;
    inviteMessage << ":" << getClient(fd)->getNickname() << "!~" << getClient(fd)->getUsername() << "@" << getClient(fd)->getIpadd() << " INVITE " << invitedClient->getNickname() << " " << channel->getName() << "\r\n";
    sendResponse(inviteMessage.str(), invitedClient->getFd());
    // sendResponse(":42-IRC " + getClient(fd)->getNickname() + " INVITE " + invitedClient->getNickname() + " " + channelName + "\r\n", invitedClient->getFd());
}
