.PHONY: all clean

all:
	-cd PCU_Libc_test && $(MAKE)
	-cd PCU_format_test && $(MAKE)
	-cd assert_output_test && $(MAKE)
	-cd PCU_Libc_test && $(MAKE) test && cp *_results.xml ../
	-cd PCU_format_test && $(MAKE) test && cp *_results.xml ../
	-cd assert_output_test && $(MAKE) test && cp *_results.xml ../

clean:
	-cd PCU_Libc_test && $(MAKE) clean
	-cd PCU_format_test && $(MAKE) clean
	-cd assert_output_test && $(MAKE) clean
	-rm *.xml
