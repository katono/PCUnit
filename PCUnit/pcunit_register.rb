#!/usr/bin/ruby

if RUBY_VERSION >= '1.9'
	Encoding.default_external = "ASCII-8BIT"
end

require 'optparse'

opt = OptionParser.new
OPTS = {}

OPTS[:d] = "."
excluded = Array.new

opt.on('-d VAL') {|v| OPTS[:d] = v }
opt.on('-e VAL') {|v| excluded.push v }
opt.on('-n') {|v| OPTS[:n] = v }
opt.on('-h', '--help') {
	print <<-"EOB"
Usage: pcunit_register.rb [-d DIR] [-e PATTERN] [-n]
    -d DIR      source code directory
    -e PATTERN  excluded file pattern
    -n          no backup flag

	EOB
	exit
}

opt.parse!(ARGV)

$suite_methods = []
$main_file = nil

def register_tests(file_name)
	file = File.open(file_name, "r")
	lines = []
	pcu_test_lines = []
	testfuncs = []
	registered_testfuncs = []
	idx = 0
	pcu_test_area = false
	prep_comment_flags = Array.new
	def is_prep_comment(flags)
		flags.each {|e|
			if e
				return true
			end
		}
		return false
	end
	normal_comment_flag = false
	while line = file.gets
		if pcu_test_area && !(line =~ /\};/)
			pcu_test_lines.push(String.new(line))
		else
			lines.push(String.new(line))
		end

		line.sub!(/\/\/.*$/, "")
		line.gsub!(/\/\*.*?\*\//, "")
		if line =~ /#\s*if\b/
			if line =~ /#\s*if\s+0/
				prep_comment_flags.push(true)
			else
				prep_comment_flags.push(false)
			end
		elsif line =~ /#\s*else/
			if prep_comment_flags.empty?
			elsif prep_comment_flags.last
				prep_comment_flags.pop
				prep_comment_flags.push(false)
			else
				prep_comment_flags.pop
				prep_comment_flags.push(true)
			end
		elsif line =~ /#\s*endif/
			if prep_comment_flags.empty?
			else
				prep_comment_flags.pop
			end
		end
		if is_prep_comment(prep_comment_flags)
			line = ""
		else
			if line["/*"]
				line[/\/\*.*/] = ""
				normal_comment_flag = true
			elsif normal_comment_flag
				if line["*/"]
					line[/.*\*\//] = ""
					normal_comment_flag = false
				else
					line = ""
				end
			end
		end

		if line =~ /void\s+(test[^\s]*)\s*\(\s*(void|)\s*\)/
			testfuncs.push($1)
		end
		if pcu_test_area && line =~ /(,|\()\s*(test[^\s"\{\}\(\),]*)/
			registered_testfuncs.push($2)
		end
		if line =~ /PCU_Test\s+.*\[\]/
			pcu_test_area = true
			tmp = line.slice(/^\s*/)
			indent = tmp == "" ? "\t" : tmp * 2
			pcu_test_area_idx = idx
		end
		if pcu_test_area && line =~ /\};/
			pcu_test_area = false
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
	undefined_testfuncs = []
	registered_testfuncs.each {|func|
		if !testfuncs.include?(func)
			undefined_testfuncs.push(func)
		end
	}
	added_testfuncs = testfuncs - registered_testfuncs
	if (undefined_testfuncs.empty? && added_testfuncs.empty?) || !pcu_test_area_idx
		return
	end
	if lines[0] =~ /\r\n$/
		nl = "\r\n"
	else
		nl = "\n"
	end
	if !OPTS[:n]
		File.rename(file_name, file_name + ".bak")
	end
	file = File.open(file_name, "w")
	idx = 0
	pcu_test_area_end = false
	lines.each {|line|
		if pcu_test_area_idx < idx && !pcu_test_area_end
			testfuncs.each {|e|
				pcu_test_lines.each {|v|
					if v =~ /\b#{e}\b/
						file << v
					end
				}
				if added_testfuncs.include?(e)
					file << "#{indent}PCU_TEST(#{added_testfuncs[added_testfuncs.index(e)]}),#{nl}"
				end
			}
			pcu_test_area_end = true
			file << line
		else
			file << line
		end
		idx += 1
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
	excluded_flag = false
	excluded.each {|ex|
		if fname =~ Regexp.new(ex)
			excluded_flag = true
			break
		end
	}
	if !excluded_flag
		register_tests(fname)
	end
}
if $main_file
	register_suite_methods($main_file)
end

