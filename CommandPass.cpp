#include "CommandPass.hpp"

void CommandPass::execute(const std::string &args, Client *client)
{
	std::cout << INVERSE_BG << BLUE << "PASS command executed with args: " BOLD << args << RESET << std::endl;
	(void)args;
	(void)client;
}
