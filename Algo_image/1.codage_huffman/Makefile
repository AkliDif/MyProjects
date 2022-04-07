all : huffman

main.o : main.c
	gcc -o main.o -c main.c -g -Wall

TP1.o : TP1.c
	gcc -o TP1.o -c TP1.c

TP2.o : TP2.c
	gcc -o TP2.o -c TP2.c


huffman: main.o TP1.o TP2.o
	gcc -o huffman main.o TP1.o TP2.o
clean :
	rm -rf *.o

mrproper : clean
	rm -rf carcassonne
