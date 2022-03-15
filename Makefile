CROSS =   
CC = $(CROSS)gcc  
CXX = $(CROSS)g++  
DEBUG =
DEFINE = -D CPPHTTPLIB_OPENSSL_SUPPORT 
CPPFLAGS = $(DEBUG) -x c++ -w -c -fPIC $(DEFINE) -std=c++11
CFLAGS = $(DEBUG) -x c -w -c -fPIC $(DEFINE) 
MV = mv -f  
RM = rm -rf  
LN = ln -sf  

TARGET = onvif_http.out

TOP_PATH = $(shell pwd)
SRC_PATH = $(TOP_PATH)
INC_PATH = 

DIRS = $(shell find $(SRC_PATH) -maxdepth 1 -type d)

FILES = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp $(dir)/*.c))


##########################################################  
# srcs  all .cpp .cc and .c files
##########################################################  
SRCS_CPP += $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))  
SRCS_CC += $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cc))  
SRCS_C += $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))  

##########################################################  
# objs  all *.o files
##########################################################  
OBJS_CPP = $(patsubst %.cpp, %.o, $(SRCS_CPP))  
OBJS_CC = $(patsubst %.cc, %.o, $(SRCS_CC))  
OBJS_C = $(patsubst %.c, %.o, $(SRCS_C))  

##########################################################  
# paths
##########################################################  

LIB_PATH += -L$(TOP_PATH)/lib/


##########################################################  
# libs
##########################################################  
LIBS     = -lssl -lcrypto -lpthread
##########################################################  
# building
##########################################################  
all:$(TARGET)

#1. link all o to out
#2. cp so to /usr/local/lib 
$(TARGET) : $(OBJS_CPP) $(OBJS_CC) $(OBJS_C)
	$(CXX)  $^ -o $(TARGET) $(LIB_PATH) $(LIBS)
	

#compile all cpp to o
$(OBJS_CPP):%.o : %.cpp
	$(CXX) $(CPPFLAGS) $< -o  $@ $(INC_PATH)

#compile all cc to o
$(OBJS_CC):%.o : %.cc
	$(CXX) $(CFLAGS) $< -o  $@ $(INC_PATH)

#compile all c to o
$(OBJS_C):%.o : %.c
	$(CXX) $(CFLAGS) $< -o  $@ $(INC_PATH)

.PHONY : clean  
clean:  
	@ $(RM) $(TARGET) $(OBJS_C) $(OBJS_CC) $(OBJS_CPP) 
