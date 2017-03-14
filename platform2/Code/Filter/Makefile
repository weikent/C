CC=gcc

INCLUDE=
RM=rm -f
WALL= -Wall
PTHREAD= #-lpthread

SOURCEC=main.c socketSerialPort.c global.c msgQ.c

SOURCECPP=

OBJECTSC=$(SOURCEC:%.c=%.o)
OBJECTSCPP=$(SOURCECPP:%.cpp=%.o)

OBJS = $(OBJECTSC) $(OBJECTSCPP)

TARGET=test

.PHONY = all
.PHONY = clean
all: clean $(TARGET)

%.o : %.c
	$(CC) -c $(WALL)  $< -o $@ $(INCLUDE) $(C99)

%.o : %.cpp
	$(CXX) -c $(WALL) $< -o $@ $(INCLUDE)

$(TARGET): $(OBJECTSC) $(OBJECTSCPP)
	$(CC) -o $(TARGET) $(INCLUDE)  $(OBJECTSC) $(OBJECTSCPP)  $(PTHREAD) -lm -O2 $(C99)


clean:
#	rm $(OBJECTSC) $(OBJECTSCPP) $(TARGET)
	$(RM) $(OBJS) $(TARGET)

install:
#	cp *.h *.cpp *.c -r ../wpa


main:$(TARGET)
