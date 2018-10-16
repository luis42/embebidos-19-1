# Archivo Makefile simple

PROYECTO=proc1
DESTDIR=/usr/local/bin
CFLAGS=-Wall
LDFLAGS=-lm
CC=gcc

all: $(PROYECTO)


%.o: %.c 
	$(CC) -c $< $(CFLAGS)

#muestreo.o: muestreo.c
#	$(CC) -c muestreo.c $(CFLAGS)

#procesamiento.o: procesamiento.c
#	$(CC) -c procesamiento.c $(CFLAGS)

#archivos.o: archivos.c
#	$(CC) -c archivos.c $(CFLAGS)

#$(PROYECTO): muestreo.o procesamiento.o archivos.o
#	$(CC) muestreo.o procesamiento.o archivos.o -o $(PROYECTO) $(LDFLAGS)

#$(PROYECTO): muestreo.o procesamiento.o archivos.o
#	$(CC) $^ -o $@ $(LDFLAGS)

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

