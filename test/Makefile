CFLAGS = -Wall -ansi -pedantic-errors -g
CFLAGS += -DPCU_NO_STDLIB
ifeq ($(STD),C99)
	CFLAGS += -std=c99
	C99 = "CFLAGS=-Wall -ansi -pedantic-errors -g -Iinclude -std=c99"
endif

TARGET = PCUnit_test

OBJS = main.o
OBJS += ../PCUnit/PCU_Libc.o
OBJS += test_sprintf.o
OBJS += test_string.o
OBJS += test_strtol.o
OBJS += test_assert.o
OBJS += ../PCUnit/libpcunit.a

all: $(TARGET)

.SUFFIXES: .c .o

.c .o:
	$(CC) $(CFLAGS) -c $<


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

../PCUnit/libpcunit.a:
	cd ../PCunit && $(MAKE) $(C99)



.PHONY: clean
clean:
	rm -f *.o $(TARGET) ../PCUnit/*.o ../PCUnit/*.a


../PCUnit/PCU_Libc.o: ../PCUnit/include/PCU_Libc.h ../PCUnit/include/PCUnit.h

