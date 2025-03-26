/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandPrivMsg.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpointil <jpointil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:02:42 by jpointil          #+#    #+#             */
/*   Updated: 2025/03/24 09:43:03 by jpointil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"

class Client;

class CommandPrivMsg : public Command {

    private:
       
    public:

        void execute(const std::string &args, Client *client);
};
