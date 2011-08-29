CC = gcc
CFLAGS = -Wall -g
CFLAGS += -I..
# CFLAGS += -DPCU_NO_LIBC
TARGET = sample
PCUNITDIR = ../PCUnit
LIBPCUNIT = $(PCUNITDIR)/libpcunit.a
LFLAGS = -L$(PCUNITDIR) -lpcunit
OBJS = main.o
OBJS += RingBuf.o test_RingBuf.o
OBJS += StrToken.o test_StrToken.o
OBJS += $(LIBPCUNIT)

.PHONY: all clean pcunit_register test

all: pcunit_register $(TARGET) test

pcunit_register:
	ruby ../util/pcunit_register.rb

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $<

$(LIBPCUNIT):
	cd $(PCUNITDIR) && $(MAKE)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LFLAGS)

test:
	./$(TARGET)

clean:
	rm -f *.o $(TARGET) $(PCUNITDIR)/*.o $(LIBPCUNIT)

main.o:  $(PCUNITDIR)/PCU_Libc.h $(PCUNITDIR)/PCUnit.h
StrToken.o: StrToken.h
test_StrToken.o: StrToken.h $(PCUNITDIR)/PCU_Libc.h $(PCUNITDIR)/PCUnit.h
RingBuf.o: RingBuf.h
test_RingBuf.o: RingBuf.h $(PCUNITDIR)/PCU_Libc.h $(PCUNITDIR)/PCUnit.h

$(LIBPCUNIT): $(PCUNITDIR)/*.h $(PCUNITDIR)/*.c
