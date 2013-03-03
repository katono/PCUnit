#!/usr/bin/ruby

require 'optparse'
require 'rexml/document'

opt = OptionParser.new

output_file_name = "test_results.xml"
no_stdout = false
exit_code_flag = false

opt.on('-e') {|v| exit_code_flag = v }
opt.on('-n') {|v| no_stdout = v }
opt.on('-h', '--help') {
	print <<-"EOB"
Usage: pcunit_xml_output.rb [output_file] [-e] [-n]
    -e  return non-zero exit code if tests failed
    -n  no stdout flag

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

$failure_message = ""
$testsuite = nil
$testcase = nil
$exists_testcase = false

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
		$failure_message = ""
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
		$exists_testcase = true
	elsif line =~ /^ \[(.*)\]/
		if $1 == "PASSED"
		elsif $1 == "SKIPPED"
			skipped = REXML::Element.new("skipped") 
			$testcase.add_element(skipped)
		elsif $1 == "FAILED"
			failure = REXML::Element.new("failure") 
			failure.add_attribute("message", $failure_message)
			failure.add_attribute("type", "")
			$testcase.add_element(failure)
		end
		$failure_message = ""
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
			if $failure_message =~ /(PCU_ASSERT|PCU_FAIL|SETUP FAILED|TEARDOWN FAILED)/
				failure = REXML::Element.new("failure") 
				failure.add_attribute("message", $failure_message)
				failure.add_attribute("type", "")
				$testcase.add_element(failure)
			end
			$failure_message = ""
			$testcase = nil
		end
	else
		$failure_message += line + "#x0A;\n"
	end
end

while s = STDIN.gets
	if !no_stdout
		puts s
	end
	if RUBY_VERSION >= '1.9'
		s = s.encode("UTF-8")
	end
	parse_line s
end
$testsuites.attributes["tests"]    = ($testsuites.attributes["tests"].to_i    + $testsuite.attributes["tests"].to_i).to_s
$testsuites.attributes["failures"] = ($testsuites.attributes["failures"].to_i + $testsuite.attributes["failures"].to_i).to_s
$testsuites.attributes["errors"]   = ($testsuites.attributes["errors"].to_i   + $testsuite.attributes["errors"].to_i).to_s
if !$exists_testcase
	# dummy
	$testsuite = REXML::Element.new("testsuite") 
	$testsuite.add_attribute("name", "")
	$testsuite.add_attribute("tests", "0")
	$testsuite.add_attribute("failures", "0")
	$testsuite.add_attribute("skipped", "0")
	$testsuite.add_attribute("errors", "0")
	$testsuite.add_attribute("time", "0")
	$testsuites.add_element($testsuite)
	$testcase = REXML::Element.new("testcase") 
	$testcase.add_attribute("name", "")
	$testcase.add_attribute("classname", "")
	$testcase.add_attribute("time", "0")
	$testsuite.add_element($testcase)
end

str = ""
doc.write(str, 2)
str.gsub!(/#x0A;/, '&#x0A;')
str.gsub!(/message='/, "message='\n")
str += "\n"
if ARGV[0]
	output_file_name = ARGV[0]
end
File.open(output_file_name, "w") { |f|
	f.write(str)
}

if exit_code_flag
	exit $exit_code
end
