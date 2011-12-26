CC = gcc
AR = ar
CFLAGS = -Wall -Wextra -std=c99 -pedantic-errors -Os
# CFLAGS += -DPCU_NO_LIBC
# CFLAGS += -DPCU_NO_VSNPRINTF
# CFLAGS += -DPCU_NO_MALLOC
# CFLAGS += -DPCU_NO_SETJMP
# CFLAGS += -DPCU_NO_WCHAR
# CFLAGS += -DPCU_NO_FLOATINGPOINT
# CFLAGS += -DPCU_NO_DIV32
# CFLAGS += -DPCU_NO_STDARG
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
	install -d $(INSTALLDIR)/lib
	install -d $(INSTALLDIR)/include/PCUnit
	install -d $(INSTALLDIR)/bin
	install -m 644 $(TARGET) $(INSTALLDIR)/lib
	install -m 644 PCUnit.h PCU_Libc.h $(INSTALLDIR)/include/PCUnit
	if [ -d ../util ]; then\
		if [ -e ../util/pcunit_template.rb ]; then\
			install -m 755 ../util/pcunit_template.rb $(INSTALLDIR)/bin;\
		fi;\
		if [ -e ../util/pcunit_register.rb ]; then\
			install -m 755 ../util/pcunit_register.rb $(INSTALLDIR)/bin;\
		fi;\
	fi

uninstall:
	rm -f $(INSTALLDIR)/lib/$(TARGET)
	rm -rf $(INSTALLDIR)/include/PCUnit
	rm -f $(INSTALLDIR)/bin/pcunit_*.rb

clean:
	rm -f *.o *.a

PCUnit.o: PCU_Suite.h PCU_Libc.h PCUnit.h
PCU_Test.o: PCU_Test.h PCU_Libc.h PCUnit.h
PCU_Suite.o: PCU_Test.h PCU_Suite.h PCU_Libc.h PCUnit.h
PCU_Libc.o: PCU_Libc.h PCUnit.h
