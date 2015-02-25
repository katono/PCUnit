#!/usr/bin/ruby

if RUBY_VERSION >= '1.9'
	Encoding.default_external = "ASCII-8BIT"
end

require 'optparse'
require 'fileutils'
require 'pathname'

opt = OptionParser.new

$output_dir = "./"
$prefix = "mock_"
$include_original_flag = false
src_dir = ""
excluded = Array.new
$function_name = { :init => "init", :verify => "verify", :expect => "expect", :set_callback => "set_callback", :num_calls => "num_calls" }

def usage()
	print <<-"EOB"
Usage: pcunit_mock.rb header_file ... [-d DIR] [-e PATTERN] [-s [SRC_DIR]] [-p PREFIX]
    -d DIR         output directory
    -e PATTERN     excluded header_file pattern
    -s [SRC_DIR]   you can use functions defined at the original source file in SRC_DIR
    -p PREFIX      prefix of mock file (default: mock_)


	EOB
end

opt.on('-d VAL') {|v| $output_dir = v + "/" }
opt.on('-e VAL') {|v| excluded.push v }
opt.on('-s [VAL]') {|v|
	if v
		src_dir = v
	end
	$include_original_flag = true
}
opt.on('-p VAL') {|v| $prefix = v }
opt.on('-h', '--help') {
	usage
	exit
}


opt.parse!(ARGV)

if !ARGV[0]
	usage
	exit
end


FileUtils.makedirs($output_dir)

