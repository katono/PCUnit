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
Usage: pcunit_template.rb suite_name1 [suite_name2 ...] [-d DIR] [-p [EXT]] [-m [FILE]] [-o]
    -d DIR     output directory
    -p [EXT]   C++ file extension (default: cpp)
    -m [FILE]  main file name (default: main.c or main.cpp)
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
	PCU_FAIL0("TODO");
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


int main(void)
{
	const PCU_SuiteMethod suites[] = {
	};
	PCU_set_putchar(putchar);
	PCU_enable_color();
	PCU_run(suites, sizeof suites / sizeof suites[0]);
	return 0;
}

	EOB
end

ARGV.each {|name|
	if OPTS[:o] || !File.exist?(OPTS[:d] + name + OPTS[:p])
		create_test(name)
	end
}

if OPTS[:m]
	if OPTS[:o] || !File.exist?(OPTS[:d] + OPTS[:m])
		create_alltests()
	end
end

