objects = msgqueue.o getOtherID.o dataParse.o comm.o cJSON.o
VPATH = global
comm:$(objects)
	gcc -o comm $(objects) -lpthread -lm

cJSON.o:cJSON.h

dataParse.o:dataParse.h

comm.o:global.h

getOther.o:getOtherID.h

msgqueue.o:msgqueue.h



clean:
	rm comm $(objects)

install:
	cp comm ~
