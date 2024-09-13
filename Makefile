NAME    		= ircserv
FLAGS   		= -Wall -Wextra -Werror -g -std=c++98 -I includes
RM      		= rm -rf
CC      		= g++

SRCS_DIR 		= srcs/
SERVER_DIR 		= $(SRCS_DIR)server/
CMD_DIR 		= $(SERVER_DIR)cmd/
SRV_UTILS_DIR 	= $(SERVER_DIR)utils/

SRCS    = $(SRCS_DIR)main.cpp \
		  $(SRCS_DIR)Client.cpp \
		  $(SRCS_DIR)Channel.cpp \
		  $(SRCS_DIR)argsValidation.cpp \
		  $(SERVER_DIR)Server.cpp \
		  $(CMD_DIR)auth.cpp \
		  $(CMD_DIR)exec.cpp \
		  $(CMD_DIR)invite.cpp \
		  $(CMD_DIR)join.cpp \
		  $(CMD_DIR)kick.cpp \
		  $(CMD_DIR)nick.cpp \
		  $(CMD_DIR)part.cpp \
		  $(CMD_DIR)quit.cpp \
		  $(CMD_DIR)topic.cpp \
		  $(CMD_DIR)user.cpp \
		  $(CMD_DIR)mode.cpp \
		  $(CMD_DIR)privmsg.cpp \
		  $(SRV_UTILS_DIR)splitters.cpp

OBJ_DIR  = objs/

OBJ_FILES = $(addprefix $(OBJ_DIR), $(notdir $(SRCS:.cpp=.o)))

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ_FILES)
	$(CC) $(FLAGS) $(OBJ_FILES) -o $(NAME)

$(OBJ_DIR)%.o: $(SRCS_DIR)%.cpp | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR)%.o: $(SERVER_DIR)%.cpp | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR)%.o: $(CMD_DIR)%.cpp | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR)%.o: $(SRV_UTILS_DIR)%.cpp | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
