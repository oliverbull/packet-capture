IDIR =./include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=./obj
LDIR=./lib
SDIR=./src

LIBS=-lm
DEBUG=-g3

_DEPS = logging.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o logging.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(DEBUG) -c -o $@ $< $(CFLAGS)

packetcapture: $(OBJ)
	$(CC) $(DEBUG) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o packetcapture
