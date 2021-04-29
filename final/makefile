CC = g++

SRCDIR := src
ODIR := obj
BIN := final
IDIR := include
LIBDIR := lib

CFLAGS = `sdl2-config --cflags` -g -I$(IDIR) --std=c++17
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lm -L$(LIBDIR)

_LIBS := tinyxml2.a
LIBS := $(patsubst %, $(LIBDIR)/%, $(_LIBS))

_DEPS = GameObject.h GameEngine.h Player.h Sprite.h globals.h parsing.h tileset.h TextureController.h layers.h camera.h ui.h
DEPS := $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ = GameEngine.o GameObject.o Player.o Sprite.o parsing.o TextureController.o layers.o camera.o tileset.o ui.o main.o
OBJ := $(patsubst %, $(ODIR)/%, $(_OBJ))

.PHONY: all
all: $(ODIR) $(LIBDIR) $(LIBS) $(BIN)

$(LIBDIR)/tinyxml2.a: $(SRCDIR)/tinyxml2/tinyxml2.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

run: all
	./$(BIN) ./res/joust.xml

$(ODIR):
	mkdir -p $@

$(LIBDIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(ODIR) $(BIN) $(LIBS)
