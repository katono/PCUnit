CFLAGS = -Wall -std=c99 -g

TARGET = assert_test
TARGET_NO_SETJMP = assert_test_nosetjmp
TARGET_NOT_SUPPORTED = assert_test_not_supported

LIBPCUNIT = ../../PCUnit/libpcunit.a
LIBPCUNIT_NO_SETJMP = dummy.a
LIBPCUNIT_NOT_SUPPORTED = dummy2.a
OBJS = main.o
OBJS += $(LIBPCUNIT)
OBJS += AssertTest.o
OBJS += AssertEqualTest.o
OBJS += AssertPtrTest.o
OBJS += AssertStringTest.o
OBJS += AssertStringWTest.o
OBJS += AssertMemoryTest.o
OBJS += AssertDoubleTest.o
OBJS += AssertOperatorTest.o
OBJS += FailTest.o
OBJS += SetupTest.o
OBJS += InitializeTest.o
OBJS += CleanupTest.o

.PHONY: all clean pcunit_register test xml no_setjmp not_supported console

all: pcunit_register $(LIBPCUNIT) $(TARGET)

no_setjmp: pcunit_register $(LIBPCUNIT_NO_SETJMP) $(TARGET_NO_SETJMP)
	$(MAKE) all
	- ./$(TARGET) > $(TARGET).txt
	- ./$(TARGET_NO_SETJMP) > $(TARGET_NO_SETJMP).txt
	diff $(TARGET).txt $(TARGET_NO_SETJMP).txt

not_supported: pcunit_register $(LIBPCUNIT_NOT_SUPPORTED) $(TARGET_NOT_SUPPORTED)

pcunit_register:
	-ruby ../../PCUnit/pcunit_register.rb
	cd ../../PCUnit && $(MAKE) clean


.SUFFIXES: .c .o

.c.o:
	#  $(CC) $(CFLAGS) -E $< > $<.i
	$(CC) $(CFLAGS) -c $<

$(LIBPCUNIT):
	cd ../../PCUnit && $(MAKE) CFLAGS="-Wall -W -std=c99 -DPCU_USE_WCHAR -g"

$(LIBPCUNIT_NO_SETJMP):
	cd ../../PCUnit && $(MAKE) CFLAGS="-Wall -W -std=c99 -DPCU_USE_WCHAR -DPCU_NO_SETJMP -g"

$(LIBPCUNIT_NOT_SUPPORTED):
	cd ../../PCUnit && $(MAKE) CFLAGS="-Wall -W -std=c99 -DPCU_NO_FLOATINGPOINT -g"

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
	cd ../../PCUnit && $(MAKE) clean

$(TARGET_NO_SETJMP): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
	cd ../../PCUnit && $(MAKE) clean

$(TARGET_NOT_SUPPORTED): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
	cd ../../PCUnit && $(MAKE) clean

test:
	./$(TARGET) nocolor verbose

xml:
	./$(TARGET) nocolor verbose | ruby ../../PCUnit/pcunit_xml_output.rb

console:
	./$(TARGET) console

clean:
	rm -f *.o *.i $(TARGET) $(TARGET_NO_SETJMP) $(TARGET_NOT_SUPPORTED) $(TARGET).txt $(TARGET_NO_SETJMP).txt *.xml


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
