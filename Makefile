NAME		=	irc

UNAME_S		:=	$(shell uname -s)

CC			=	c++
FLAG		=	-Wall -Wextra -Werror -std=c++98

C_FILES		=	main.cpp Client.cpp Server.cpp Channel.cpp Command.cpp  \
				CommandCap.cpp CommandNick.cpp CommandUser.cpp CommandMode.cpp CommandJoin.cpp CommandPass.cpp \
				CommandPing.cpp CommandPrivMsg.cpp CommandTopic.cpp CommandPart.cpp CommandInvite.cpp Reply.cpp\

OBJ_DIR		=	builds

OBJS		=	$(addprefix $(OBJ_DIR)/, $(C_FILES:.cpp=.o))

DEPS		=	$(OBJS:.o=.d)

all:		$(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
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
