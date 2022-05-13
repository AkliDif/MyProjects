all : carcassonne

main.o : main.c 
	gcc -c main.c -o main.o `sdl2-config --cflags --libs` -lSDL2_image
Tuile.o : Tuile.c
	gcc -o Tuile.o -c Tuile.c

Player.o : Player.c
	gcc -o Player.o -c Player.c

Meeple.o : Meeple.c
	gcc -o Meeple.o -c Meeple.c

Pile.o: Pile.c
	gcc -o Pile.o -c Pile.c

Plateau.o: Plateau.c
	gcc -o Plateau.o -c Plateau.c

carcassonne: main.o Tuile.o Player.o Meeple.o Pile.o Plateau.o 
	gcc -o carcassonne main.o Tuile.o Player.o Meeple.o Pile.o Plateau.o `sdl2-config --cflags --libs` -lSDL2_image

clean :
	rm -rf *.o

mrproper : clean
	rm -rf carcassonne
