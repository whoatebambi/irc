#include "Server.hpp"
#include <csignal>
#include <clocale>



static void signal_interrupt(int sig)
{
    (void)sig;
    Server::getInstance().shutdown();
}

static int handle_signals(int sig, void (*handler)(int))
{
    sigset_t set;
    struct sigaction action;

    sigemptyset(&set);
    action.sa_handler = handler;
    action.sa_mask = set;
    action.sa_flags = 0;
    if (sigaction(sig, &action, NULL) == -1)
    {
        throw std::runtime_error("Sigaction failed");
        return -1;
    }
    return 0;
}

int main(int ac, char **av)
{
    if (ac != 3)
        return 1;
    try
    {
        handle_signals(SIGINT, &signal_interrupt);
        handle_signals(SIGQUIT, SIG_IGN);

        std::setlocale(LC_ALL, "en_US.UTF-8");

        Server::getInstance().init(av);
        Server::getInstance().boot();
        while(Server::getInstance().isLive())
        {
            Server::getInstance().monitor();
        }
    }
    catch (const ServExcept &e) 
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception &e) 
    {
        std::cerr << "Unexpected Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) 
    {
        std::cerr << "Unknown fatal error occurred." << std::endl;
        return 1;
    }
    return 0;
//
}