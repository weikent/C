### Makefile --- 

## Author: weishijian@weikents-Air.lan
## Version: $Id: Makefile,v 0.0 2017/03/16 08:18:16 weishijian Exp $
## Keywords: 
## X-URL: 






#CC = gcc
CC=mipsel-openwrt-linux-gcc
CXX = g++

VPATH = 
INCLUDE = -I. -L. -Iinclude -I/home/weishijian/SDK7688/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/include/ -L/home/weishijian/SDK7688/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/lib -fPIC
WALL = -Wall
C99 = #-std=c99
THREAD = -lpthread
OPTIMIZE = -O2

RM = rm -f


SOURCECPP =

SOURCEC = ReadSocketBuffer.c packMessage.c global.c comm.c SendHeart.c ConnectionCheck.c SendSocket.c TcpClient.c tool.c ../../common/md5.c ../../common/msgQ.c DeviceModelChange.c ../../common/dealJson.c ../../common/cJSON.c

OBJECTSCPP = $(SOURCECPP:%.cpp=%.o)

OBJECTSC = $(SOURCEC:%.c=%.o)

OBJS = $(OBJECTSC) $(OBJECTSCPP)


TARGET = comm

.PHONY = all
.PHONY = clean
all = clean $(TARGET)

%.o : %.c
	$(CC) -c $(WALL)  $< -o $@ $(INCLUDE) $(C99)

%.o : %.cpp
	$(CXX) -c $(WALL) $< -o $@ $(INCLUDE)

$(TARGET): $(OBJECTSC) $(OBJECTSCPP)
	$(CC) -o $(TARGET) $(INCLUDE)  $(OBJECTSC) $(OBJECTSCPP) $(THREAD) $(OPTIMIZE) $(C99) -lm -ldl -liwinfo -luci -lubox


clean:
	$(RM) $(OBJS) $(TARGET)

install:
#	cp *.h *.cpp *.c -r ../wpa




### Makefile ends here
