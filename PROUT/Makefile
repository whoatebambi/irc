# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jfita <jfita@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/25 16:00:57 by guillaumebe       #+#    #+#              #
#    Updated: 2025/03/03 12:29:29 by jfita            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################### COLORS #####################################

RESET		:=	\e[0m
BOLD		:=	\e[1m
DIM		:=	\e[2m
ITAL		:=	\e[3m
UNDERLINE	:=	\e[4m

BLACK		:=	\e[30m
GRAY		:=	\e[90m
RED		:=	\e[31m
GREEN		:=	\e[32m
YELLOW		:=	\e[33m
ORANGE		:=	\e[38;5;208m
BLUE		:=	\e[34m
PURPLE		:=	\e[35m
PINK		:=	\033[38;2;255;182;193m
CYAN		:=	\e[36m



################################### BASICS ###################################
NAME		=	IRC

CC		=	c++
CFLAGS =	-Wall -Wextra -Werror -std=c++98 -g3 -I $(INC) -I$(INC)Command
DEP_FLAGS	=	-MMD -MP
INC		=	include/
LIBC	=	ar -rcs
RM		=	rm -rf


################################### SOURCES ###################################

GENERAL_FILES	= 	Channel.cpp CliSocket.cpp Factory.cpp main.cpp MainSocket.cpp SerSocket.cpp Server.cpp ErrMsg.cpp

COMMAND_DIR 	= 	Command/
COMMAND_FILES	= 	CapCommand.cpp Command.cpp NickCommand.cpp PassCommand.cpp UserCommand.cpp \
					PingCommand.cpp JoinCommand.cpp QuitCommand.cpp PartCommand.cpp KickCommand.cpp InviteCommand.cpp ModeCommand.cpp TopicCommand.cpp \
					PrivmsgCommand.cpp
					

######################## COMBINE DIRECTORIES AND FILES ########################

SRC_DIR		= src/

SRC_NAMES	= $(GENERAL_FILES) \
			$(addprefix $(COMMAND_DIR), $(COMMAND_FILES)) \

OBJ_DIR		= build/

OBJ_NAMES	= $(SRC_NAMES:.cpp=.o)

OBJ_FOLDERS	= $(addprefix $(OBJ_DIR), $(COMMAND_DIR))

OBJ		= $(addprefix $(OBJ_DIR), $(OBJ_NAMES))

#################################### MANDATORY ####################################

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
			@mkdir -p $(dir $@)
			@printf "$(ITAL)$(GREEN)Compiling: $(RESET)$(ITAL)$<\n"
			@$(CC) $(DEP_FLAGS) $(CFLAGS) -c $< -o $@


$(NAME): $(OBJ)
	$(CC) $(DEP_FLAGS) $(CFLAGS)  $^ -o $@

all:		$(NAME) 

# Include dependencies if they exist
-include $(OBJ:.o=.d)

.DEFAULT_GOAL := all

#################################### RULES ####################################

clean:
		rm -f $(OBJ_DIR)/*.o $(DEPS)
		rm -rf $(OBJ_DIR)
		@printf "$(BOLD)$(PINK)[IRC]: $(RESET)$(PINK)object files $(RESET)\t\t=> CLEANED! \n\n"

fclean: 	clean
			rm -f $(OBJ_DIR)/*.o $(DEPS)
			rm -rf $(OBJ_DIR)
			@printf "$(BOLD)$(BRIGHT_PURPLE)[IRC]: $(RESET)$(BRIGHT_PURPLE)exec. files $(RESET)\t=> CLEANED! \n\n"

re:		fclean all
			@printf "\n\n $(BOLD)$(YELLOW)Cleaning done! $(RESET)\n"
			
.PHONY:		all clean fclean re bonus