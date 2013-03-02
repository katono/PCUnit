CC = gcc
AR = ar
CFLAGS = -Wall -W -Os
# CFLAGS += -DPCU_NO_VSPRINTF
# CFLAGS += -DPCU_NO_SETJMP
# CFLAGS += -DPCU_NO_LIBC
# CFLAGS += -DPCU_NO_STDARG
# CFLAGS += -DPCU_NO_FLOATINGPOINT
# CFLAGS += -DPCU_NO_DIV32
# CFLAGS += -DPCU_NO_CONSOLE_RUN
CFLAGS += -DPCU_USE_WCHAR
# CFLAGS += -DPCU_FORMAT_BUFSIZE=256

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
	install -m 644 PCUnit.h $(INSTALLDIR)/include/PCUnit
	for i in `ls pcunit_*.rb`; do\
		install -m 755 $$i $(INSTALLDIR)/bin;\
	done

uninstall:
	rm -f $(INSTALLDIR)/lib/$(TARGET)
	rm -rf $(INSTALLDIR)/include/PCUnit
	rm -f $(INSTALLDIR)/bin/pcunit_*.rb

clean:
	rm -f *.o *.a

PCUnit.o: PCU_Test.h PCU_Suite.h PCU_Libc.h PCUnit.h Makefile
PCU_Test.o: PCU_Test.h PCU_Suite.h PCU_Libc.h PCUnit.h Makefile
PCU_Suite.o: PCU_Test.h PCU_Suite.h PCU_Libc.h PCUnit.h Makefile
PCU_Libc.o: PCU_Libc.h PCUnit.h Makefile
