CC = gcc
CFLAGS = -Wall -g
# CFLAGS += -DPCU_NO_LIBC
TARGET = sample
PCUNITDIR = ../PCUnit
CFLAGS += -I$(PCUNITDIR)/../
LIBPCUNIT = $(PCUNITDIR)/libpcunit.a
LFLAGS = -L$(PCUNITDIR) -lpcunit
OBJS = main.o
OBJS += RingBuf.o test_RingBuf.o
OBJS += StrToken.o test_StrToken.o
OBJS += $(LIBPCUNIT)

.PHONY: all clean pcunit_register test xml

all: pcunit_register $(TARGET)

pcunit_register:
	-ruby $(PCUNITDIR)/pcunit_register.rb

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $<

$(LIBPCUNIT):
	cd $(PCUNITDIR) && $(MAKE)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LFLAGS)

test: all
	./$(TARGET)

xml: all
	./$(TARGET) -v | ruby $(PCUNITDIR)/pcunit_xml_output.rb $(OUTPUT)

clean:
	rm -f *.o $(TARGET) $(PCUNITDIR)/*.o $(LIBPCUNIT)

main.o:  $(PCUNITDIR)/PCUnit.h
StrToken.o: StrToken.h
test_StrToken.o: StrToken.h $(PCUNITDIR)/PCUnit.h
RingBuf.o: RingBuf.h
test_RingBuf.o: RingBuf.h $(PCUNITDIR)/PCUnit.h

$(LIBPCUNIT): $(PCUNITDIR)/*.h $(PCUNITDIR)/*.c
