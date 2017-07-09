# Makefile for morphy library
NAME	:= libmorphy
CC		:= gcc
DBGF	:= -g -D DEBUG --coverage
CFLAGS	:= -c -Wall -fPIC #$(shell pkg-config --cflags glib-2.0)
SRC		:= *.c
OBJS	:= *.o
SRCDIR	:= ./src/
SNAME	:= $(NAME).a
DNAME	:= $(NAME).so
TDIRS	:= ./tests/
LIBS	:= #$(shell pkg-config --libs glib-2.0)
LDFLAGS	:= -L ./$(LIBS) 


.PHONY: all clean test run debug release

all: $(SNAME) $(DNAME)

debug: CFLAGS := $(CFLAGS) $(DBGF) #-fprofile-instr-generate
debug: LDFLAGS := $(LDFLAGS) --coverage
debug: all

release: CFLAGS := $(CFLAGS) -O3
release: all

$(SNAME) : $(OBJS)
	ar rvs $(SNAME) $(OBJS)

$(DNAME) : $(OBJS)
	$(CC) -shared -o $(DNAME) $(OBJS) $(LDFLAGS)

$(OBJS) : $(SRCDIR)$(SRC)
	$(CC) $(CFLAGS) $(SRCDIR)$(SRC) 
#$(LDFLAGS)

clean:
	rm *.o

test: 
	$(CC) -I./ -I./tests/ctreelib --coverage -o ./tests/utest ./tests/*.c ./tests/ctreelib/*.c  $(SNAME) $(LDFLAGS) -lm
run:
	$(TDIRS)tcreate
