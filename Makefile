# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: florencecousergue <florencecousergue@st    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#              #
#    Updated: 2025/02/18 18:03:36 by florencecou      ###   ########.fr        #
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

C_FILES		=	main.cpp Client.cpp Server.cpp

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
