/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandQuit.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpointil <jpointil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 12:37:15 by jpointil          #+#    #+#             */
/*   Updated: 2025/03/31 15:07:20 by jpointil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"

class CliSocket;

class CommandQuit : public Command
{
    private:

    public:
        void execute(const std::string &args, Client *client);
};
