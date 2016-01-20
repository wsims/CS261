all: prog 

hashMap.o: hashMap.c hashMap.h 
	gcc -g -Wall -std=c99 -c hashMap.c
	
main.o: main.c hashMap.h
	gcc -g -Wall -std=c99 -c main.c
		
prog: hashMap.o main.o
	gcc -g -Wall -std=c99 -o prog hashMap.o  main.o

clean:
	rm prog
	rm hashMap.o
	rm main.o
