/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandQuit.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpointil <jpointil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 12:36:02 by jpointil          #+#    #+#             */
/*   Updated: 2025/03/31 16:31:09 by jpointil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "CommandQuit.hpp"
#include "Client.hpp"

void CommandQuit::execute(const std::string &args, Client *client) {

    std::set<int> pairs = Server::getInstance().getSharedChans(client);
    std::string msg = client->get_source() + "QUIT :Quit " + args;
    Reply::sendBroadcast(pairs, client, msg);
    Server::getInstance().removeClient(client->get_fd());
}
