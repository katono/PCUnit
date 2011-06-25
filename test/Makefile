CFLAGS = -Wall -ansi -pedantic-errors -g
CFLAGS += -DPCU_NO_LIBC
CFLAGS += -std=c99

TARGET = PCUnit_test

OBJS = main.o
OBJS += ../PCUnit/PCU_Libc.o
OBJS += test_snprintf.o
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
	./$(TARGET)


LIBPCUNIT_CFLAGS = "CFLAGS=-Wall -ansi -pedantic-errors -g -std=c99 -DPCU_printf=printf"

../PCUnit/libpcunit.a:
	cd ../PCunit && $(MAKE) $(LIBPCUNIT_CFLAGS)



.PHONY: clean
clean:
	rm -f *.o $(TARGET) ../PCUnit/*.o ../PCUnit/*.a


../PCUnit/PCU_Libc.o: ../PCUnit/PCU_Libc.h ../PCUnit/PCUnit.h

