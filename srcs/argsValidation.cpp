/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argsValidation.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 13:31:18 by moassi            #+#    #+#             */
/*   Updated: 2024/08/06 13:53:48 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

void validateArgs(int ac, char **av, int &port, std::string &password)
{
    if (ac != 3)
        throw std::runtime_error("Error: usage => ./ircserv <port> <password>");
    std::stringstream ss(av[1]);
    ss >> port;
    if (ss.fail() || !ss.eof() || port <= 0)
        throw std::runtime_error("Error: invalid port number");
    password = av[2];
    if (password.length() < 8)
        throw std::runtime_error("Error: invalid password\npassword should be 8 characters long");
}