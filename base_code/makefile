CC = g++

SRCDIR = src
ODIR = obj
BIN = base
BINDIR = bin
IDIR = include

CFLAGS = `sdl2-config --cflags` -g -I$(IDIR)
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lm

_DEPS = GameObject.h GameEngine.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ = GameEngine.o GameObject.o main.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

.PHONY: all
all: $(BINDIR) $(ODIR) $(BINDIR)/$(BIN)

$(ODIR)/%.o: $(SRCDIR)/%.cpp $(DEPS) $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINDIR)/$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

run: all
	./$(BINDIR)/$(BIN)

$(ODIR):
	mkdir -p $@

$(BINDIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(ODIR) $(BINDIR)
