#for multchannel decoder makefile by pkf
BIN = server_system

#BASE_INSTALL_DIR    := /opt/arm-2009q1

#BASE_INSTALL_DIR    := /opt/arm-2009q1-368/arm-2009q1

BUILD_TOOL_DIR      := $(BASE_INSTALL_DIR)

#BUILD_TOOL_PREFIX   := /opt/arm-2009q1/bin/arm-none-linux-gnueabi-
#BUILD_TOOL_PREFIX   := $(BUILD_TOOL_DIR)/bin/arm-none-linux-gnueabi-
BUILD_TOOL_PREFIX   := arm-hisiv300-linux-uclibcgnueabi-

CC = $(BUILD_TOOL_PREFIX)gcc
CPP = $(BUILD_TOOL_PREFIX)c++  

LIBSO = librtspserver.so
INCS =   
LIBS = -lpthread
SUBDIRS =  

DEFINC = -I"./BasicUsageEnvironment/include" -I"./groupsock/include" -I"./liveMedia/include" \
-I"./UsageEnvironment/include" -I"./appro/"  -I"../interface/inc"  -I"./liveMedia/" 
#-I"./myclient"   


INC_DIR := UsageEnvironment BasicUsageEnvironment liveMedia groupsock
INC := $(patsubst %,-I%/include,$(INC_DIR))

#CFLAGS := $(INC) -Wall -I $(KDIR)/include  -DBSD=1 -DSOCKLEN_T=socklen_t -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64  -lpthread -I./ -g3 -O3


CSRCS = $(wildcard  ./*.c ./BasicUsageEnvironment/*.c ./groupsock/*.c ./liveMedia/*.c \
./UsageEnvironment/*.c ./appro/*.c ./myclient/*.c)  
CPPSRCS = $(wildcard ./*.cpp ./BasicUsageEnvironment/*.cpp ./groupsock/*.cpp ./liveMedia/*.cpp \
./UsageEnvironment/*.cpp ./appro/*.cpp ./myclient/*.cpp,./testProgs/testOnDemandRTSPServer.cpp)  


   
COBJS := $(CSRCS:.c=.o)  
CPPOBJS := $(CPPSRCS:.cpp=.o)  



MAKEDEPEND = gcc -MM -MT  
CFLAGS = -O2 -Wall -DBSD=1 -DLOCALE_NOT_USED -DSOCKLEN_T=socklen_t -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64

CPPFLAGS = -O2 -Wall -DBSD=1 -DALLOW_SERVER_PORT_REUSE -DLOCALE_NOT_USED -DSOCKLEN_T=socklen_t -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64


CDEF = $(CSRCS:.c=.d)  
CPPDEF = $(CPPSRCS:.cpp=.d)  
PLATS = win32-debug win32-release linux-debug linux-release  
none:  
	@echo "Please choose a platform"
	@echo $(PLATS)  
win32-debug:  
	$(MAKE) all INCS=-I"c:/mingw/include" LIBS="-L"c:/mingw/lib" -L"./../../lib/lxnet" -llxnet -lws2_32 -L"./../../lib/tinyxml" -ltinyxml" CFLAGS="-Wall -DWIN32 -DDEBUG -g" CPPFLAGS="-Wall -DWIN32 -DDEBUG -g"  
win32-release:  
	$(MAKE) all INCS=-I"c:/mingw/include" LIBS="-L"c:/mingw/lib" -L"./../../lib/lxnet" -llxnet -lws2_32 -L"./../../lib/tinyxml" -ltinyxml" CFLAGS="-Wall -DWIN32 -DNDEBUG -O2" CPPFLAGS="-Wall -DWIN32 -DNDEBUG -O2"  
linux-debug:  
	$(MAKE) all  -lpthread  CFLAGS="-Wall -DDEBUG -g" CPPFLAGS="-Wall -DDEBUG -g"  
linux-release:
	$(MAKE) all  -lpthread  CFLAGS="-Wall -DNDEBUG -O2" CPPFLAGS="-Wall -DNDEBUG -O2 -DBSD=1 -DSOCKLEN_T=socklen_t -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64"


all:$(BIN)  fsupdate1
lib:$(LIBSO) fsupdate2
 
$(CDEF) : %.d : %.c
	$(MAKEDEPEND) $(<:.c=.o) $< $(DEFINC) > $@  
$(CPPDEF) : %.d : %.cpp  
	$(MAKEDEPEND) $(<:.cpp=.o) $< $(DEFINC) > $@  

	
depend:  
	-rm $(CDEF)  
	-rm $(CPPDEF)  
	$(MAKE) $(CDEF)  
	$(MAKE) $(CPPDEF)  
$(COBJS) : %.o: %.c  
	$(CC) -c $< -o $@ $(INCS) $(DEFINC) $(CFLAGS)  
$(CPPOBJS) : %.o: %.cpp  
	$(CPP) -c $< -o $@ $(INCS) $(DEFINC) $(CPPFLAGS)  

$(BIN) : $(COBJS) $(CPPOBJS)  
	$(CPP) -o $(BIN) $(COBJS) $(CPPOBJS) $(LIBS)  
	-rm $(COBJS) $(CPPOBJS)  
	
$(LIBSO): $(COBJS) $(CPPOBJS)
	$(CPP) -shared -fPIC -nostartfiles -o $@ $^  $(LIBS)
	

fsupdate1:
#	cp $(LIBSO) ../../../tirvideo_sdk/bell_lyb/rdk/demos/link_api_test_demos/linkapi
#	cp $(LIBSO) /home/tirvideo/lyb/armfs/opt/dvr_rdk/ti814x
#	cp $(BIN)	/home/tirvideo/lyb/armfs/opt/dvr_rdk/ti814x
fsupdate2:
	#cp $(LIBSO) /opt/tirvideo_sdk/rdk/demos/mcfw_api_demos/mcfw_demo
#	cp $(LIBSO) /home/tirvideo/lyb/armfs/opt/dvr_rdk/ti814x
#	cp $(BIN)	/home/tirvideo/lyb/armfs/opt/dvr_rdk/ti814x

.PHONY:clean cleanall  

clean:            
	-rm $(BIN) $(COBJS) $(CPPOBJS) $(LIBSO)
	-rm *.d  
	-rm *.obj
	-rm *.o
	-rm *.dep
	-rm *.dep *.obj *.d

