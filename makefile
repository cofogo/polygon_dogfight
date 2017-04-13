SRC = src/*.cpp
INCLUDE = -I include

#CC specifies which compiler we are using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we are using
# -w suppress all warnings
COMPILER_FLAGS = -std=c++11

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = polyfight

#This is the target that compiles our executable
all : $(SRC)
	$(CC) $(SRC) $(INCLUDE) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

debug : $(OBJS)
	$(CC) -g $(SRC) $(INCLUDE) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
