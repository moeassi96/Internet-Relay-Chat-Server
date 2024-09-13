/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitters.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 12:56:25 by moassi            #+#    #+#             */
/*   Updated: 2024/08/10 12:56:45 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::strVector   Server::splitCmdLines(const std::string &str)
{
    strVector lines;
    std::istringstream ss(str);
    std::string line;
    while (std::getline(ss, line))
    {
        size_t br = line.find_first_of("\r\n");
        if (br != std::string::npos)
            line = line.substr(0, br);
        lines.push_back(line);
    }
    return (lines);
}

Server::strVector   Server::splitCMD(const std::string &str)
{
    strVector args;
    std::istringstream ss(str);
    std::string arg;
    while (ss >> arg)
    {
        args.push_back(arg);
        arg.clear();
    }
    return (args);
}

Server::strVector   Server::splitOnCommas(const std::string &str)
{
    strVector tokens;
    std::istringstream ss(str);
    std::string token;

    while (std::getline(ss, token, ','))
    {
        tokens.push_back(token);
    }
    return (tokens);
}
