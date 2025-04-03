NAME		=	ircserv

UNAME_S		:=	$(shell uname -s)

CC			=	c++
FLAG		=	-Wall -Wextra -Werror -std=c++98 -Ipoll

# Settings for florencecousergue macOS
ifeq ($(UNAME_S), Darwin)
CC		=	c++
FLAG	=	-Wall -Wextra -Werror -std=c++98 -Ipoll
endif

C_FILES		=	main.cpp Client.cpp Server.cpp Channel.cpp Command.cpp Reply.cpp \
				poll/PollPoller.cpp poll/PollerFactory.cpp \
				CommandCap.cpp CommandNick.cpp CommandUser.cpp CommandMode.cpp CommandJoin.cpp CommandPass.cpp \
				CommandPing.cpp CommandPrivMsg.cpp CommandTopic.cpp CommandPart.cpp CommandInvite.cpp \
				CommandKick.cpp CommandQuit.cpp\

ifeq ($(UNAME_S), Linux)
	C_FILES += poll/EpollPoller.cpp
endif

OBJ_DIR		=	builds

OBJS		=	$(addprefix $(OBJ_DIR)/, $(C_FILES:.cpp=.o))

DEPS		=	$(OBJS:.o=.d)

all:		$(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(FLAG) -MMD -MP -c $< -o $@

$(NAME):	$(OBJS)
	$(CC) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJ_DIR)/*.o $(DEPS)
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

-include $(DEPS)

.PHONY:	all clean fclean re

asan:
	clang++ -fsanitize=address -std=c++98 -g $(C_FILES) -o $(NAME)