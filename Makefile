##
## EPITECH PROJECT, 2025
## Makefile
## File description:
## The main Makefile of our project
##

CLIENT	=	felon_client
SERVER	=	felon_server

CC		=	g++

SFML	=	-lsfml-graphics -lsfml-window -lsfml-system
RAYLIB	=	-lraylib
DEBUGFLAGS	=	-Wno-unused-parameter -Wno-return-type -Wno-unused-variable
CFLAGS  = -std=c++20 -g3 -Wall -Wextra $(RAYLIB) -Iinclude/ -Iinclude/ECS -Iinclude/Systems -Iinclude/Server -Iinclude/Client
LIBS	=

COMMON_SRC	=	src/common/Components.cpp			\
				src/common/Game.cpp					\
				src/common/Utils.cpp				\
				src/common/Server.cpp				\
				src/common/Logger.cpp				\

CLIENT_SRC	=	src/Client/main.cpp					\

SERVER_SRC	=	src/Server/main.cpp					\

CLIENT_OBJ	=	$(CLIENT_SRC:%.cpp=%.o) $(COMMON_SRC:%.cpp=%.o)
SERVER_OBJ	=	$(SERVER_SRC:%.cpp=%.o) $(COMMON_SRC:%.cpp=%.o)


all:	$(CLIENT) $(SERVER)

$(CLIENT_OBJ): %.o: %.cpp
	@echo -e "\033[36mCompiling file \033[32m'$<'\033[36m\n\tas\t \033[93m'$@'\033[0m"
	@$(CC) -o $@ -c $< $(CFLAGS) $(DEBUGFLAGS)

$(SERVER_OBJ): %.o: %.cpp
	@echo -e "\033[36mCompiling file \033[32m'$<'\033[36m\n\tas\t \033[93m'$@'\033[0m"
	@$(CC) -o $@ -c $< $(CFLAGS) $(DEBUGFLAGS)

$(CLIENT): $(CLIENT_OBJ)
	@echo -e "\033[36mCompiling binary ... \033[93m'$(CLIENT)'\033[0m"
	@$(CC) -o $(CLIENT) $(CLIENT_OBJ) $(LIBS) $(CFLAGS) $(DEBUGFLAGS)
	@echo -e "\033[31mDONE.\033[0m"

$(SERVER): $(SERVER_OBJ)
	@echo -e "\033[36mCompiling binary ... \033[93m'$(SERVER)'\033[0m"
	@$(CC) -o $(SERVER) $(SERVER_OBJ) $(LIBS) $(CFLAGS) $(DEBUGFLAGS)
	@echo -e "\033[31mDONE.\033[0m"

clean:
	@echo -e "\033[36mCleaning object files..."
	@rm -f $(CLIENT_OBJ) $(SERVER_OBJ)

fclean: clean
	@echo -e "\033[36mRemoving binaries..."
	@rm -f $(CLIENT) $(SERVER)

run:	all
	@echo -e "Starting game..."
	@./$(SERVER) &
	@./$(CLIENT)

re: fclean all
