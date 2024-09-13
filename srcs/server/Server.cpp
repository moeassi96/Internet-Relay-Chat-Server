/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 20:15:26 by moassi            #+#    #+#             */
/*   Updated: 2024/08/11 16:01:52 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* -------------------------Constructors-Destructors--------------------------*/

Server::Server(): _server_socket_fd(-1){}

Server::~Server(){}

Server::Server(const Server &other){ *this = other; }

Server&         Server::operator=(const Server &other)
{
    if (this != &other)
    {
        this->_port = other._port;
        this->_server_socket_fd = other._server_socket_fd;
        this->_password = other._password;
        this->_clients = other._clients;
        this->_channels = other._channels;
        this->_fds = other._fds;
    }
    return (*this);
}

/* ------------------------------Getters-Setters------------------------------*/

int 			Server::getPort(){ return (this->_port); }
int 			Server::getServerSocketFd(){ return (this->_server_socket_fd); }
std::string		Server::getPassword(){ return (this->_password); }

void			Server::setPort(int port){ this->_port = port; }
void			Server::setServerSocketFd(int fd){ this->_server_socket_fd = fd; }
void			Server::setPassword(std::string	password){ this->_password = password; }

/* --------------------------------Server Setup--------------------------------*/

void            Server::serverInit(int port, std::string password)
{
    this->_port = port;
    this->_password = password;
    createServerSocket();
    std::cout << "Server <" << _server_socket_fd << "> Connected" << std::endl;
    std::cout << "Waiting for connections.." << std::endl;
    while (_signal == false)
    {
        if ((poll(&_fds[0], _fds.size(), -1) == -1) && _signal == false)
            throw (std::runtime_error("Error: poll() failed"));
        for(size_t i = 0; i < _fds.size(); i++)
        {
            if (_fds[i].revents & POLLIN)
            {
                if (_fds[i].fd == _server_socket_fd)
                    acceptNewClient();
                else
                    recieveNewData(_fds[i].fd);
            }
        }
    }
    closeFds();
}

void            Server::createServerSocket()
{
    struct sockaddr_in  add;
    struct pollfd       newPoll;
    add.sin_family = AF_INET;
    add.sin_port = htons(this->_port);
    add.sin_addr.s_addr = INADDR_ANY;
    this->_server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_server_socket_fd == -1)
        throw std::runtime_error("Error: failed to create socket");
    int en = 1;
    if (setsockopt(_server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
        throw std::runtime_error("Error: failed to set socket option => 'SO_REUSEADDR'");
    if (fcntl(_server_socket_fd, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("Error: failed to set socket option => '(O_NONBLOCK)'"));
    if (bind(_server_socket_fd, (struct sockaddr *)&add, sizeof(add)) == -1) 
		throw(std::runtime_error("Error: failed to bind socket to address"));
    if (listen(_server_socket_fd, SOMAXCONN) == -1) 
		throw(std::runtime_error("Error: failed to => listen()"));
    newPoll.fd = _server_socket_fd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;
    this->_fds.push_back(newPoll);
}

/* -------------------------------Signal handler-------------------------------*/

bool            Server::_signal = false;
void            Server::signalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::_signal = true;
}

/* -----------------------------------channels---------------------------------*/

void            Server::removeChannel(const std::string &name)
{
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i].getName() == name)
        {
            _channels.erase(_channels.begin() + i);
            return ;
        }
    }
}

Channel*        Server::getChannel(const std::string &name)
{
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i].getName() == name)
            return (&_channels[i]);
    }
    return (NULL);
}

/* ----------------------------------Removers----------------------------------*/

void            Server::removeClient(int fd)
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

void            Server::removeFd(int fd)
{
    for (size_t i = 0; i < _fds.size(); i++)
    {
        if (_fds[i].fd == fd)
        {
            _fds.erase(_fds.begin() + i);
            return ;
        }
    }
}

void            Server::removeClientFromChannels(int fd)
{
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i].getClient(fd))
            _channels[i].removeClient(fd);
        else if (_channels[i].getOperator(fd))
            _channels[i].removeOperator(fd);
        if (_channels[i].getClientNum() == 0)
        {
            _channels.erase(_channels.begin() + i);
            i--;
        }
        /*
         announce his exit to the whole channel
        */
    }
}

/* ----------------------------------Closing----------------------------------*/

void            Server::closeFds()
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        std::cout << "client: " << _clients[i].getFd() << " disconnected" << std::endl;
        close(_clients[i].getFd());
    }
    if (_server_socket_fd != -1)
    {
        std::cout << "server: " << _server_socket_fd << " disconnected" << std::endl;
        close(_server_socket_fd);
    }
}

/* ----------------------------------The rest----------------------------------*/

void            Server::acceptNewClient()
{
    Client client;
    struct sockaddr_in clientAddress;
    struct pollfd newPoll;
    socklen_t len = sizeof(clientAddress);
    
    int incomingfd = accept(_server_socket_fd, (sockaddr *)&(clientAddress), &len);
	if (incomingfd == -1)
	{
        std::cout << "Error: accept() failed" << std::endl;
        return ;
    }
    if (fcntl(incomingfd, F_SETFL, O_NONBLOCK) == -1) 
	{
        std::cout << "Error: fcntl() failed" << std::endl;
        return ;
    }
    newPoll.fd = incomingfd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
    client.setFd(incomingfd);
    client.setIpAdd(inet_ntoa((clientAddress.sin_addr)));
    _clients.push_back(client);
    _fds.push_back(newPoll);
    std::cout << "Client <" << incomingfd << "> Connected" << std::endl;
}

void            Server::recieveNewData(int fd)
{
	char buff[1024]; 
	memset(buff, 0, sizeof(buff));
    Client *client = getClient(fd);
	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1 , 0);

	if(bytes <= 0)
    {
		std::cout << "Client <" << fd << "> Disconnected" << std::endl;
		removeClient(fd);
        removeClientFromChannels(fd);
        removeFd(fd);
		close(fd);
	}
	else
    { 
        std::cout << buff << std::endl;
		client->addToBuffer(buff);
        if (client->getBuffer().find_first_of("\r\n") == std::string::npos)
            return ;
        strVector lines = splitCmdLines(client->getBuffer()); 
        for (size_t i = 0; i < lines.size(); i++)
            execCommand(lines[i], fd);
        if (getClient(fd))
            getClient(fd)->setBuffer("");
	}
}

/* ----------------------------------Clients----------------------------------*/

Client*			Server::getClient(int fd)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i].getFd() == fd)
            return (&_clients[i]);
    }
    return (NULL);
}

Client*			Server::getClientByName(std::string name)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i].getNickname() == name)
            return (&_clients[i]);
    }
    return (NULL);
}

int				Server::countClientinChannels(int fd)
{
    int count;
    count = 0;
    for(size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i].getClient(fd) || _channels[i].getOperator(fd))
            count++;
    }
    return (count);
}

/* ------------------------------------RES------------------------------------*/

void              Server::sendResponse(const std::string &res, int fd)
{
    if (send(fd, res.c_str(), res.length(), 0) == -1)
        std::cerr << "Error: sending response" << std::endl;
}
