/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandPrivMsg.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpointil <jpointil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:02:21 by jpointil          #+#    #+#             */
/*   Updated: 2025/03/24 16:39:41 by jpointil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "CommandPrivMsg.hpp"
#include "Server.hpp"
#include "Channel.hpp"

void CommandPrivMsg::execute(const std::string &args, Client *client) {

    std::vector<std::string> argsVec = splitArgs(args);
    std::string msgArg = "";
    std::string msg;

    if (argsVec.size() > 1)
        msgArg = argsVec[1];
    if (!argsVec[0].empty()) {

        if (argsVec[0][0] == '#' || argsVec[0][0] == '&') {

            std::cout << GREEN << INVERSE << "Channel :3" << RESET << std::endl;
            std::map<std::string, Channel *> channel = Server::getInstance().getChannelMap();
            std::map<std::string, Channel *>::const_iterator it = channel.begin();
            for ( ; it != channel.end(); ++it) {

                if (sameString(argsVec[0], it->first)) {

                    if (client->isInList(it->second->getMembers())) {

                        msg = client->getSource() + " PRIVMSG " + argsVec[0] + " :" + msgArg;
                        std::set<int> tmplist = it->second->getMembers();
                        Channel::removeFromList(tmplist, client->getFd());
                        broadcast(tmplist, msg);
                        break ;
                    }
                    else {

                        msg = client->getNickname() + argsVec[0];
                        return sendRpl(client, ERR_CANOTSENDTOCHAN, msg.c_str());
                    }
                }
            }
            if (it == channel.end()) {
                msg = client->getNickname() + argsVec[0];
                return sendRpl(client, ERR_NOSUCHCHANNEL, msg.c_str());
            }
        }
        else {

            std::vector<Client *> clientsVec = Server::getInstance().getClients();
            std::vector<Client *>::const_iterator it2 = clientsVec.begin();
            for ( ; it2 != clientsVec.end(); ++it2) {

                if (argsVec[0] == (*it2)->getNickname()) {

                    msg = client->getSource() + " PRIVMSG " + argsVec[0] + " " + msgArg;
                    sendMsg((*it2), msg);
                    break ;
                }
            }
            if (it2 == clientsVec.end()) {

                msg = client->getNickname() + argsVec[0];
                return sendRpl(client, ERR_NOSUCHNICK, msg.c_str());
            }
        }
    }
    return;
}
