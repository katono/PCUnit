CC = gcc
CFLAGS = -Wall -g
CFLAGS += -I../..
PCUNIT_DIR = ../../PCUnit
LFLAGS = -L$(PCUNIT_DIR) -lpcunit

TARGET = alltests
OBJS = main.o
OBJS += MockTest.o
OBJS += mock_target.o


.PHONY: all clean pcunit_register test xml

all: pcunit_register $(TARGET)

pcunit_register:
	-ruby $(PCUNIT_DIR)/pcunit_register.rb -e mock_target

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $<

$(TARGET): $(OBJS)
	cd $(PCUNIT_DIR) && $(MAKE)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LFLAGS)

test: all
	./$(TARGET)

xml: all
	./$(TARGET) verbose | ruby $(PCUNIT_DIR)/pcunit_xml_output.rb $(OUTPUT)

clean:
	cd $(PCUNIT_DIR) && $(MAKE) clean
	rm -f *.o $(TARGET) target.h mock_target.*

mock_target.o: $(PCUNIT_DIR)/pcunit_mock.rb create_header.rb
	-ruby ./create_header.rb
	-ruby $(PCUNIT_DIR)/pcunit_mock.rb target.h
	$(CC) $(CFLAGS) -c $*.c

MockTest.o: mock_target.o
