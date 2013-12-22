objects = local.o msgqueue.o getOtherID.o

local:$(objects)
	g++ -o local $(objects) -lpthread

local.o:
msgqueue.o:msgqueue.h
getOtherID.o:getOtherID.h


clean:
	rm local $(objects)

install:
	cp local ~
