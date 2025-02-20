/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florencecousergue <florencecousergue@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/02/19 17:39:25 by florencecou      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Client {
	public:
		Client(const char* serverIp, int port);
		~Client();
		void connectToServer();
	
	private:
		const char* _serverIp;
		int _port;
		int _socketFd;
};
	