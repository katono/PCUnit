#!/usr/bin/ruby

require 'optparse'

opt = OptionParser.new
OPTS = {}

OPTS[:d] = "./"
OPTS[:p] = ".c"

opt.on('-m [VAL]') {|v|
	if v
		OPTS[:m] = v
	else
		OPTS[:m] = "main"
	end
}
opt.on('-M [VAL]') {|v|
	if v
		OPTS[:M] = v
	else
		OPTS[:M] = "Makefile"
	end
}
opt.on('-i') {|v| OPTS[:i] = v }
opt.on('-d VAL') {|v| OPTS[:d] = v + "/" }
opt.on('-o') {|v| OPTS[:o] = v }
opt.on('-p [VAL]') {|v|
	if v =~ /(cpp|cc|cxx)/i
		OPTS[:p] = "." + $1
	else
		OPTS[:p] = ".cpp"
	end
}
opt.on('-h', '--help') {
	print <<-"EOB"
Usage: pcunit_template.rb [suite_name ...] [-d DIR] [-p [EXT]] [-m [FILE]] [-M [FILE] [-i]] [-o]
    -d DIR     output directory
    -p [EXT]   change file extension from C to C++ (default: cpp)
    -m [FILE]  create main file (default: main.c or main.cpp)
    -M [FILE]  create Makefile (default: Makefile)
       [-i]    whether libpcunit.a is installed
    -o         overwrite flag

	EOB
	exit
}


opt.parse!(ARGV)

if OPTS[:m] == "main"
	OPTS[:m] += OPTS[:p]
end

def create_test(suite_name)
	file = File.open(OPTS[:d] + suite_name + OPTS[:p], "w")
	file.print <<-"EOB"
#include "PCUnit/PCUnit.h"

static int initialize(void)
{
	return 0;
}

static int cleanup(void)
{
	return 0;
}

static int setup(void)
{
	return 0;
}

static int teardown(void)
{
	return 0;
}


static void test_TODO(void)
{
	PCU_FAIL("TODO");
}


PCU_Suite *#{suite_name}_suite(void)
{
	static PCU_Test tests[] = {
	};
	static PCU_Suite suite = {
		"#{suite_name}", tests, sizeof tests / sizeof tests[0], setup, teardown, initialize, cleanup
	};
	return &suite;
}

	EOB
end

def create_alltests()
	file = File.open(OPTS[:d] + OPTS[:m], "w")
	file.print <<-"EOB"
#include "PCUnit/PCUnit.h"
#include <stdio.h>


int main(int argc, char *argv[])
{
	const PCU_SuiteMethod suites[] = {
	};
	PCU_set_putchar(putchar);
	PCU_set_getchar(getchar);
	PCU_enable_color();
	if (argc >= 2) {
		PCU_set_verbose(1);
	}
	return PCU_run(suites, sizeof suites / sizeof suites[0]);
}

	EOB
end

def create_makefile(objs)
	cc = OPTS[:p] == ".c" ? "gcc" : "g++"
	ext = OPTS[:p]
	file = File.open(OPTS[:d] + OPTS[:M], "w")
	file.print <<-"EOB"
CC = #{cc}
CFLAGS = -Wall -g
CFLAGS += -I.
LFLAGS = -LPCUnit -lpcunit

TARGET = alltests
OBJS = main.o
#{objs}

.PHONY: all clean pcunit_register test

all: pcunit_register $(TARGET)

pcunit_register:
	ruby PCUnit/pcunit_register.rb

.SUFFIXES: #{ext} .o

#{ext}.o:
	$(CC) $(CFLAGS) -c $<

$(TARGET): $(OBJS)
	cd PCUnit && $(MAKE)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LFLAGS)

test: all
	./$(TARGET)

xml: all
	./$(TARGET) verbose | ruby PCUnit/pcunit_xml_output.rb $(OUTPUT)

clean:
	cd PCUnit && $(MAKE) clean
	rm -f *.o $(TARGET)

	EOB
end

def create_makefile_installed(objs)
	cc = OPTS[:p] == ".c" ? "gcc" : "g++"
	ext = OPTS[:p]
	file = File.open(OPTS[:d] + OPTS[:M], "w")
	file.print <<-"EOB"
INSTALLDIR = /usr/local

CC = #{cc}
CFLAGS = -Wall -g
CFLAGS += -I$(INSTALLDIR)/include
LFLAGS = -L$(INSTALLDIR)/lib -lpcunit

TARGET = alltests
OBJS = main.o
#{objs}

.PHONY: all clean pcunit_register test

all: pcunit_register $(TARGET)

pcunit_register:
	ruby $(INSTALLDIR)/bin/pcunit_register.rb

.SUFFIXES: #{ext} .o

#{ext}.o:
	$(CC) $(CFLAGS) -c $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LFLAGS)

test: all
	./$(TARGET)

xml: all
	./$(TARGET) verbose | ruby $(INSTALLDIR)/bin/pcunit_xml_output.rb $(OUTPUT)

clean:
	rm -f *.o $(TARGET)

	EOB
end

makefile_objs = ""
ARGV.each {|name|
	if OPTS[:o] || !File.exist?(OPTS[:d] + name + OPTS[:p])
		create_test(name)
	end
	makefile_objs += "OBJS += #{name}.o\n"
}

if OPTS[:m]
	if OPTS[:o] || !File.exist?(OPTS[:d] + OPTS[:m])
		create_alltests()
	end
end

if OPTS[:M]
	if OPTS[:o] || !File.exist?(OPTS[:d] + OPTS[:M])
		if OPTS[:i]
			create_makefile_installed(makefile_objs)
		else
			create_makefile(makefile_objs)
		end
	end
end

