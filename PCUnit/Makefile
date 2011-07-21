CC = gcc
AR = ar
CFLAGS = -Wall -Wextra -std=c99 -pedantic-errors -Os
# CFLAGS += -DPCU_NO_LIBC
# CFLAGS += -DPCU_NO_VSNPRINTF
# CFLAGS += -DPCU_NO_MALLOC
# CFLAGS += -DPCU_NO_SETJMP
# CFLAGS += -DPCU_NO_FLOATINGPOINT
TARGET = libpcunit.a
OBJS = PCUnit.o
OBJS += PCU_Test.o
OBJS += PCU_Suite.o
OBJS += PCU_Libc.o

all: $(TARGET)

.SUFFIXES: .c .o

.c .o:
	$(CC) $(CFLAGS) -c $<


$(TARGET): $(OBJS)
	$(AR) rcs $(TARGET) $^


install:
	cp $(TARGET) /usr/local/lib
	mkdir -p /usr/local/include/PCUnit
	cp PCUnit.h PCU_Libc.h /usr/local/include/PCUnit


uninstall:
	rm /usr/local/lib/$(TARGET)
	rm -rf /usr/local/include/PCUnit


.PHONY: clean
clean:
	rm -f *.o *.a

PCUnit.o: PCU_Suite.h PCU_Libc.h PCUnit.h
PCU_Test.o: PCU_Test.h PCU_Libc.h PCUnit.h
PCU_Suite.o: PCU_Test.h PCU_Suite.h PCU_Libc.h PCUnit.h
PCU_Libc.o: PCU_Libc.h PCUnit.h
