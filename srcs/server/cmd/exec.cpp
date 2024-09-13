/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 12:48:55 by moassi            #+#    #+#             */
/*   Updated: 2024/08/13 10:27:00 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::execCommand(std::string &command, int fd)
{
    strVector cmd_args = splitCMD(command);
    if (cmd_args.size() == 0)
        return ;
    if (cmd_args[0] == "CAP" && cmd_args[1] == "LS")
        return ;
    else if (cmd_args[0] == "PASS" || cmd_args[0] == "pass")
        authenticateUser(command, fd);
    else if (cmd_args[0] == "NICK" || cmd_args[0] == "nick")
        setNickname(command, fd);
    else if (cmd_args[0] == "USER" || cmd_args[0] == "user")
        setUsername(command, fd);
    else if (getClient(fd)->getIsFullyRegistered())
    {
        if (cmd_args[0] == "JOIN" || cmd_args[0] == "join")
            join(command, fd);
        else if (cmd_args[0] == "INVITE" || cmd_args[0] == "invite")
            invite(command, fd);
        else if (cmd_args[0] == "KICK" || cmd_args[0] == "kick")
            kick(command, fd);
        else if (cmd_args[0] == "PART" || cmd_args[0] == "part")
            part(command, fd);
        else if (cmd_args[0] == "TOPIC" || cmd_args[0] == "topic")
            topic(command, fd);
        else if (cmd_args[0] == "QUIT" || cmd_args[0] == "quit")
            quit(command, fd);
        else if (cmd_args[0] == "MODE" || cmd_args[0] == "mode")
            mode(command, fd);
        else if (cmd_args[0] == "PRIVMSG" || cmd_args[0] == "privmsg")
            privmsg(command, fd);
    }
    else
        sendResponse(ERR_NOTREGISTERED(getClient(fd)->getNickname()), fd);
}
