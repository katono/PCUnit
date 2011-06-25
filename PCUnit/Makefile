CC = gcc
AR = ar
CFLAGS = -Wall -std=c99 -pedantic-errors -Os
# CFLAGS += -DPCU_NO_LIBC
TARGET = libpcunit.a
OBJS = PCUnit.o
OBJS += PCU_Test.o
OBJS += PCU_TestCase.o
OBJS += PCU_Libc.o

all: $(TARGET)

.SUFFIXES: .c .o

.c .o:
	$(CC) $(CFLAGS) -c $<


$(TARGET): $(OBJS)
	$(AR) rcs $(TARGET) $^


.PHONY: clean
clean:
	rm -f *.o *.a

PCUnit.o: PCU_TestCase.h PCU_Libc.h PCUnit.h
PCU_Test.h: PCUnit.h
PCU_Test.o: PCU_Test.h PCU_Libc.h PCUnit.h
PCU_TestCase.h: PCUnit.h
PCU_TestCase.o: PCU_Test.h PCU_TestCase.h PCU_Libc.h PCUnit.h
PCU_Libc.o: PCU_Libc.h PCUnit.h
