/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:56:09 by moassi            #+#    #+#             */
/*   Updated: 2024/08/10 15:36:41 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <ctime>

/* -------------------------Constructors-Destructors--------------------------*/

Channel::Channel()
{
    this->_invite_only = false;
    this->_key = false;
    this->_limit = -1;
    this->_topic_restriction = false;
    this->_topic = "";
    this->_topic_owner = "";
    this->_topic_time = "";
    this->_password = "";
    this->_name = "";
}

Channel::~Channel(){}

Channel::Channel(Channel const &other){ *this = other; }

Channel & Channel::operator=(Channel const &other)
{
    if (this != &other){
		this->_invite_only = other._invite_only;
		this->_topic = other._topic;
        this->_topic_owner = other._topic_owner;
        this->_topic_time = other._topic_time;
		this->_key = other._key;
		this->_limit = other._limit;
		this->_topic_restriction = other._topic_restriction;
		this->_name = other._name;
		this->_password = other._password;
		this->_clients = other._clients;
		this->_operators = other._operators;
	}
	return (*this);
}

/* ------------------------------Getters-Setters------------------------------*/

bool            Channel::getInviteOnly(){ return (this->_invite_only); }
bool            Channel::getTopicRestriction(){ return (this->_topic_restriction); }
bool            Channel::getKey(){ return (this->_key); }
int             Channel::getLimit(){ return (this->_limit); }
std::string     Channel::getName(){ return (this->_name); }
std::string     Channel::getTopicOwner(){ return (this->_topic_owner); }
std::string     Channel::getTopicTime(){ return (this->_topic_time); }
std::string     Channel::getPassword(){ return (this->_password); }
std::string     Channel::getTopic(){ return (this->_topic); }

void            Channel::setInviteOnly(bool invite_only){ this->_invite_only = invite_only; }
void            Channel::setTopicRestriction(bool topic_restriction){ this->_topic_restriction = topic_restriction; }
void            Channel::setKey(bool key){ this->_key = key; }
void            Channel::setLimit(int limit){ this->_limit = limit; }
void            Channel::setName(std::string name){ this->_name = name; }
void            Channel::setPassword(std::string password){ this->_password = password; }
void            Channel::setTopic(std::string topic){ this->_topic = topic; }
void            Channel::setTopicOwner(std::string topic_owner){ this->_topic_owner = topic_owner; }
void            Channel::setTopicTime()
{
    std::time_t currentTime = std::time(NULL);
    std::ostringstream oss;
    oss << currentTime;
    std::string time = oss.str();
    this->_topic_time = time;
}

/* ----------------------------Add/Remove clients----------------------------*/

void            Channel::addClient(Client client)
{
    this->_clients.push_back(client);
}

void            Channel::addOperator(Client client)
{
    this->_operators.push_back(client);
}

void            Channel::removeClient(int fd)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i].getFd() == fd)
        {
            _clients.erase(_clients.begin() + i);
            break ;
        }
    }
}

void            Channel::removeOperator(int fd)
{
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i].getFd() == fd)
        {
            _operators.erase(_operators.begin() + i);
            break ;
        }
    }
}

Client*         Channel::getClient(int fd)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i].getFd() == fd)
            return (&_clients[i]);
    }
    return (NULL);
}

Client*         Channel::getOperator(int fd)
{
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i].getFd() == fd)
            return (&_operators[i]);
    }
    return (NULL);
}

void            Channel::addInvite(int fd)
{
    if (!verifyInvitation(fd))
        _invites.push_back(fd);
}

void            Channel::removeInvite(int fd)
{
    for (size_t i = 0; i < _invites.size(); i++)
    {
        if (_invites[i] == fd)
        {
            _invites.erase(_invites.begin() + i);
        }
    }
}

/* ------------------------------------utils----------------------------------*/

size_t          Channel::getClientNum(){ return (_clients.size()); }

size_t          Channel::getOperatorNum(){ return (_operators.size()); }

std::string     Channel::getClientNames()
{
    std::string names = "";

    for(size_t i = 0; i < _operators.size(); i++)
    {
        if (i != 0)
           names += " "; 
        names += "@";
        names += _operators[i].getNickname();
    }
    for(size_t i = 0; i < _clients.size(); i++)
    {
        if (names.length() > 0)
            names += " "; 
        names += _clients[i].getNickname();
    }
    return (names);
}

bool            Channel::verifyInvitation(int fd)
{
    for (size_t i = 0; i < _invites.size(); i++)
    {
        if (_invites[i] == fd)
            return (true);
    }
    return (false);
}

void            Channel::assignNewOperator()
{
    if (!_clients.empty())
    {
        Client client = _clients.front();
        _clients.erase(_clients.begin());
        _operators.push_back(client);
        std::stringstream ss;
        ss << ":42-IRC"<< " MODE " << this->getName() << " +o " << client.getNickname() << "\r\n";
        sendResponseToall(ss.str());
    }
}

void            Channel::makeOperator(int fd)
{
    Client *client = getClient(fd);
    if (!client)
        return ;
    removeClient(fd);
    addOperator(*client);
}

void            Channel::makeClient(int fd)
{
    Client *client = getOperator(fd);
    if (!client)
        return ;
    removeOperator(fd);
    addClient(*client);
    if (_operators.size() == 0)
        assignNewOperator();
}

/* ------------------------------------RES------------------------------------*/

void            Channel::sendResponse(const std::string &res, int fd)
{
    if (send(fd, res.c_str(), res.length(), 0) == -1)
        std::cerr << "Error: sending response" << std::endl;
}

void            Channel::sendResponseToall(const std::string &res)
{
    for (size_t i = 0; i < _operators.size(); i++)
    {
        sendResponse(res, _operators[i].getFd());
    }
    for (size_t i = 0; i < _clients.size(); i++)
    {
        sendResponse(res, _clients[i].getFd());
    }
}

void            Channel::sendResponseToallOthers(const std::string &res, int fd)
{
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i].getFd() != fd)
            sendResponse(res, _operators[i].getFd());
    }
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i].getFd() != fd)
            sendResponse(res, _clients[i].getFd());
    }
}