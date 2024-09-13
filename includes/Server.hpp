/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 19:59:23 by moassi            #+#    #+#             */
/*   Updated: 2024/08/12 16:02:50 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>
#include <vector>
#include <cstring>
#include <sstream>
#include "Client.hpp"
#include "Channel.hpp"
#include "error_msgs.hpp"


class Client;
class Channel;

class Server
{
	public:
		typedef std::vector<std::string> strVector;
		
    private:
	
        int							_port;
        int							_server_socket_fd;
		std::string 				_password;
		
		static bool 				_signal;
		
		std::vector<Client>			_clients;
		std::vector<Channel>		_channels;
		std::vector<struct pollfd>	_fds;
		
		void            removeChannel(const std::string &name);
        Channel*        getChannel(const std::string &name);
		void 			removeClient(int fd);
		void 			removeFd(int fd);
		void			removeClientFromChannels(int fd);
		Client*			getClient(int fd);
		Client*			getClientByName(std::string name);
		int				countClientinChannels(int fd); 
		void            sendResponse(const std::string &res, int fd);
		strVector 		splitCmdLines(const std::string &str);
		strVector 		splitCMD(const std::string &str);
		strVector 		splitOnCommas(const std::string &str);
		void    		execCommand(std::string &command, int fd);
		void			authenticateUser(std::string command, int fd);
		bool 			isUsedNickname(const std::string &nickname);
		bool 			isValidNickname(const std::string &nickname);
		void 			setNickname(std::string command, int fd);
		void 			setUsername(std::string command, int fd);
		void			newChannel(strVector args, int fd);
		void			joinChannel(strVector args, int fd);
		void            join(std::string command, int fd);
		void            invite(std::string command, int fd);
		void            kick(std::string command, int fd);
		void            part(std::string command, int fd);
		void            topic(std::string command, int fd);
		void            quit(std::string command, int fd);
		void            mode(std::string command, int fd);
		void            privmsg(std::string command, int fd);
		void    		msgChannel(std::string channelName, std::string msg, int fd);
		void    		msgClient(std::string targetName, std::string msg, int fd);

    public:
	
		Server();
		~Server();
		Server(const Server &other);
		Server& 		operator=(const Server &other);
		
		int 			getPort();
		int 			getServerSocketFd();
		std::string		getPassword();
		void			setPort(int port);
		void			setServerSocketFd(int fd);
		void			setPassword(std::string	password);
		
		void 			serverInit(int port, std::string password); 
		void 			createServerSocket();

		static void 	signalHandler(int signum);
	
		void 			closeFds();
		void 			acceptNewClient(); 
		void 			recieveNewData(int fd); //-> recieve data from a registered client
};

#endif
