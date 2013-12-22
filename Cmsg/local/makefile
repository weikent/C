objects = msgqueue.o getOtherID.o local.o 
VPATH = global
comm:$(objects)
	gcc -o local $(objects) -lpthread

local.o:global.h

getOther.o:getOtherID.h

msgqueue.o:msgqueue.h



clean:
	rm comm $(objects)

install:
	cp comm ~
