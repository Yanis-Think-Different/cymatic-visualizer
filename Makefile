CC = gcc
CFLAGS = -Wall -pedantic -std=c17 -I include/
COBJETS = build/main.o build/audio.o build/affichage.o build/physic.o
CSCREEN = -lraylib

visual : $(COBJETS)
	$(CC) $(CFLAGS) $(COBJETS) -o test $(CSCREEN)

build/audio.o : src/audio.c include/audio.h 
	$(CC) $(CFLAGS) -c src/audio.c -o build/audio.o

build/affichage.o : src/affichage.c include/affichage.h 
	$(CC) $(CFLAGS) -c src/affichage.c -o build/affichage.o $(CSCREEN)

build/physic.o : src/physic.c include/physic.h 
	$(CC) $(CFLAGS) -c src/physic.c -o build/physic.o

build/math.o : src/math.c include/math.h 
	$(CC) $(CFLAGS) -c src/math.c -o build/math.o

build/main.o : src/main.c include/affichage.h include/audio.h include/physic.h
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o $(CSCREEN)

clean:
	rm -f *.o test

