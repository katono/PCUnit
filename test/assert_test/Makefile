CFLAGS = -Wall -DPCU_USE_WCHAR -g

TARGET = assert_test

OBJS = main.o
OBJS += ../../PCUnit/libpcunit.a
OBJS += AssertTest.o
OBJS += AssertEqualTest.o
OBJS += AssertPtrTest.o
OBJS += AssertStringTest.o
OBJS += AssertStringWTest.o
OBJS += AssertDoubleTest.o
OBJS += AssertOperatorTest.o
OBJS += FailTest.o
OBJS += SetupTest.o
OBJS += InitializeTest.o
OBJS += CleanupTest.o

all: pcunit_register $(TARGET)

pcunit_register:
	ruby ../../util/pcunit_register.rb
	cd ../../PCUnit && $(MAKE) clean
	cd ../../PCUnit && $(MAKE) CFLAGS="-Wall -W -Wno-long-long -ansi -pedantic-errors -DPCU_USE_WCHAR -g"


.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -E $< > $<.i
	$(CC) $(CFLAGS) -c $<


$(TARGET): $(OBJS)
	cd ../../PCUnit && $(MAKE)
	$(CC) $(CFLAGS) $(OBJS) -o $@
	./$(TARGET)
	cd ../../PCUnit && $(MAKE) clean

test:
	./$(TARGET) 1


.PHONY: clean
clean:
	rm -f *.o *.i $(TARGET)


main.o: ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertTest.o: ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertEqualTest.o:  ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertPtrTest.o:  ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertStringTest.o:  ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertStringWTest.o:  ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertDoubleTest.o:  ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertOperatorTest.o:  ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
FailTest.o:  ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
SetupTest.o:  ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
InitializeTest.o:  ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
CleanupTest.o:  ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
