/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auth.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:01:48 by moassi            #+#    #+#             */
/*   Updated: 2024/08/10 13:04:13 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::authenticateUser(std::string command, int fd)
{
    Client *client = getClient(fd);
    size_t startPos = command.find_first_not_of("\t ", 4);
    if (startPos == std::string::npos)
        startPos = command.length();
    command = command.substr(startPos);
    
    if (command.empty())
        sendResponse(ERR_NOTENOUGHPARAM(client->getNickname()), fd);
    else if (client->getIsRegistered())
        sendResponse(ERR_ALREADYREGISTERED(client->getNickname()), fd);
    else if (command != _password)
    {
        std::cout << command << " " << _password << std::endl;
        sendResponse(ERR_PASSWDMISMATCH(client->getNickname()), fd);
    }
    else
        client->setIsRegistered(true);
}
