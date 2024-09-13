/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 21:49:19 by moassi            #+#    #+#             */
/*   Updated: 2024/08/06 15:05:02 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

int main(int ac, char **av)
{
    int         port;
    std::string password;
    
    try{ validateArgs(ac, av, port, password); }
    catch(const std::exception& e)
    { std::cerr << e.what() << '\n'; return (1); }
    
    Server server;
    try
    {
        signal(SIGINT, Server::signalHandler);
        signal(SIGQUIT, Server::signalHandler);
        server.serverInit(port, password);
    }
    catch(const std::exception& e)
    {
        server.closeFds();
        std::cerr << e.what() << std::endl;
    }
    
    return (0);
}