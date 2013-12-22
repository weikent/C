objects = comm.o dataparse.o cJSON.o msgsnd_cmd.o Thread.o msgrcv_data.o msgqueue.o utility.o APModel.o global.o phoneDataParse.o

comm:$(objects)
	g++ -o comm $(objects) -lpthread -lm

global.o:global.h

dataparse.o:dataparse.h

cJSON.o:cJSON.h

comm.o:msgsnd_cmd.h

msgsnd_cmd.o:msgsnd_cmd.h

msgrcv_data.o:msgrcv_data.h

Thread.o:Thread.h

msgqueue.o:msgqueue.h

utility.o:utility.h

APModel.o:APModel.h

phoneDataParse.o:phoneDataParse.h

clean:
	rm comm $(objects)

install:
	cp comm ~
