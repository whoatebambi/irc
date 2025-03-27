#pragma once

#include "Command.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#include <string>
#include <sstream>

class Client;
class Channel;

struct CommandContext {
	Client*		client;
	Channel*	channel;
	std::string	modeStr;
	bool		adding;
	size_t		index;
	std::string	modes;
	std::string	params;
	std::vector<std::string> const	&args;

	CommandContext(Client* c, Channel* ch, const std::vector<std::string>& a)
		: client(c), channel(ch), modeStr(a[1]), adding(true), index(2), args(a) {}
};

class CommandMode : public Command
{
	public:
		void	execute(const std::string &args, Client *client);

		void	sendModes(Channel *chan, Client *client);
		void	applyModes(Channel* channel, Client *client, const std::vector<std::string> &args);
		void	handleMode_i(CommandContext &ctx);
		void	handleMode_t(CommandContext &ctx);
		void	handleMode_k(CommandContext &ctx);
		void	handleMode_l(CommandContext &ctx);
		void	handleMode_o(CommandContext &ctx);
};
