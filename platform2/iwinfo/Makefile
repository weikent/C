IWINFO_BACKENDS    = $(BACKENDS)
IWINFO_CFLAGS      = $(CFLAGS) -std=gnu99 -fstrict-aliasing -Iinclude -I/home/startech1/SDK7688/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/include/ -fPIC
IWINFO_LDFLAGS     = -luci -lubox

IWINFO_LIB         = libiwinfo.so
IWINFO_LIB_LDFLAGS = $(LDFLAGS) -shared -L/home/startech1/SDK7688/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/lib
IWINFO_LIB_OBJ     = iwinfo_utils.o iwinfo_wext.o iwinfo_wext_scan.o iwinfo_lib.o

IWINFO_LUA         = iwinfo.so
IWINFO_LUA_LDFLAGS = $(LDFLAGS) -shared -L. -L/home/startech1/SDK7688/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/lib -liwinfo -llua
IWINFO_LUA_OBJ     = iwinfo_lua.o

IWINFO_CLI         = iwinfo
IWINFO_CLI_LDFLAGS = $(LDFLAGS) -L. -liwinfo
IWINFO_CLI_OBJ     = iwinfo_cli.o
CC 		   = mipsel-openwrt-linux-gcc



%.o: %.c
	$(CC) $(IWINFO_CFLAGS) $(FPIC) -c -o $@ $<

compile: clean $(IWINFO_LIB_OBJ) $(IWINFO_LUA_OBJ) $(IWINFO_CLI_OBJ)
	$(CC) $(IWINFO_LDFLAGS) $(IWINFO_LIB_LDFLAGS) -o $(IWINFO_LIB) $(IWINFO_LIB_OBJ)
	$(CC) $(IWINFO_LDFLAGS) $(IWINFO_LUA_LDFLAGS) -o $(IWINFO_LUA) $(IWINFO_LUA_OBJ)
	#$(CC) $(IWINFO_LDFLAGS) $(IWINFO_CLI_LDFLAGS) -o $(IWINFO_CLI) $(IWINFO_CLI_OBJ)
	$(CC) -o iwinfo iwinfo_cli.o -L. -L/home/startech1/SDK7688/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/lib -liwinfo -luci -lubox

clean:
	rm -f *.o $(IWINFO_LIB) $(IWINFO_LUA) $(IWINFO_CLI)
