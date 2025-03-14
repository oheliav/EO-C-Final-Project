CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
OBJECTS = main.o preAssembler.o secondPass.o firstPass.o util.o bitUtils.o dataStructures.o errors.o

assembler: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o assembler

main.o: main.c globals.h firstPass.h secondPass.h preAssembler.h util.h bitUtils.h dataStructures.h errors.h
	$(CC) $(CFLAGS) -c main.c

preAssembler.o: preAssembler.c preAssembler.h globals.h dataStructures.h
	$(CC) $(CFLAGS) -c preAssembler.c

secondPass.o: secondPass.c secondPass.h globals.h dataStructures.h util.h bitUtils.h
	$(CC) $(CFLAGS) -c secondPass.c

firstPass.o: firstPass.c firstPass.h globals.h dataStructures.h util.h bitUtils.h
	$(CC) $(CFLAGS) -c firstPass.c

util.o: util.c util.h globals.h bitUtils.h dataStructures.h
	$(CC) $(CFLAGS) -c util.c

bitUtils.o: bitUtils.c bitUtils.h
	$(CC) $(CFLAGS) -c bitUtils.c

dataStructures.o: dataStructures.c dataStructures.h globals.h
	$(CC) $(CFLAGS) -c dataStructures.c

errors.o: errors.c errors.h globals.h
	$(CC) $(CFLAGS) -c errors.c

clean:
	rm -f $(OBJECTS) assembler
