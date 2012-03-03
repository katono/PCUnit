CFLAGS = -Wall -g

TARGET = assert_test
TARGET_NO_SETJMP = assert_test_nosetjmp

LIBPCUNIT = ../../PCUnit/libpcunit.a
LIBPCUNIT_NO_SETJMP = dummy.a
OBJS = main.o
OBJS += $(LIBPCUNIT)
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

all: pcunit_register $(LIBPCUNIT) $(TARGET)

no_setjmp: pcunit_register $(LIBPCUNIT_NO_SETJMP) $(TARGET_NO_SETJMP)

pcunit_register:
	ruby ../../util/pcunit_register.rb


.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -E $< > $<.i
	$(CC) $(CFLAGS) -c $<

$(LIBPCUNIT):
	cd ../../PCUnit && $(MAKE) clean
	cd ../../PCUnit && $(MAKE) CFLAGS="-Wall -W -Wno-long-long -ansi -pedantic-errors -DPCU_USE_WCHAR -g"

$(LIBPCUNIT_NO_SETJMP):
	cd ../../PCUnit && $(MAKE) clean
	cd ../../PCUnit && $(MAKE) CFLAGS="-Wall -W -Wno-long-long -ansi -pedantic-errors -DPCU_USE_WCHAR -DPCU_NO_SETJMP -g"

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
#     ./$(TARGET)
	cd ../../PCUnit && $(MAKE) clean

$(TARGET_NO_SETJMP): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
#     ./$(TARGET)
	cd ../../PCUnit && $(MAKE) clean

test:
	./$(TARGET) 1


.PHONY: clean
clean:
	rm -f *.o *.i $(TARGET) $(TARGET_NO_SETJMP)


main.o: ../../PCUnit/PCUnit.h
AssertTest.o: ../../PCUnit/PCUnit.h
AssertEqualTest.o:  ../../PCUnit/PCUnit.h
AssertPtrTest.o:  ../../PCUnit/PCUnit.h
AssertStringTest.o:  ../../PCUnit/PCUnit.h
AssertStringWTest.o:  ../../PCUnit/PCUnit.h
AssertDoubleTest.o:  ../../PCUnit/PCUnit.h
AssertOperatorTest.o:  ../../PCUnit/PCUnit.h
FailTest.o:  ../../PCUnit/PCUnit.h
SetupTest.o:  ../../PCUnit/PCUnit.h
InitializeTest.o:  ../../PCUnit/PCUnit.h
CleanupTest.o:  ../../PCUnit/PCUnit.h
