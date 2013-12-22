objects = line.o word.o justify.o


justify:$(objects)
	gcc -o justify $(objects)

justify.o:line.h word.h

word.o:word.h

line.o:line.h


clean:
	rm justify $(objects)
