#pragma once

#include "Command.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#include <string>
#include <sstream>

class Client;
class Channel;

class CommandMode : public Command
{
	public:
		void	execute(const std::string &args, Client *client);

		void	sendChannelMode(Channel *chan, Client *client);

		void	applyModes(Channel* channel, Client *client, const std::vector<std::string>& args);
		void	handleMode_i(Channel* channel, bool adding, std::string& modes);
		void	handleMode_t(Channel* channel, bool adding, std::string& modes);
		void	handleMode_k(Channel* channel, Client* client, const std::vector<std::string>& args, bool adding, size_t& j, std::string& modes, std::string& params);
		void	handleMode_l(Channel* channel, const std::vector<std::string>& args, bool adding, size_t& j, std::string& modes, std::string& params);
		void	handleMode_o(Channel* channel, Client* client, const std::vector<std::string>& args, bool adding, size_t& j, std::string& modes, std::string& params);

		void	refactorTarget(std::string &target);
		std::vector<std::string>	splitArgs(const std::string &input);
};
