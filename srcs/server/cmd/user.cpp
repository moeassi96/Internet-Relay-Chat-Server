/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:14:32 by moassi            #+#    #+#             */
/*   Updated: 2024/08/10 13:15:35 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::setUsername(std::string command, int fd)
{
    Client *client = getClient(fd);
    size_t startPos = command.find_first_not_of("\t ", 4);
    if (startPos == std::string::npos)
        startPos = command.length();
    command = command.substr(startPos);
    strVector split = splitCMD(command);
    if (split.size() < 4)
        sendResponse(ERR_NOTENOUGHPARAM(client->getNickname()), fd);
    else if (!client->getIsRegistered())
        sendResponse(ERR_NOTREGISTERED(client->getNickname()), fd);
    else if (!client->getUsername().empty())
        sendResponse(ERR_ALREADYREGISTERED(client->getNickname()), fd);
    else
    {
        client->setUsername(split[0]);
        if (client->getIsRegistered() && !client->getNickname().empty())
        {
            client->setIsFullyRegistered(true);
            sendResponse(RPL_WELCOME(client->getNickname()), fd);
        }
    }   
}
