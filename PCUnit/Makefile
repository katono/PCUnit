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

.PHONY: all clean install uninstall

all: $(TARGET)

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $<

$(TARGET): $(OBJS)
	$(AR) rcs $(TARGET) $^

INSTALLDIR = /usr/local

install:
	install -d $(INSTALLDIR)/include/PCUnit
	install -m 644 $(TARGET) $(INSTALLDIR)/lib
	install -m 644 PCUnit.h PCU_Libc.h $(INSTALLDIR)/include/PCUnit
	install -m 755 ../util/pcunit_*.rb $(INSTALLDIR)/bin

uninstall:
	rm $(INSTALLDIR)/lib/$(TARGET)
	rm -rf $(INSTALLDIR)/include/PCUnit
	rm $(INSTALLDIR)/bin/pcunit_*.rb

clean:
	rm -f *.o *.a

PCUnit.o: PCU_Suite.h PCU_Libc.h PCUnit.h
PCU_Test.o: PCU_Test.h PCU_Libc.h PCUnit.h
PCU_Suite.o: PCU_Test.h PCU_Suite.h PCU_Libc.h PCUnit.h
PCU_Libc.o: PCU_Libc.h PCUnit.h
