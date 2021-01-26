CC = g++

SRCS = main.cpp
BIN = project1
CFLAGS = `sdl2-config --cflags --libs` -lSDL2_image -g

all: $(BIN)

$(BIN): $(SRCS)
	 $(CC) $^ $(CFLAGS)
