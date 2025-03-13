# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: codespace <codespace@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#              #
#    Updated: 2025/03/11 14:28:12 by codespace        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	irc

UNAME_S		:=	$(shell uname -s)

# Default settings
CC			=	c++
FLAG		=	-Wall -Wextra -Werror -std=c++98

# Settings for florencecousergue macOS
ifeq ($(UNAME_S), Darwin)
CC			=	/opt/homebrew/Cellar/gcc/14.2.0_1/bin/g++-14
FLAG		=	-Wall -Wextra -Werror -std=c++98 -I/usr/local/include -L/usr/local/lib
endif

C_FILES		=	main.cpp Client.cpp Server.cpp Command.cpp CommandCap.cpp

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
