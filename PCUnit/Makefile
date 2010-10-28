CFLAGS = -Wall -ansi -pedantic-errors -g
# CFLAGS += -DPCU_NO_STDLIB
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

PCUnit.o: include/PCU_TestCase.h include/PCU_Libc.h include/PCUnit.h
include/PCU_Test.h: include/PCUnit.h
PCU_Test.o: include/PCU_Test.h include/PCU_Libc.h include/PCUnit.h
include/PCU_TestCase.h: include/PCUnit.h
PCU_TestCase.o: include/PCU_Test.h include/PCU_TestCase.h include/PCU_Libc.h include/PCUnit.h
PCU_Libc.o: include/PCU_Libc.h include/PCUnit.h
