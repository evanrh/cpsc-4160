CC = g++
CFLAGS = `sdl2-config --cflags` -g
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lm

SRCDIR = src
ODIR = obj
BIN = assignment1 
BINDIR = bin

_OBJ = main.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

.PHONY: all
all: $(BINDIR) $(ODIR) $(BINDIR)/$(BIN)

$(ODIR)/%.o: $(SRCDIR)/%.cpp $(ODIR)
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
