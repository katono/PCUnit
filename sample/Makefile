CFLAGS = -Wall -Wextra -Wno-missing-field-initializers -std=c99 -pedantic-errors -g
CFLAGS += -I../PCUnit
# CFLAGS += -DPCU_NO_LIBC
LFLAGS = -L../PCUnit -lpcunit
TARGET = sample
OBJS = main.o
OBJS += RingBuf.o test_RingBuf.o
OBJS += StrToken.o test_StrToken.o
OBJS += ../PCUnit/libpcunit.a

all: $(TARGET)

.SUFFIXES: .c .o

.c .o:
	$(CC) $(CFLAGS) -c $<


../PCUnit/libpcunit.a:
	cd ../PCUnit && $(MAKE)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LFLAGS)
	./$(TARGET)


.PHONY: clean
clean:
	rm -f *.o $(TARGET) ../PCUnit/*.o ../PCUnit/*.a

main.o: 
StrToken.o: StrToken.h
test_StrToken.o: StrToken.h
RingBuf.o: RingBuf.h
test_RingBuf.o: RingBuf.h

