
ifeq ($(DJGPP),)
$(error ERROR: DJGPP not defined! ***)
endif

CC = gcc
#CFLAGS = -O2 -s -std=c99 -Wall -pedantic -finline -DDEBUG
CFLAGS = -O2 -std=gnu99 -Wall -finline 
LDFLAGS = -lm
RM=del
OBJS=viaxfg.o

all: viaxfg.exe

viaxfg.exe: $(OBJS)
	-$(CC) $(CFLAGS) -o viaxfg.exe $(OBJS) $(LDFLAGS)

%.o: %.c
	-$(CC) $(CFLAGS) -c $^

clean:
	-$(RM) *.exe
	-$(RM) *.o

