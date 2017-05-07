# Makefile for morphy library
NAME	:= libmorphy_$(shell uname -m)-$(shell uname -s)
CC		:= gcc
DBGF	:= -g -D DEBUG
CFLAGS	:= -c -Wall -fPIC #$(shell pkg-config --cflags glib-2.0)
SRC		:= *.c
OBJS	:= *.o
SNAME	:= $(NAME).a
DNAME	:= $(NAME).so
TDIRS	:= ./tests/
LDFLAGS	:= -L./ 
LIBS	:= $(shell pkg-config --libs glib-2.0)


.PHONY: all clean test run debug

all: $(SNAME) $(DNAME)

debug: CFLAGS := $(CFLAGS) $(DBGF)
debug: all 

$(SNAME) : $(OBJS)
	ar rvs $(SNAME) $(OBJS)

$(DNAME) : $(OBJS)
	$(CC) -shared -o $(DNAME) $(OBJS) 

$(OBJS) : $(SRC)
	$(CC) $(CFLAGS) $(SRC)

clean:
	rm *.o

test: $(DNAME)
	$(CC) $(LDFLAGS) -o $(TDIRS)tcreate $(TDIRS)tcreate.c -lmorphy_x86_64-Darwin 
	$(CC) $(LDFLAGS) -I./ -o ./unittests/utest ./unittests/statedata.c libmorphy_x86_64-Darwin.a
run:
	$(TDIRS)tcreate
