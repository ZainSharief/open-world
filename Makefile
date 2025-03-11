CC = C:\MinGW\bin\g++.exe
CFLAGS = -g -std=c++17

INCLUDE_DIR = include
LIB_DIR = lib
SRC_DIR = src
OUTPUT = open-world.exe

# Include all .cpp files and glad.c
SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(SRC_DIR)/glad.c
LIBS = -lglfw3dll

$(OUTPUT): $(SRCS)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) $(SRCS) $(LIBS) -o $(OUTPUT)

clean:
	del $(OUTPUT)
