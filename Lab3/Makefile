-include deps.mk

CFLAGS := -g -O2 -std=c11 -Wall -Wextra
CFLAGS += -Wno-switch -Wno-conversion
ALUOBJ ?= recode57_alu.o

recode57: recode57.o $(ALUOBJ)

test: recode57
	./test_recode.sh

clean::
	rm -f recode57 core *.o

.PHONY: test clean