class FuncDecl
	attr_reader :decl, :name, :ret_type, :params, :callback_typedef, :callback_type
	def initialize(decl_str)
		@decl = decl_str.strip
		@name = decl_str[/(\w+)\s*\(/, 1]
		@ret_type = decl_str[0 ... decl_str.index(@name)].strip
		@params = get_params(decl_str[(decl_str.index(@name) + @name.length) ... decl_str.length])
		@callback_type = @name + "_Callback"
		@callback_typedef = "typedef " + decl_str.strip.sub(@name, "(*#{@callback_type})") + ";"
		if @callback_typedef.include?('...')
			@callback_typedef['...'] = 'va_list'
		end
		#  puts "decl: " + @decl
		#  puts "name: " + @name
		#  puts "ret_type: " + @ret_type
		#  p params
		#  puts "callback_typedef: " + @callback_typedef
		#  puts "callback_type: " + @callback_type
		#  puts
	end
	def is_omitted_short_long(p_array)
		if p_array[0] == 'short'
			if p_array.size == 2 && p_array[1] == 'int'
				return true
			end
		elsif p_array[0] == 'long'
			if p_array.size == 2 && p_array[1] == 'int'
				return true
			elsif p_array.size == 3 && p_array[1] == 'long' && p_array[2] == 'int'
				return true
			elsif p_array.size == 2 && p_array[1] == 'long'
				return true
			elsif p_array.size == 2 && p_array[1] == 'double'
				return true
			end
		end
		return false
	end
	def is_omitted_p_name(param)
		p_array = param.split(' ')
		p_array = p_array - ["const", "struct", "union", "enum"]
		if p_array.size > 1 && p_array[1] =~ /^(char|_*int(8|16|32|64|128)?|short|long)$/
			p_array = p_array - ["signed", "unsigned"]
		end
		if p_array.size == 1
			return true
		elsif p_array[p_array.size - 1] =~ /\*$/ || is_omitted_short_long(p_array)
			return true
		end
		return false
	end
	def get_param_normal(param)
		param.gsub!(/(\w)(\*+)(\w)/, '\1 \2\3')
		if is_omitted_p_name(param)
			p_type = param
			if param == 'void'
				p_name = ""
			elsif param == '...'
				p_name = ""
			else
				p_name = "mock_param" + @param_count.to_s
				@param_count += 1
			end
		else
			p_name = param[/\w+$/]
			p_type = param.sub(/\w+$/, '').strip
		end
		return [p_type, p_name]
	end
	def get_param_funcptr(param)
		p_type = "mock_#{@name}_funcptr#{@param_typedef_funcptr_count.to_s}_t"
		@param_typedef_funcptr_count += 1
		p_name = param[/\(\s*\*\s*(\w+)\s*\)/, 1]
		if !p_name
			p_name = "mock_param" + @param_count.to_s
			@param_count += 1
			p_typedef = param.sub(/(.*\(\s*\*\s*)(\s*\).*)/, "typedef \\1#{p_type}\\2;")
		else
			p_typedef = param.sub(/(.*\(\s*\*\s*)\w+(\s*\).*)/, "typedef \\1#{p_type}\\2;")
		end
		return [p_type, p_name, p_typedef]
	end
	def get_param_array(param)
		p_type = "mock_#{@name}_array#{@param_typedef_array_count.to_s}_t"
		@param_typedef_array_count += 1
		param.gsub!(/(\w)(\*+)(\w)/, '\1 \2\3')
		param.gsub!(/\s*\[\s*/, '[')
		param.gsub!(/\s*\]\s*/, ']')
		tmp = param[/([^\[]*)\[/, 1]
		if is_omitted_p_name(tmp)
			p_name = "mock_param" + @param_count.to_s
			@param_count += 1
			p_typedef = param.sub(/([^\[]*)(\[.*\])/, "typedef \\1 #{p_type}\\2;")
		else
			p_name = tmp[/\w+$/]
			p_typedef = param.sub(/(.*[^\w]+)\w+(\[.*\])/, "typedef \\1#{p_type}\\2;")
		end
		return [p_type, p_name, p_typedef]
	end
	def get_params(params_str)
		params_str.gsub!(/\((.*)\)/, '\1')
		params_str.strip!
		paren_nest = 0
		i = 0
		while i < params_str.length
			if params_str[i, 1] == "("
				paren_nest += 1
			elsif params_str[i, 1] == ")"
				paren_nest -= 1
			end
			if paren_nest == 0 && params_str[i, 1] == ","
				params_str[i, 1] = ";"
			end
			i += 1
		end
		@param_count = 1
		@param_typedef_funcptr_count = 1
		@param_typedef_array_count = 1
		param_list = Array.new
		params_str.split(/\s*;\s*/).each { |param|
			if param[/\(/]
				param_list.push get_param_funcptr(param)
			elsif param[/\[/]
				param_list.push get_param_array(param)
			else
				param_list.push get_param_normal(param)
			end
		}
		return param_list
	end
end

class MockGen
	attr_reader :basename
	def remove_struct_union(str)
		idx = str.index(/(struct|union)\s*\w*\s*\{/)
		if !idx
			return nil
		end
		brace_nest = 0
		brace_flag = false
		i = 0
		while i < str.length
			if i >= idx
				if str[i, 1] == "{"
					brace_flag = true
					brace_nest += 1
				elsif str[i, 1] == "}"
					brace_nest -= 1
				end
				if brace_flag && brace_nest == 0
					str[idx, i - idx] = ""
					break
				end
			end
			i += 1
		end
		return idx
	end
	def read_header(header_file)
		if !File.exist?(header_file)
			puts header_file + ": Not Found."
			return false
		end
		@header_file = header_file
		@func_decl_list = Array.new
		@basename = File.basename(@header_file, ".h")
		@mock_basename = $prefix + @basename
		@include_stdarg = false
		File.open(@header_file, "r") { |f|
			file_str = f.read
			file_str.gsub!(/\r/, '')
			file_str.gsub!(%r!//.*!, '')
			file_str.gsub!(/\\\n/, '')
			file_str.gsub!(/#.*\n/, ' ')
			file_str.gsub!(/\n/, ' ')
			file_str.gsub!(%r!/\*.*?\*/!, '')
			file_str.gsub!(/extern\s*"C"\s*\{(.*)\}/, '\1')
			file_str.gsub!(/(typedef\s+)?enum\s*\w*\s*\{[^;]*\}\s*\w*\s*;/, ';')
			while remove_struct_union(file_str)
			end
			file_str.gsub!(/typedef.*?;/, ';')
			file_str.gsub!(/extern\s+[^\(\)]+?;/, ';')
			file_str.gsub!(/extern\s+/, '')
			file_str.gsub!(/\t+/, ' ')
			file_str.gsub!(/  +/, ' ')

			file_str.split(/\s*;\s*/).each { |decl_str|
				if !@include_stdarg && decl_str.include?('...')
					@include_stdarg = true
				end
				if decl_str.include?('(') && !decl_str[/^[^\(]*\(\s*\*/] && !decl_str[/(\{|\||\^|&|<|>|\+|-|\/|%|!|~)/]
					@func_decl_list.push FuncDecl.new(decl_str)
				end
			}
		}
		return @func_decl_list.empty? ? false : true
	end
	def create_header()
		File.open($output_dir + @mock_basename + ".h", "w") { |f|
			included = "#{(@mock_basename).upcase}_H_INCLUDED"
			f.puts "#ifndef #{included}"
			f.puts "#define #{included}"
			f.puts
			header_path = Pathname.new(@header_file).expand_path
			f.puts '#include "' + header_path.relative_path_from(Pathname.new($output_dir).expand_path).to_s + '"'
			if @include_stdarg
				f.puts "#include <stdarg.h>"
			end
			f.puts
			f.puts "void #{@mock_basename}_#{$function_name[:init]}(void);"
			f.puts "#define #{@mock_basename}_#{$function_name[:verify]}()	#{@mock_basename}_#{$function_name[:verify]}_aux(__FILE__, __LINE__)"
			f.puts "void #{@mock_basename}_#{$function_name[:verify]}_aux(const char *file, unsigned int line);"
			f.puts
			f.puts
			@func_decl_list.each { |fd|
				fd.params.each { |param|
					if param.size >= 3
						f.puts param[2]
					end
				}
				f.puts "typedef struct {"
				if fd.ret_type != "void"
					f.puts "	" + fd.ret_type + (fd.ret_type[/ \*+$/] ? "" : " ") + "retval;"
				else
					f.puts "	int dummy;"
				end
				if fd.params.size > 0 && fd.params[0][0] != "void"
					f.puts "	struct {"
					fd.params.each { |param|
						if param[0] == '...'
							next
						end
						if param.size >= 3 && param[2][/\[\s*\]/]
							f.puts "		int dummy_#{param[1]};"
							next
						end
						s = ""
						s += param[0]
						s += (param[0][/ \*+$/] ? "" : " ") + param[1]
						f.puts "		#{s};"
					}
					f.puts "	} expected;"
					f.puts "	struct {"
					fd.params.each { |param|
						if param[0] == '...'
							next
						end
						f.puts "		unsigned int #{param[1]}:1;"
					}
					f.puts "	} ignored;"
				end
				f.puts "} #{fd.name}_Expectation;"
				f.puts fd.callback_typedef
				f.puts "#define #{fd.name}_#{$function_name[:expect]}(expectations, num)	#{fd.name}_#{$function_name[:expect]}_aux(expectations, num, __FILE__, __LINE__)"
				f.puts "void #{fd.name}_#{$function_name[:expect]}_aux(const #{fd.name}_Expectation *expectations, int num, const char *file, unsigned int line);"
				f.puts "#define #{fd.name}_#{$function_name[:set_callback]}(callback, expected_num_calls)	#{fd.name}_#{$function_name[:set_callback]}_aux(callback, expected_num_calls, __FILE__, __LINE__)"
				f.puts "void #{fd.name}_#{$function_name[:set_callback]}_aux(#{fd.callback_type} callback, int expected_num_calls, const char *file, unsigned int line);"
				f.puts "int #{fd.name}_#{$function_name[:num_calls]}(void);"
				f.puts
			}
			f.puts "#endif /* #{included} */"
		}
	end
	def create_src(src_file)
		File.open($output_dir + @mock_basename + ".c", "w") { |f|
			f.puts "#include \"#{@mock_basename}.h\""
			f.puts "#include \"PCUnit/PCUnit.h\""
			f.puts
			if $include_original_flag
				f.puts "#if " + ($include_original_flag ? "1" : "0")
				f.puts "#define #{@mock_basename.upcase}_INCLUDE_ORIGINAL"
				f.puts "#endif"
				f.puts
				f.puts "#ifdef #{@mock_basename.upcase}_INCLUDE_ORIGINAL"
				@func_decl_list.each { |fd|
					f.puts fd.decl.sub(fd.name, fd.name + "_original") + ";"
				}
				f.puts
				@func_decl_list.each { |fd|
					f.puts "#define #{fd.name} #{fd.name}_original"
				}
				f.puts
				src_path = Pathname.new(src_file).expand_path
				f.puts '#include "' + src_path.relative_path_from(Pathname.new($output_dir).expand_path).to_s + '"'
				f.puts
				@func_decl_list.each { |fd|
					f.puts "#undef " + fd.name
				}
				f.puts "#endif"
				f.puts
			end
			f.puts "struct #{@mock_basename}_t {"
			@func_decl_list.each { |fd|
				f.puts "	#{fd.callback_type} #{fd.name}_funcptr;"
			}
			@func_decl_list.each { |fd|
				f.puts "	int #{fd.name}_expected_num_calls;"
			}
			@func_decl_list.each { |fd|
				f.puts "	int #{fd.name}_actual_num_calls;"
			}
			@func_decl_list.each { |fd|
				f.puts "	const #{fd.name}_Expectation *#{fd.name}_expectations;"
			}
			@func_decl_list.each { |fd|
				f.puts "	const char *#{fd.name}_file;"
			}
			@func_decl_list.each { |fd|
				f.puts "	unsigned int #{fd.name}_line;"
			}
			f.puts "};"
			if $include_original_flag
				f.puts "#ifdef #{@mock_basename.upcase}_INCLUDE_ORIGINAL"
				f.puts "static struct #{@mock_basename}_t #{@mock_basename} = {"
				@func_decl_list.each { |fd|
					f.puts "	#{fd.name}_original,"
				}
				@func_decl_list.each { |fd|
					f.puts "	-1,"
				}
				f.puts "};"
				f.puts "#else"
			end
			f.puts "static struct #{@mock_basename}_t #{@mock_basename};"
			if $include_original_flag
				f.puts "#endif"
			end
			f.puts
			f.puts "void #{@mock_basename}_#{$function_name[:init]}(void)"
			f.puts "{"
			f.puts "	char *p = (char *) &#{@mock_basename};"
			f.puts "	size_t i;"
			f.puts "	for (i = 0; i < sizeof #{@mock_basename}; i++) {"
			f.puts "		*(p++) = 0;"
			f.puts "	}"
			if $include_original_flag
				f.puts "#ifdef #{@mock_basename.upcase}_INCLUDE_ORIGINAL"
				@func_decl_list.each { |fd|
					f.puts "	#{@mock_basename}.#{fd.name}_funcptr = #{fd.name}_original;"
				}
				@func_decl_list.each { |fd|
					f.puts "	#{@mock_basename}.#{fd.name}_expected_num_calls = -1;"
				}
				f.puts "#endif"
			end
			f.puts "}"
			f.puts
			f.puts "#ifdef _MSC_VER"
			f.puts '#define LINE_FORMAT	"(%u) "'
			f.puts "#else"
			f.puts '#define LINE_FORMAT	":%u"'
			f.puts "#endif"
			f.puts
			f.puts "void #{@mock_basename}_#{$function_name[:verify]}_aux(const char *file, unsigned int line)"
			f.puts "{"
			f.puts "	if (PCU_test_has_failed()) {"
			if $include_original_flag
				f.puts "		goto end;"
			else
				f.puts "		return;"
			end
			f.puts "	}"
			@func_decl_list.each { |fd|
				f.puts "	if ((#{@mock_basename}.#{fd.name}_expectations || #{@mock_basename}.#{fd.name}_funcptr) && #{@mock_basename}.#{fd.name}_expected_num_calls >= 0) {"
				f.puts "		PCU_ASSERT_EQUAL_MESSAGE(#{@mock_basename}.#{fd.name}_expected_num_calls, #{@mock_basename}.#{fd.name}_actual_num_calls, PCU_format(\"%s\" LINE_FORMAT \": Check the number of calls of #{fd.name}().\", file, line));"
				f.puts "		if (PCU_test_has_failed()) {"
				if $include_original_flag
					f.puts "			goto end;"
				else
					f.puts "			return;"
				end
				f.puts "		}"
				f.puts "	}"
			}
			if $include_original_flag
				f.puts "end:"
				f.puts "	#{@mock_basename}_#{$function_name[:init]}();"
			end
			f.puts "}"
			f.puts
			f.puts "static const char *#{@mock_basename}_ordinal(int num)"
			f.puts '{'
			f.puts '	switch (num) {'
			f.puts '	case 1:'
			f.puts '		return "st";'
			f.puts '	case 2:'
			f.puts '		return "nd";'
			f.puts '	case 3:'
			f.puts '		return "rd";'
			f.puts '	default:'
			f.puts '		return "th";'
			f.puts '	}'
			f.puts '}'
			f.puts
			f.puts
			def get_local_var_name(fd, base, idx)
				begin
					exit_flag = true
					name = base[0, idx]
					fd.params.each { |param|
						if param[1] == name
							idx += 1
							exit_flag = false
							break
						end
					}
				end until exit_flag
				return name
			end
			@func_decl_list.each { |fd|
				s = "("
				fd.params.each { |param|
					s += param[0]
					if !param[1].empty?
						s += (param[0][/ \*+$/] ? "" : " ") + param[1]
					end
					s += ", "
				}
				s.sub!(/, $/, '')
				s += ")"
				f.puts fd.ret_type + (fd.ret_type[/ \*+$/] ? "" : " ") + fd.name + s
				f.puts "{"
				if fd.ret_type != "void"
					local_ret = get_local_var_name(fd, "retval_retval", 3)
					f.puts "	" + fd.ret_type + (fd.ret_type[/ \*+$/] ? "" : " ") + "#{local_ret};"
				end
				f.puts "	if (#{@mock_basename}.#{fd.name}_expectations && #{@mock_basename}.#{fd.name}_expected_num_calls >= 0) {"
				if fd.ret_type != "void" || !(fd.params.size == 0 || fd.params[0][0] == "void")
					local_expectation = get_local_var_name(fd, "expectation", 1)
				end
				if local_expectation
					f.puts "		const #{fd.name}_Expectation *#{local_expectation};"
				end
				f.puts "		PCU_ASSERT_OPERATOR_MESSAGE(#{@mock_basename}.#{fd.name}_expected_num_calls, >, #{@mock_basename}.#{fd.name}_actual_num_calls, PCU_format(\"%s\" LINE_FORMAT \": Check the number of calls of #{fd.name}().\", #{@mock_basename}.#{fd.name}_file, #{@mock_basename}.#{fd.name}_line));"
				if local_expectation
					f.puts "		#{local_expectation} = #{@mock_basename}.#{fd.name}_expectations + #{@mock_basename}.#{fd.name}_actual_num_calls;"
				end
				prev_param_name = ''
				va_list_name = ''
				if !(fd.params.size == 0 || fd.params[0][0] == "void")
					fd.params.each { |param|
						if param[0] == '...'
							va_list_name = get_local_var_name(fd, "arg_list", 3)
							next
						end
						if param.size >= 3 && param[2][/\[\s*\]/]
							next
						end
						if va_list_name == ''
							prev_param_name = param[1]
						end
						msg = "PCU_format(\"%s\" LINE_FORMAT \": Check the parameter '#{param[1]}' of #{fd.name}() called for the %d%s time.\", #{@mock_basename}.#{fd.name}_file, #{@mock_basename}.#{fd.name}_line, #{@mock_basename}.#{fd.name}_actual_num_calls, #{@mock_basename}_ordinal(#{@mock_basename}.#{fd.name}_actual_num_calls))"
						f.puts "		if (!#{local_expectation}->ignored.#{param[1]}) {"
						if param[0] =~ /\b((un)?signed\s+)?\b(u?char|_*u?int(8|16|32|64|128)?(_t)?|u?short|u?long|^size_t|^ptrdiff_t|^bool|^byte|^word|^dword)$/i || param[0] =~ /\b(un)?signed$/ || param[0] =~ /enum\s+\w+$/
							f.puts "			PCU_ASSERT_EQUAL_MESSAGE(#{local_expectation}->expected.#{param[1]}, #{param[1]}, #{msg});"
						elsif param[0] =~ /\b(float|double)$/i
							f.puts "			PCU_ASSERT_DOUBLE_EQUAL_MESSAGE(#{local_expectation}->expected.#{param[1]}, #{param[1]}, 0, #{msg});"
						elsif param[0] =~ /\bchar\s*\*$/
							f.puts "			PCU_ASSERT_STRING_EQUAL_MESSAGE(#{local_expectation}->expected.#{param[1]}, #{param[1]}, #{msg});"
						elsif param[0] =~ /\bwchar_t\s*\*$/
							f.puts "			PCU_ASSERT_STRINGW_EQUAL_MESSAGE(#{local_expectation}->expected.#{param[1]}, #{param[1]}, #{msg});"
						elsif param[0] =~ /\b(TCHAR\s*\*|LPC?TSTR)$/
							f.puts "			PCU_ASSERT_STRINGT_EQUAL_MESSAGE(#{local_expectation}->expected.#{param[1]}, #{param[1]}, #{msg});"
						elsif param[0] =~ /\*/
							f.puts "			PCU_ASSERT_PTR_EQUAL_MESSAGE(#{local_expectation}->expected.#{param[1]}, #{param[1]}, #{msg});"
						elsif param[0] =~ /funcptr\d+_t/
							f.puts "			PCU_ASSERT_PTR_EQUAL_MESSAGE(#{local_expectation}->expected.#{param[1]}, #{param[1]}, #{msg});"
						elsif param[0] =~ /array\d+_t/
							f.puts "			PCU_ASSERT_MEMORY_EQUAL_MESSAGE(#{local_expectation}->expected.#{param[1]}, #{param[1]}, sizeof(#{param[1]}), 1, #{msg});"
						else
							f.puts "			PCU_ASSERT_MEMORY_EQUAL_MESSAGE(&#{local_expectation}->expected.#{param[1]}, &#{param[1]}, sizeof(#{param[1]}), 1, #{msg});"
						end
						f.puts "		}"
					}
				end
				if fd.ret_type != "void"
					f.puts "		#{local_ret} = #{local_expectation}->retval;"
				end
				f.puts "	} else if (#{@mock_basename}.#{fd.name}_funcptr) {"
				if va_list_name != ''
					f.puts "		va_list #{va_list_name};"
				end
				f.puts "		if (#{@mock_basename}.#{fd.name}_expected_num_calls >= 0) {"
				f.puts "			PCU_ASSERT_OPERATOR_MESSAGE(#{@mock_basename}.#{fd.name}_expected_num_calls, >, #{@mock_basename}.#{fd.name}_actual_num_calls, PCU_format(\"%s\" LINE_FORMAT \": Check the number of calls of #{fd.name}().\", #{@mock_basename}.#{fd.name}_file, #{@mock_basename}.#{fd.name}_line));"
				f.puts "		}"
				if va_list_name != ''
					f.puts "		va_start(#{va_list_name}, #{prev_param_name});"
				end
				if fd.ret_type != "void"
					s = "		#{local_ret} = "
				else
					s = "		"
				end
				s += "#{@mock_basename}.#{fd.name}_funcptr("
				fd.params.each { |param|
					if !param[1].empty?
						s += param[1]
						s += ", "
					end
				}
				if va_list_name != ''
					s += va_list_name
				end
				s.sub!(/, $/, '')
				s += ");"
				f.puts s
				if va_list_name != ''
					f.puts "		va_end(#{va_list_name});"
				end
				f.puts "	} else {"
				f.puts "		PCU_FAIL(\"Call #{fd.name}_#{$function_name[:expect]}() or #{fd.name}_#{$function_name[:set_callback]}().\");"
				f.puts "	}"
				f.puts "	#{@mock_basename}.#{fd.name}_actual_num_calls++;"
				if fd.ret_type != "void"
					f.puts "	return #{local_ret};"
				end
				f.puts "}"
				f.puts
				f.puts "void #{fd.name}_#{$function_name[:expect]}_aux(const #{fd.name}_Expectation *expectations, int num, const char *file, unsigned int line)"
				f.puts "{"
				f.puts "	#{@mock_basename}.#{fd.name}_expectations = expectations;"
				f.puts "	#{@mock_basename}.#{fd.name}_expected_num_calls = num;"
				f.puts "	#{@mock_basename}.#{fd.name}_file = file;"
				f.puts "	#{@mock_basename}.#{fd.name}_line = line;"
				f.puts "}"
				f.puts
				f.puts "void #{fd.name}_#{$function_name[:set_callback]}_aux(#{fd.callback_type} callback, int expected_num_calls, const char *file, unsigned int line)"
				f.puts "{"
				f.puts "	#{@mock_basename}.#{fd.name}_funcptr = callback;"
				f.puts "	#{@mock_basename}.#{fd.name}_expected_num_calls = expected_num_calls;"
				f.puts "	#{@mock_basename}.#{fd.name}_file = file;"
				f.puts "	#{@mock_basename}.#{fd.name}_line = line;"
				f.puts "}"
				f.puts
				f.puts "int #{fd.name}_#{$function_name[:num_calls]}(void)"
				f.puts "{"
				f.puts "	return #{@mock_basename}.#{fd.name}_actual_num_calls;"
				f.puts "}"
				f.puts
			}
		}
	end
end

ARGV.each {|name|
	Dir.glob(name) {|fname|
		excluded_flag = false
		excluded.each {|ex|
			if fname =~ Regexp.new(ex)
				excluded_flag = true
				break
			end
		}
		if excluded_flag
			next
		end
		gen = MockGen.new
		if gen.read_header(fname)
			gen.create_header()
			if File.directory? src_dir
				gen.create_src(src_dir + '/' + gen.basename + '.c')
			else
				gen.create_src(fname.sub(/\.h/, '.c'))
			end
		end
	}
}
