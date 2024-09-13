/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msgs.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:11:27 by moassi            #+#    #+#             */
/*   Updated: 2024/08/11 15:46:32 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_MSGS_H
#define ERROR_MSGS_H

#define ENDL "\r\n"

#define ERR_NOSUCHNICK(nick, nick2) ":42-IRC 401 " + nick + " " + nick2 + " :No such nick/channel" + ENDL
#define ERR_NOSUCHCHANNEL(nick, chan) ":42-IRC 403 " + nick + " " + chan + " :No such channel" + ENDL
#define ERR_TOOMANYCHANNELS(nick, chan) ":42-IRC 405 " + nick + " " + chan + " :You have joined too many channels" + ENDL
#define ERR_NONICKNAMEGIVEN(nick) ":42-IRC 431 " + nick + " :No nickname given" + ENDL
#define ERR_ERRONEUSNICKNAME(nick) ":42-IRC 432 " + nick + " :Erroneus nickname" + ENDL
#define ERR_NICKNAMEINUSE(nick) ":42-IRC 433 " + nick + " :Nickname is already in use" + ENDL
#define ERR_USERNOTINCHANNEL(nick1, nick2, chan) ":42-IRC 441 " + nick1 + " " + nick2 + " " + chan + " :They aren't on that channel" + ENDL
#define ERR_NOTONCHANNEL(nick, chan) ":42-IRC 442 " + nick + " " + chan + " :You're not on that channel" + ENDL
#define ERR_USERONCHANNEL(nick1, nick2, chan) ":42-IRC 443 " + nick1 + " " + nick2 + " " + chan + " :is already on channel" + ENDL
#define ERR_NOTREGISTERED(nick) ":42-IRC 451 " + nick + " :You have not registered" + ENDL
#define ERR_NOTENOUGHPARAM(nick) ":42-IRC 461 " + nick + " :Not enough parameters" + ENDL
#define ERR_ALREADYREGISTERED(nick) ":42-IRC 462 " + nick + " :You may not reregister" + ENDL
#define ERR_PASSWDMISMATCH(nick) ":42-IRC 464 " + nick + " :Password incorrect" + ENDL
#define ERR_CHANNELISFULL(nick, chan) ":42-IRC 471 " + nick + " " + chan + ":Cannot join channel (+l)" + ENDL
#define ERR_INVITEONLYCHAN(nick, chan) ":42-IRC 473 " + nick + " " + chan + ":Cannot join channel (+i)" + ENDL
#define ERR_BADCHANNELKEY(nick, chan) ":42-IRC 475 " + nick + " " + chan + ":Cannot join channel (+k)" + ENDL
#define ERR_CHANOPRIVSNEEDED(nick, chan) ":42-IRC 482 " + nick + " " + chan + " :You're not channel operator" + ENDL
#define ERR_INVALIDMODEPARAM(nick, chan, mode, param) ":42-IRC 696 " + nick + " " + chan + " " + mode + " " + param + " :Invalid mode parameter" + ENDL
#define ERR_UNKNOWNMODE(nick, mode) ":42-IRC 472 " + nick + " " + mode + " :is unknown mode char to me" + ENDL
#define RPL_WELCOME(nick) ":42-IRC 001 " + nick + " :Welcome to the 42-IRC Network" + ENDL
#define RPL_JOINMSG(nick, user, ip, chan) ":42-IRC " + nick + "!" + user + "@" + ip + " JOIN " + chan + ENDL
#define RPL_NICKCHANGED(oldnick, newnick)  ":42-IRC " + oldnick + " NICK " + newnick + ENDL
#define RPL_NAMREPLY(nick, chan, namelist) ":42-IRC 353 " + nick + " = " + chan + " :" + namelist + ENDL
#define RPL_ENDOFNAMES(nick, chan) ":42-IRC 366 " + nick + " " + chan + " :End of /NAMES list" + ENDL
#define RPL_NOTOPIC(nick, chan) ":42-IRC 331 " + nick + " " + chan + " :No topic is set" + ENDL
#define RPL_TOPIC(nick, chan, topic) ":42-IRC 332 " + nick + " " + chan + " :" + topic + ENDL
#define RPL_TOPICWHOTIME(nick, chan, nick2, setat) ":42-IRC 333 " + nick + " " + chan + " " + nick2 + " " + setat + ENDL
#define RPL_INVITING(nick1, nick2, chan) ":42-IRC 341 " + nick1 + " " + nick2 + " " + chan + ENDL
#endif