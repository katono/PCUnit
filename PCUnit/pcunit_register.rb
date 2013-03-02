#!/usr/bin/ruby

if RUBY_VERSION >= '1.9'
	Encoding.default_external = "ASCII-8BIT"
end

require 'optparse'

opt = OptionParser.new
OPTS = {}

OPTS[:d] = "."

opt.on('-d VAL') {|v| OPTS[:d] = v }
opt.on('-n') {|v| OPTS[:n] = v }
opt.on('-h', '--help') {
	print <<-"EOB"
Usage: pcunit_register.rb [-d DIR] [-n]
    -d DIR  source code directory
    -n      no backup flag

	EOB
	exit
}

opt.parse!(ARGV)

$suite_methods = []
$main_file = nil

def register_tests(file_name)
	file = File.open(file_name, "r")
	lines = []
	testfuncs = []
	registered_testfuncs = []
	idx = 0
	mark = false
	while line = file.gets
		lines.push(line)
		if line =~ /void\s+(test[^\s]*)\s*\(\s*(void|)\s*\)/
			testfuncs.push($1)
		end
		if mark && line =~ /(,|\()\s*(test[^\s"\{\}\(\),]*)/
			registered_testfuncs.push($2)
		end
		if line =~ /PCU_Test\s+.*\[\]/
			mark = true
			tmp = line.slice(/^\s*/)
			indent = tmp == "" ? "\t" : tmp * 2
		end
		if mark && line =~ /\};/
			insert_idx = idx
			mark = false
		end
		if line =~ /PCU_Suite\s*\*\s*([^\s]+)\s*\(\s*(void|)\s*\)\s*[^;]*\s*$/
			$suite_methods.push($1)
		end
		if !$main_file && line =~ /PCU_SuiteMethod\s+.*\[\s*\]/
			$main_file = file_name
		end
		idx += 1
	end
	file.close
	diff = testfuncs - registered_testfuncs
	if diff.empty? || !insert_idx
		return
	end
	if lines[0] =~ /\r\n$/
		nl = "\r\n"
	else
		nl = "\n"
	end
	diff.each {|func|
		lines.insert(insert_idx, "#{indent}PCU_TEST(#{func}),#{nl}")
		insert_idx += 1
	}
	if !OPTS[:n]
		File.rename(file_name, file_name + ".bak")
	end
	file = File.open(file_name, "w")
	lines.each {|line|
		file << line
	}
	file.close
end

def register_suite_methods(file_name)
	$suite_methods.uniq!
	file = File.open(file_name, "r")
	lines = []
	registered_suite_methods1 = []
	registered_suite_methods2 = []
	idx = 0
	mark = false
	insert_idx_decl = nil
	while line = file.gets
		if line =~ /PCU_Suite\s*\*\s*([^\s]+)\s*\(\s*(void|)\s*\)\s*;\s*$/
			registered_suite_methods1.push($1)
			next
		end
		if mark && line =~ /([^\s,]+)\s*,/
			registered_suite_methods2.push($1)
			next
		end
		lines.push(line)
		if line =~ /PCU_SuiteMethod\s+.*\[\s*\]/
			mark = true
			indent = line.slice(/^\s*/) * 2
		end
		if mark && line =~ /\}\s*;/
			insert_idx_init = idx
			mark = false
		end
		if !insert_idx_decl && line =~ /^\s*\{/
			insert_idx_decl = idx - 2
		elsif !insert_idx_decl && line =~ /\)\s*\{/
			insert_idx_decl = idx - 1
		end
		idx += 1
	end
	file.close
	diff1 = $suite_methods - registered_suite_methods1
	diff2 = $suite_methods - registered_suite_methods2
	if (diff1.empty? && diff2.empty?) || !insert_idx_init || !insert_idx_decl
		return
	end
	if lines[0] =~ /\r\n$/
		nl = "\r\n"
	else
		nl = "\n"
	end
	$suite_methods.each {|method|
		lines.insert(insert_idx_decl, "PCU_Suite *#{method}(void);#{nl}")
		insert_idx_decl += 1
	}
	insert_idx_init += $suite_methods.size
	$suite_methods.each {|method|
		lines.insert(insert_idx_init, "#{indent}#{method},#{nl}")
		insert_idx_init += 1
	}
	if !OPTS[:n]
		File.rename(file_name, file_name + ".bak")
	end
	file = File.open(file_name, "w")
	lines.each {|line|
		file << line
	}
	file.close
end

Dir.chdir(OPTS[:d])
Dir.glob("**/*.{c{,pp,c,xx},C{,PP,C,XX}}") {|fname|
	register_tests(fname)
}
if $main_file
	register_suite_methods($main_file)
end

