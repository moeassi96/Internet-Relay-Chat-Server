/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:45:05 by moassi            #+#    #+#             */
/*   Updated: 2024/08/10 15:31:19 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include "Server.hpp"

class Client;

class Channel
{
    private:
        bool                _invite_only;
        bool                _topic_restriction;
        bool                _key;
        int                 _limit;
        std::string         _topic_owner;
        std::string         _topic_time;
        std::string         _name;
        std::string         _topic;
        std::string         _password;
        std::vector<Client> _clients;
        std::vector<Client> _operators;
        std::vector<int>    _invites;
        
    public:

    /* -------------------------Constructors-Destructors--------------------------*/
    
        Channel();
        ~Channel();
        Channel(Channel const &other);
        Channel &operator=(Channel const &other);
    
    /* ------------------------------Getters-Setters------------------------------*/

        bool            getInviteOnly();
        bool            getTopicRestriction();
        bool            getKey();
        int             getLimit();
        std::string     getName();
        std::string     getPassword();
        std::string     getTopic();
        std::string     getTopicOwner();
        std::string     getTopicTime();

        void            setInviteOnly(bool invite_only);
        void            setTopicRestriction(bool topic_restriction);
        void            setKey(bool key);
        void            setLimit(int limit);
        void            setName(std::string name);
        void            setPassword(std::string password);
        void            setTopic(std::string topic);
        void            setTopicOwner(std::string topic_owner);
        void            setTopicTime();

    /* ---------------------------Add/Remove/get clients--------------------------*/

        void            addClient(Client client);
        void            addOperator(Client client);
        void            removeClient(int fd);
        void            removeOperator(int fd);
        Client*         getClient(int fd);
        Client*         getOperator(int fd);
        void            addInvite(int fd);
        void            removeInvite(int fd);
        void            makeOperator(int fd);
        void            makeClient(int fd);

    /* ------------------------------------utils----------------------------------*/

        size_t          getClientNum();
        size_t          getOperatorNum();
        std::string     getClientNames();
        bool            verifyInvitation(int fd);
        void            assignNewOperator();
    
    /* ------------------------------------RES------------------------------------*/
        void            sendResponse(const std::string &res, int fd);
		void            sendResponseToall(const std::string &res);
        void            sendResponseToallOthers(const std::string &res, int fd);
};

#endif