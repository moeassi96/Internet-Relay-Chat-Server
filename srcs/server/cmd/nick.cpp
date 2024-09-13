/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:03:04 by moassi            #+#    #+#             */
/*   Updated: 2024/08/10 13:03:58 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool    Server::isUsedNickname(const std::string &nickname)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i].getNickname() == nickname)
            return (true);
    }
    return (false);
}

bool    Server::isValidNickname(const std::string &nickname)
{
    if (nickname.empty() || nickname[0] == ':' || nickname[0] == '&' || nickname[0] == '#')
        return (false);
    for (size_t i = 1; i < nickname.length(); i++)
    {
        if (!std::isalnum(nickname[i]) && nickname[i] != '_')
            return (false);
    }
    return (true);
}

void    Server::setNickname(std::string command, int fd)
{
    Client *client = getClient(fd);
    size_t startPos = command.find_first_not_of("\t ", 4);
    if (startPos == std::string::npos)
        startPos = command.length();
    command = command.substr(startPos);
        
    if (command.empty())
        sendResponse(ERR_NONICKNAMEGIVEN(client->getNickname()), fd);
    else if (client->getNickname() == command)
        return ;
    else if (!isValidNickname(command))
        sendResponse(ERR_ERRONEUSNICKNAME(client->getNickname()), fd);
    else if(isUsedNickname(command))
        sendResponse(ERR_NICKNAMEINUSE(client->getNickname()), fd);
    else if(!client->getIsRegistered())
        sendResponse(ERR_NOTREGISTERED(client->getNickname()), fd);
    else
    {
        if (!client->getNickname().empty())
            sendResponse(RPL_NICKCHANGED(client->getNickname(), command), fd);
        client->setNickname(command);
        if (client->getIsRegistered() && !client->getUsername().empty() && !client->getIsFullyRegistered())
        {
            client->setIsFullyRegistered(true);
            sendResponse(RPL_WELCOME(client->getNickname()), fd);
        }
    }
}
