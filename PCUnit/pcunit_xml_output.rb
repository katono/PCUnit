#!/usr/bin/ruby

require 'optparse'
require 'rexml/document'

opt = OptionParser.new

output_file_name = "test_results.xml"

opt.on('-o VAL') {|v| output_file_name = v }
opt.on('-h', '--help') {
	print <<-"EOB"
Usage: pcunit_xml_output.rb [-o FILE]
    -o FILE  output file name

	EOB
	exit
}

opt.parse!(ARGV)

$exit_code = 0
doc = REXML::Document.new
doc.add(REXML::XMLDecl.new(version="1.0", encoding="UTF-8"))
$testsuites = REXML::Element.new("testsuites")
doc.add_element($testsuites)
$testsuites.add_attribute("tests", "0")
$testsuites.add_attribute("failures", "0")
$testsuites.add_attribute("errors", "0")
$testsuites.add_attribute("time", "0")
$testsuites.add_attribute("name", "")

$current_msg = ""
$testsuite = nil
$testcase = nil
def parse_line(line)
	line.chomp!
	if line =~ /(\033\[\d*m)/
		line[$1] = ""
	end
	if line =~ /^Suite: (.*)/
		if $testsuite
			$testsuites.attributes["tests"]    = ($testsuites.attributes["tests"].to_i    + $testsuite.attributes["tests"].to_i).to_s
			$testsuites.attributes["failures"] = ($testsuites.attributes["failures"].to_i + $testsuite.attributes["failures"].to_i).to_s
			$testsuites.attributes["errors"]   = ($testsuites.attributes["errors"].to_i   + $testsuite.attributes["errors"].to_i).to_s
		end
		$current_msg = ""
		$testcase = nil
		$testsuite = REXML::Element.new("testsuite") 
		$testsuite.add_attribute("name", $1)
		$testsuite.add_attribute("tests", "0")
		$testsuite.add_attribute("failures", "0")
		$testsuite.add_attribute("skipped", "0")
		$testsuite.add_attribute("errors", "0")
		$testsuite.add_attribute("time", "0")
		$testsuites.add_element($testsuite)
	elsif line =~ /^Test: (.*)/
		$testcase = REXML::Element.new("testcase") 
		$testcase.add_attribute("name", $1)
		$testcase.add_attribute("classname", String($testsuite.attributes["name"]))
		$testcase.add_attribute("time", "0")
		$testsuite.add_element($testcase)
	elsif line =~ /^ \[(.*)\]/
		if $1 == "PASSED"
		elsif $1 == "SKIPPED"
			skipped = REXML::Element.new("skipped") 
			$testcase.add_element(skipped)
		elsif $1 == "FAILED"
			failure = REXML::Element.new("failure") 
			failure.add_attribute("message", $current_msg)
			failure.add_attribute("type", "")
			$testcase.add_element(failure)
		end
		$current_msg = ""
		$testcase = nil
	elsif line =~ /^(\d+) Tests, (\d+) Failures, (\d+) Skipped/
		tests = $1.to_i
		failures = $2.to_i
		skipped = $3.to_i
		$testsuite.attributes["tests"]    = ($testsuite.attributes["tests"].to_i    + tests).to_s
		$testsuite.attributes["failures"] = ($testsuite.attributes["failures"].to_i + failures).to_s
		$testsuite.attributes["skipped"]  = ($testsuite.attributes["skipped"].to_i  + skipped).to_s
		if failures > 0
			$exit_code = 1
		end
	elsif line =~ /^(INITIALIZE|CLEANUP) FAILED : (.*)/
		$testcase = REXML::Element.new("testcase") 
		$testcase.add_attribute("name", $1 + " FAILED")
		$testcase.add_attribute("classname", String($testsuite.attributes["name"]))
		$testcase.add_attribute("time", "0")
		failure = REXML::Element.new("error") 
		failure.add_attribute("message", $2)
		failure.add_attribute("type", "")
		$testcase.add_element(failure)
		$testsuite.add_element($testcase)
		$testsuite.attributes["errors"] = ($testsuite.attributes["errors"].to_i + 1).to_s
		$exit_code = 1
	elsif line == "OK"
	elsif line == ""
		if $testcase
			if $current_msg =~ /(PCU_ASSERT|PCU_FAIL|SETUP FAILED|TEARDOWN FAILED)/
				failure = REXML::Element.new("failure") 
				failure.add_attribute("message", $current_msg)
				failure.add_attribute("type", "")
				$testcase.add_element(failure)
			end
			$current_msg = ""
			$testcase = nil
		end
	else
		$current_msg += line + "#x0A;\n"
	end
end

while s = gets
	puts s
	parse_line s
end
$testsuites.attributes["tests"]    = ($testsuites.attributes["tests"].to_i    + $testsuite.attributes["tests"].to_i).to_s
$testsuites.attributes["failures"] = ($testsuites.attributes["failures"].to_i + $testsuite.attributes["failures"].to_i).to_s
$testsuites.attributes["errors"]   = ($testsuites.attributes["errors"].to_i   + $testsuite.attributes["errors"].to_i).to_s

str = ""
doc.write(str, 2)
str.gsub!(/#x0A;/, '&#x0A;')
File.open(output_file_name, "w") { |f|
	f.write(str)
}

exit $exit_code
