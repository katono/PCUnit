CFLAGS = -Wall -g


TARGET = assert_output_test

OBJS = main.o
OBJS += ../../PCUnit/libpcunit.a
OBJS += AssertOutputTest.o


.PHONY: all clean pcunit_register test

all: pcunit_register $(TARGET)

pcunit_register:
	ruby ../../util/pcunit_register.rb
	cd ../assert_test && $(MAKE)
	../assert_test/assert_test nocolor > assert_output_actual.txt

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $<

../../PCUnit/libpcunit.a:
	cd ../../PCUnit && $(MAKE)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
	./$(TARGET)

test:
	./$(TARGET)

clean:
	rm -f *.o $(TARGET) assert_output_actual.txt

