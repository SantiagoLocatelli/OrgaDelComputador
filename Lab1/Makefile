CFLAGS := -ggdb3 -O2 -Wall -Wextra -std=c11
CFLAGS += -Wmissing-prototypes

# Usar POSIX.2008-1 como default.
CPPFLAGS := -D_POSIX_C_SOURCE=200809L

PROGS := cp57 ls57

all: $(PROGS)

cp57: cp57.o
ls57: ls57.o

clean:
	rm -f $(PROGS) *.o core

.PHONY: all clean
