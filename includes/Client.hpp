/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 20:09:26 by moassi            #+#    #+#             */
/*   Updated: 2024/08/07 16:09:28 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client 
{
    private:
        int                 _fd;
        bool                _is_registered;
        bool                _is_fully_registered;
        std::string         _ipadd;
        std::string         _nickname;
        std::string         _username;
        std::string         _buffer;
    
    public:

    /* -------------------------Constructors-Destructors--------------------------*/
        Client();
        Client(std::string nickname, std::string username, int fd);
        ~Client();
        Client(Client const &other);
        Client&         operator=(Client const &other);

    /* ------------------------------Getters-Setters------------------------------*/
    
        int             getFd();
        bool            getIsRegistered();
        bool            getIsFullyRegistered();
        std::string     getIpadd();
        std::string     getNickname();
        std::string     getUsername();
        std::string     getBuffer();
        
        void            setFd(int fd);
        void            setIsRegistered(bool is_registered);
        void            setIsFullyRegistered(bool is_registered);
        void            setIpAdd(std::string ipadd);
        void            setNickname(std::string nickname);
        void            setUsername(std::string username);
        void            setBuffer(std::string buffer);   
        void            addToBuffer(std::string str);       
};

#endif