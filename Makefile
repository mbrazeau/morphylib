# Makefile for morphy library
NAME	:= libmorphy_$(shell uname -m)-$(shell uname -s)
CC		:= gcc
DBGF	:= -g -D DEBUG
CFLAGS	:= -c -Wall -fPIC #$(shell pkg-config --cflags glib-2.0)
SRC		:= *.c
OBJS	:= *.o
SRCDIR	:= ./src/
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

$(OBJS) : $(SRCDIR)$(SRC)
	$(CC) $(CFLAGS) $(SRCDIR)$(SRC)

clean:
	rm *.o

test: $(DNAME)
	$(CC) $(LDFLAGS) -o $(TDIRS)tcreate $(TDIRS)tcreate.c -lmorphy_x86_64-Darwin 
	$(CC) $(LDFLAGS) -I./ -o ./mtests/utest ./mtests/statedata.c libmorphy_x86_64-Darwin.a
run:
	$(TDIRS)tcreate
