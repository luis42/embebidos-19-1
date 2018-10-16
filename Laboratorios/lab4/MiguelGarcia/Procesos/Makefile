# Archivo Makefile simple

PROYECTO=proc
objects := $(patsubst %.c,%.o,$(wildcard *.c))
DESTDIR=/usr/local/bin
CFLAGS=-Wall
LDFLAGS=-lm
CC=gcc

all: $(PROYECTO)


%.o: %.c 
	$(CC) -c $< $(CFLAGS)

$(PROYECTO): $(objects)
	$(CC) $^ -o $@ $(LDFLAGS)

.PHONY: clean

install: all
	if [ ! -d $(DESTDIR) ] ; then \
		sudo mkdir $(DESTDIR); \
	fi; \
	sudo cp $(PROYECTO) $(DESTDIR)
unistall:
	sudo rm $(DESTDIR)$(PROYECTO)
clean:
	rm -f *.o $(PROYECTO)

