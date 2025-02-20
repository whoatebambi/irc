/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florencecousergue <florencecousergue@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/02/19 17:38:00 by florencecou      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

Client::Client(const char* serverIp, int port) : _serverIp(serverIp), _port(port), _socketFd(-1) {}

Client::~Client() {
    if (_socketFd != -1)
        close(_socketFd);
}

void Client::connectToServer() {
    _socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketFd == -1) {
        std::cerr << "Socket creation failed\n";
        return;
    }

    sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_port);
    if (inet_pton(AF_INET, _serverIp, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address\n";
        close(_socketFd);
        return;
    }

    if (connect(_socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Connection failed\n";
        close(_socketFd);
        return;
    }

    std::cout << "Connected to server " << _serverIp << " on port " << _port << "\n";
}