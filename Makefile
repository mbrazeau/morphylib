# Makefile for treelib
NAME	:= libmorphy_$(shell uname -m)-$(shell uname -s)
CC		:= gcc
DBGF	:= -g -D DEBUG
CFLAGS	:= -c -Wall -fPIC
SRC		:= *.c
OBJS	:= *.o
SNAME	:= $(NAME).a
DNAME	:= $(NAME).so
TDIRS	:= ./tests/
LDFLAGS	:= -L./
#LDLIBS	:=-lmorphy_x86_64-Darwin  

.PHONY: all clean test run debug

all: $(SNAME) $(DNAME)

debug: CFLAGS := $(CFLAGS) $(DBGF)
debug: all 

$(SNAME) : $(OBJS)
	ar rvs ${SNAME} ${OBJS} 

$(DNAME) : $(OBJS)
	$(CC) -shared -o $(DNAME) $(OBJS) 

$(OBJS) : $(SRC)
	$(CC) $(CFLAGS) $(SRC)

clean:
	rm *.o

test: $(DNAME)
	$(CC) $(LDFLAGS) -o $(TDIRS)tcreate $(TDIRS)tcreate.c -lmorphy_x86_64-Darwin 
	$(CC) $(LDFLAGS) -o ./unittests/utest ./unittests/statedata.c -lmorphy_x86_64-Darwin
run:
	$(TDIRS)tcreate
