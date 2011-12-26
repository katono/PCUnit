CFLAGS = -Wall -g

TARGET = assert_test

OBJS = main.o
OBJS += ../../PCUnit/libpcunit.a
OBJS += AssertTestLongjmp.o
OBJS += AssertTestReturn.o
OBJS += AssertEqualTestLongjmp.o
OBJS += AssertEqualTestReturn.o
OBJS += AssertPtrTestLongjmp.o
OBJS += AssertPtrTestReturn.o
OBJS += AssertStringTestLongjmp.o
OBJS += AssertStringTestReturn.o
OBJS += AssertStringWTestLongjmp.o
OBJS += AssertStringWTestReturn.o
OBJS += AssertDoubleTestLongjmp.o
OBJS += AssertDoubleTestReturn.o
OBJS += AssertOperatorTestLongjmp.o
OBJS += AssertOperatorTestReturn.o
OBJS += FailTestLongjmp.o
OBJS += FailTestReturn.o
OBJS += SetupTestLongjmp.o
OBJS += SetupTestReturn.o
OBJS += InitializeTestLongjmp.o
OBJS += InitializeTestReturn.o
OBJS += CleanupTestLongjmp.o
OBJS += CleanupTestReturn.o

all: pcunit_register $(TARGET)

pcunit_register:
	ruby ../../util/pcunit_register.rb


.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -E $< > $<.i
	$(CC) $(CFLAGS) -c $<


$(TARGET): $(OBJS)
	cd ../../PCUnit && $(MAKE)
	$(CC) $(CFLAGS) $(OBJS) -o $@
	./$(TARGET)

test:
	./$(TARGET) 1


.PHONY: clean
clean:
	rm -f *.o *.i $(TARGET)


main.o: ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertTestLongjmp.o: AssertTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertTestReturn.o:  AssertTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertEqualTestLongjmp.o:  AssertEqualTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertEqualTestReturn.o:   AssertEqualTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertPtrTestLongjmp.o:  AssertPtrTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertPtrTestReturn.o:   AssertPtrTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertStringTestLongjmp.o:  AssertStringTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertStringTestReturn.o:   AssertStringTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertStringWTestLongjmp.o:  AssertStringWTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertStringWTestReturn.o:   AssertStringWTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertDoubleTestLongjmp.o:  AssertDoubleTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertDoubleTestReturn.o:   AssertDoubleTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertOperatorTestLongjmp.o:  AssertOperatorTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
AssertOperatorTestReturn.o:   AssertOperatorTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
FailTestLongjmp.o:  FailTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
FailTestReturn.o:   FailTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
SetupTestLongjmp.o:  SetupTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
SetupTestReturn.o:   SetupTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
InitializeTestLongjmp.o:  InitializeTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
InitializeTestReturn.o:   InitializeTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
CleanupTestLongjmp.o:  CleanupTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
CleanupTestReturn.o:   CleanupTest.c ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
