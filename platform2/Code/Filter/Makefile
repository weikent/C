### Makefile --- 

## Author: weishijian@weikents-Air.lan
## Version: $Id: Makefile,v 0.0 2017/03/15 05:13:02 weishijian Exp $
## Keywords: 
## X-URL: 






CC = gcc
CXX = g++
VPATH = 
INCLUDE = 
WALL = -Wall
C99 = #-std=c99
THREAD = -lpthread
OPTIMIZE = -O2



SOURCECPP =

SOURCEC =

OBJECTSCPP = $(SOURCECPP:%.cpp=%.o)

OBJECTSC = $(SOURCEC:%.c=%.o)

OBJS = $(OBJECTSC) $(OBJECTSCPP)


TARGET = 

.PHONY = all
.PHONY = clean
all :
	$(MAKE) -f Makefile.Filter
	$(MAKE) -f Makefile.forTest

%.o : %.c
	$(CC) -c $(WALL)  $< -o $@ $(INCLUDE) $(C99)

%.o : %.cpp
	$(CXX) -c $(WALL) $< -o $@ $(INCLUDE)

$(TARGET): $(OBJECTSC) $(OBJECTSCPP)
	$(CC) -o $(TARGET) $(INCLUDE)  $(OBJECTSC) $(OBJECTSCPP) $(THREAD) $(OPTIMIZE) $(C99)


clean:
	$(MAKE) clean -f Makefile.Filter
	$(MAKE) clean -f Makefile.forTest

install:
#	cp *.h *.cpp *.c -r ../wpa



### Makefile ends here
