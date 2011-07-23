CFLAGS = -Wall -Wextra -Wno-missing-field-initializers -ansi -pedantic-errors -g
CFLAGS += -std=c99
CFLAGS += -DPCU_printf=printf -DPCU_snprintf=snprintf

TARGET = PCUnit_test

OBJS = main.o
OBJS += PCU_Libc.o
OBJS += PCUnit.o
OBJS += PCU_Suite.o
OBJS += PCU_Test.o
OBJS += test_snprintf.o
OBJS += test_string.o
OBJS += test_strtol.o
OBJS += test_assert.o

all: $(TARGET)


PCU_Libc.o: ../PCUnit/PCU_Libc.c
	$(CC) $(CFLAGS) -DPCU_NO_LIBC -c $<

PCUnit.o: ../PCUnit/PCUnit.c
	$(CC) $(CFLAGS) -c $<

PCU_Suite.o: ../PCUnit/PCU_Suite.c
	$(CC) $(CFLAGS) -c $<

PCU_Test.o: ../PCUnit/PCU_Test.c
	$(CC) $(CFLAGS) -c $<

.SUFFIXES: .c .o

.c .o:
	$(CC) $(CFLAGS) -c $<


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
	./$(TARGET)


.PHONY: clean
clean:
	rm -f *.o $(TARGET)


PCUnit.o: ../PCUnit/PCU_Suite.h ../PCUnit/PCU_Libc.h ../PCUnit/PCUnit.h
PCU_Test.o: ../PCUnit/PCU_Test.h ../PCUnit/PCU_Libc.h ../PCUnit/PCUnit.h
PCU_Suite.o: ../PCUnit/PCU_Test.h ../PCUnit/PCU_Suite.h ../PCUnit/PCU_Libc.h ../PCUnit/PCUnit.h
PCU_Libc.o: ../PCUnit/PCU_Libc.h ../PCUnit/PCUnit.h
main.o: ../PCUnit/PCU_Libc.h ../PCUnit/PCUnit.h
test_snprintf.o: ../PCUnit/PCU_Libc.h ../PCUnit/PCUnit.h
test_string.o: ../PCUnit/PCU_Libc.h ../PCUnit/PCUnit.h
test_strtol.o: ../PCUnit/PCU_Libc.h ../PCUnit/PCUnit.h
test_assert.o: ../PCUnit/PCU_Libc.h ../PCUnit/PCUnit.h

