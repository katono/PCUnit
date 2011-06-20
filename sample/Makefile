CFLAGS = -Wall -ansi -pedantic-errors -g
CFLAGS += -I../PCUnit
# CFLAGS += -DPCU_NO_STDLIB
LFLAGS = -L../PCUnit -lpcunit
TARGET = sample
OBJS = main.o
OBJS += RingBuf.o test_RingBuf.o
OBJS += StrToken.o test_StrToken.o

all: $(TARGET)

.SUFFIXES: .c .o

.c .o:
	$(CC) $(CFLAGS) -c $<


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LFLAGS)


.PHONY: clean
clean:
	rm -f *.o $(TARGET)

main.o: test.h
StrToken.o: StrToken.h
test_StrToken.o: StrToken.h test.h
RingBuf.o: RingBuf.h
test_RingBuf.o: RingBuf.h test.h

