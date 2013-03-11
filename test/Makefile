.PHONY: all test xml build clean

all: test

test: build
	-cd PCU_Libc_test && $(MAKE) test
	-cd PCU_format_test && $(MAKE) test
	-cd assert_output_test && $(MAKE) test

xml: build
	-cd PCU_Libc_test && $(MAKE) xml && cp *_results.xml ../
	-cd PCU_format_test && $(MAKE) xml && cp *_results.xml ../
	-cd assert_output_test && $(MAKE) xml && cp *_results.xml ../

build:
	-cd PCU_Libc_test && $(MAKE)
	-cd PCU_format_test && $(MAKE)
	-cd assert_output_test && $(MAKE)

clean:
	-cd PCU_Libc_test && $(MAKE) clean
	-cd PCU_format_test && $(MAKE) clean
	-cd assert_output_test && $(MAKE) clean
	-rm *.xml
