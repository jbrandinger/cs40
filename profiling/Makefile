#
# makefile for profiling homework
#
CC = gcc -O2

IFLAGS  = -I/comp/40/build/include -I/usr/sup/cii40/include/cii
CFLAGS  = -g -std=gnu99 -Wall -Wextra -Werror -pedantic $(IFLAGS)
LDFLAGS = -g -L/comp/40/build/lib -L/usr/sup/cii40/lib64
LDLIBS  = -lcii40-O2 -lbitpack -l40locality -lcii40 -lm

EXECS   = um segments

all: $(EXECS)

um: um_main.o um_execution.o um_initialize.o um_segments.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

segments: um_segments.o um_segments_tests.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECS)  *.o
