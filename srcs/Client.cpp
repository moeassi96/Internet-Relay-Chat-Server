/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:02:04 by moassi            #+#    #+#             */
/*   Updated: 2024/08/07 16:10:46 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/* -------------------------Constructors-Destructors--------------------------*/
Client::Client()
{
    this->_fd = -1;
    this->_is_registered = false;
    this->_is_fully_registered = false;
    this->_nickname = "";
    this->_username = "";
}

Client::Client(std::string nickname, std::string username, int fd)
{
    this->_fd = fd;
    this->_nickname = nickname;
    this->_username = username;
    this->_is_registered = false;
    this->_is_fully_registered = false;
}

Client::~Client(){}

Client::Client(Client const &other){ *this = other; }

Client&         Client::operator=(Client const &other)
{
    if (this != &other)
    {
        this->_fd = other._fd;
        this->_is_registered = other._is_registered;
        this->_is_fully_registered = other._is_fully_registered;
        this->_username = other._username;
        this->_nickname = other._nickname;
        this->_buffer = other._buffer;
        this->_ipadd = other._ipadd;
    }
    return (*this);
}

/* ------------------------------Getters-Setters------------------------------*/
    
int             Client::getFd(){ return (this->_fd); }
bool            Client::getIsRegistered(){ return (this->_is_registered); }
bool            Client::getIsFullyRegistered(){ return (this->_is_fully_registered); }
std::string     Client::getIpadd(){ return (this->_ipadd); }
std::string     Client::getNickname(){ return (this->_nickname); }
std::string     Client::getUsername(){ return (this->_username); }
std::string     Client::getBuffer(){ return (this->_buffer); }

void            Client::setFd(int fd){ this->_fd = fd; }
void            Client::setIsRegistered(bool is_registered){ this->_is_registered = is_registered; }
void            Client::setIsFullyRegistered(bool is_fully_registered){ this->_is_fully_registered = is_fully_registered; }
void            Client::setIpAdd(std::string ipadd){ this->_ipadd = ipadd; }
void            Client::setNickname(std::string nickname){ this->_nickname = nickname; }
void            Client::setUsername(std::string username){ this->_username = username; }
void            Client::setBuffer(std::string buffer){ this->_buffer = buffer; }  
void            Client::addToBuffer(std::string str){ this->_buffer += str; }