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
LIBS	:= #$(shell pkg-config --libs glib-2.0)
LDFLAGS	:= -L .$(LIBS) 


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
#$(LDFLAGS)

clean:
	rm *.o

test: $(SNAME)
	$(CC) $(LDFLAGS) -I./ -o ./tests/utest ./tests/*.c $(SNAME) $(LDFLAGS)
run:
	$(TDIRS)tcreate
