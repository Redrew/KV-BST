CC=gcc
CFLAGS=-I. -g
all:dict1 dict2
dict1:dict1.o dictionary.o dataio.o
	$(CC) -o dict1 dict1.o dictionary.o dataio.o $(CFLAGS)
dict1.o:dict1.c
	$(CC) -c dict1.c $(CFLAGS)
dict2:dict2.o dictionary.o dataio.o
	$(CC) -o dict2 dict2.o dictionary.o dataio.o $(CFLAGS)
dict2.o:dict2.c
	$(CC) -c dict2.c $(CFLAGS)
dictionary.o:dictionary.c
	$(CC) -c dictionary.c $(CFLAGS)
dataio.o:dataio.c
	$(CC) -c dataio.c $(CFLAGS)
clean:
	rm -f *.o *.exe