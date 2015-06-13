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
$type_int = Array.new
$type_float = Array.new
$type_string = Array.new
$type_wstring = Array.new
$type_tstring = Array.new
$type_ptr = Array.new
$additional_include_files = Array.new
$others = false

def usage()
	print <<-"EOB"
Usage: pcunit_mock.rb header_file ... [OPTIONS]
    -d DIR         output directory
    -e PATTERN     excluded header_file pattern
    -s [SRC_DIR]   you can use functions defined at the original source file in SRC_DIR
    -p PREFIX      prefix of mock file (default: mock_)
    --others       you can use a created mock with other testing frameworks like CppUTest, GoogleTest, Unity, etc
    --include FILE       additional include file
    --type-int TYPE      user-defined integer type
    --type-float TYPE    user-defined floating point number type
    --type-string TYPE   user-defined string type
    --type-wstring TYPE  user-defined wstring type
    --type-tstring TYPE  user-defined tstring type
    --type-ptr TYPE      user-defined pointer type


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
opt.on('--type-int VAL') {|v| $type_int.push v }
opt.on('--type-float VAL') {|v| $type_float.push v }
opt.on('--type-string VAL') {|v| $type_string.push v }
opt.on('--type-wstring VAL') {|v| $type_wstring.push v }
opt.on('--type-tstring VAL') {|v| $type_tstring.push v }
opt.on('--type-ptr VAL') {|v| $type_ptr.push v }
opt.on('--include VAL') {|v| $additional_include_files.push v }
opt.on('--others') { $others = true }
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
			$additional_include_files.each { |v|
				if v =~ /<.*>/
					f.puts '#include ' + v
				else
					f.puts '#include "' + v + '"'
				end
			}
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
			if $others
				f.puts "#ifdef _WIN32"
				f.puts "#define _CRT_SECURE_NO_WARNINGS"
				f.puts "#endif"
			end
			f.puts "#include \"#{@mock_basename}.h\""
			if $others
				f.puts "#include <stdio.h>"
				f.puts "#include <string.h>"
				use_wchar_h = false
				use_tchar_h = false
				@func_decl_list.each { |fd|
					fd.params.each { |param|
						if param[0] =~ /\bwchar(_t)?\s*\*$/i || param[0] =~ /\bLPC?WSTR$/ || $type_wstring.include?(param[0])
							use_wchar_h = true
						elsif param[0] =~ /\b(TCHAR\s*\*|LPC?TSTR)$/ || $type_tstring.include?(param[0])
							use_tchar_h = true
						end
					}
				}
				if use_wchar_h
					f.puts "#include <wchar.h>"
				end
				if use_tchar_h
					f.puts "#include <tchar.h>"
				end
				if $include_original_flag
					f.puts "#include <stddef.h>"
				end
				f.puts
				f.puts "void PCU_mock_fail(const char *msg);"
			else
				f.puts "#include \"PCUnit/PCUnit.h\""
			end
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
			if $others
				f.puts "	int failed;"
				f.puts "	char msg[1024];"
			end
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
			if $others
				if $include_original_flag
					size = "offsetof(struct #{@mock_basename}_t, msg)"
				else
					size = "sizeof #{@mock_basename}"
				end
				f.puts "	memset(&#{@mock_basename}, 0, #{size});"
			else
				f.puts "	char *p = (char *) &#{@mock_basename};"
				f.puts "	size_t i;"
				f.puts "	for (i = 0; i < sizeof #{@mock_basename}; i++) {"
				f.puts "		*(p++) = 0;"
				f.puts "	}"
			end
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
			if $others
				f.puts "static void #{@mock_basename}_fail(const char *msg)"
				f.puts '{'
				f.puts "	#{@mock_basename}.failed = 1;"
				f.puts "	PCU_mock_fail(msg);"
				f.puts '}'
				f.puts
			end
			f.puts "#ifdef _MSC_VER"
			f.puts '#define LINE_FORMAT	"(%u) "'
			f.puts "#else"
			f.puts '#define LINE_FORMAT	":%u"'
			f.puts "#endif"
			f.puts
			f.puts "void #{@mock_basename}_#{$function_name[:verify]}_aux(const char *file, unsigned int line)"
			f.puts "{"
			if $others
				f.puts "	if (#{@mock_basename}.failed) {"
			else
				f.puts "	if (PCU_test_has_failed()) {"
			end
			if $include_original_flag
				f.puts "		#{@mock_basename}_#{$function_name[:init]}();"
			end
			f.puts "		return;"
			f.puts "	}"
			@func_decl_list.each { |fd|
				f.puts "	if (#{@mock_basename}.#{fd.name}_expectations || (#{@mock_basename}.#{fd.name}_funcptr && #{@mock_basename}.#{fd.name}_expected_num_calls >= 0)) {"
				f.puts "		if (#{@mock_basename}.#{fd.name}_expected_num_calls != #{@mock_basename}.#{fd.name}_actual_num_calls) {"
				printf_args = "\"%s\" LINE_FORMAT \": Expected %d calls of #{fd.name}(), but was %d\", file, line, #{@mock_basename}.#{fd.name}_expected_num_calls, #{@mock_basename}.#{fd.name}_actual_num_calls"
				if $others
					f.puts "			sprintf(#{@mock_basename}.msg, #{printf_args});"
					if $include_original_flag
						f.puts "			#{@mock_basename}_#{$function_name[:init]}();"
					end
					f.puts "			PCU_mock_fail(#{@mock_basename}.msg);"
				else
					f.puts "			PCU_FAIL(PCU_format(#{printf_args}));"
					if $include_original_flag
						f.puts "			#{@mock_basename}_#{$function_name[:init]}();"
					end
				end
				f.puts "			return;"
				f.puts "		}"
				f.puts "	}"
			}
			f.puts "}"
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
				f.puts "	if (#{@mock_basename}.#{fd.name}_expectations) {"
				if fd.ret_type != "void" || !(fd.params.size == 0 || fd.params[0][0] == "void")
					local_expectation = get_local_var_name(fd, "expectation", 1)
				end
				if local_expectation
					f.puts "		const #{fd.name}_Expectation *#{local_expectation};"
					if $others
						local_offset = get_local_var_name(fd, "offset_offset", 6)
						f.puts "		int #{local_offset} = 0;"
					end
				end
				f.puts "		if (#{@mock_basename}.#{fd.name}_expected_num_calls <= #{@mock_basename}.#{fd.name}_actual_num_calls) {"
				printf_args = "\"%s\" LINE_FORMAT \": Expected %d calls of #{fd.name}(), but was more\", #{@mock_basename}.#{fd.name}_file, #{@mock_basename}.#{fd.name}_line, #{@mock_basename}.#{fd.name}_expected_num_calls"
				if $others
					f.puts "			sprintf(#{@mock_basename}.msg, #{printf_args});"
					f.puts "			#{@mock_basename}_fail(#{@mock_basename}.msg);"
				else
					f.puts "			PCU_FAIL(PCU_format(#{printf_args}));"
				end
				f.puts "		}"
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
						printf_args = "\"%s\" LINE_FORMAT \": call #%d: Parameter '#{param[1]}' of #{fd.name}() is unexpected value\\n\", #{@mock_basename}.#{fd.name}_file, #{@mock_basename}.#{fd.name}_line, #{@mock_basename}.#{fd.name}_actual_num_calls + 1"
						if $others
							puts_param = Proc.new { |format, expected, actual|
								f.puts "				#{local_offset} = sprintf(#{@mock_basename}.msg, #{printf_args});"
								f.puts "				#{local_offset} += sprintf(#{@mock_basename}.msg + #{local_offset}, \"  expected : #{format}\\n\", #{expected});"
								f.puts "				#{local_offset} += sprintf(#{@mock_basename}.msg + #{local_offset}, \"  actual   : #{format}\\n\", #{actual});"
								f.puts "				#{@mock_basename}_fail(#{@mock_basename}.msg);"
							}
						else
							printf_args.sub!("\\n", "")
							msg = "PCU_format(#{printf_args})"
						end
						f.puts "		if (!#{local_expectation}->ignored.#{param[1]}) {"
						if param[0] =~ /\b((un)?signed\s+)?\b([su]?char|_*[su]?int(8|16|32|64|128)?(_t)?|[su](8|16|32|64|128)|[su]?short|[su]?long|^s?size_t|^ptrdiff_t|^bool|^byte|^word|^dword)$/i || param[0] =~ /\b(un)?signed$/ || param[0] =~ /enum\s+\w+$/ || $type_int.include?(param[0])
							if $others
								f.puts "			if (#{local_expectation}->expected.#{param[1]} != #{param[1]}) {"
								puts_param.call("0x%x (%d)", "#{local_expectation}->expected.#{param[1]}, #{local_expectation}->expected.#{param[1]}", "#{param[1]}, #{param[1]}")
								f.puts "			}"
							else
								f.puts "			PCU_ASSERT_EQUAL_MESSAGE(#{local_expectation}->expected.#{param[1]}, #{param[1]}, #{msg});"
							end
						elsif param[0] =~ /\b(float|double)$/i || param[0] =~ /\bf(32|64)$/i || $type_float.include?(param[0])
							if $others
								f.puts "			if (#{local_expectation}->expected.#{param[1]} != #{param[1]}) {"
								puts_param.call("%g", "#{local_expectation}->expected.#{param[1]}", "#{param[1]}")
								f.puts "			}"
							else
								f.puts "			PCU_ASSERT_DOUBLE_EQUAL_MESSAGE(#{local_expectation}->expected.#{param[1]}, #{param[1]}, 0, #{msg});"
							end
						elsif param[0] =~ /\bchar\s*\*$/i || param[0] =~ /\bLPC?STR$/ || $type_string.include?(param[0])
							if $others
								f.puts "			if ((#{local_expectation}->expected.#{param[1]} != #{param[1]}) && (!#{local_expectation}->expected.#{param[1]} || !#{param[1]} || strcmp((const char *) #{local_expectation}->expected.#{param[1]}, (const char *) #{param[1]}))) {"
								exp_p = get_local_var_name(fd, "exp_p_exp_p", 5)
								act_p = get_local_var_name(fd, "act_p_act_p", 5)
								exp_q = get_local_var_name(fd, "exp_q_exp_q", 5)
								act_q = get_local_var_name(fd, "act_q_act_q", 5)
								f.puts "				const char *#{exp_p} = #{local_expectation}->expected.#{param[1]} ? #{local_expectation}->expected.#{param[1]} : \"NULL\";"
								f.puts "				const char *#{act_p} = #{param[1]} ? #{param[1]} : \"NULL\";"
								f.puts "				const char *#{exp_q} = #{local_expectation}->expected.#{param[1]} ? \"\\\"\" : \"\";"
								f.puts "				const char *#{act_q} = #{param[1]} ? \"\\\"\" : \"\";"
								puts_param.call("%s%s%s (%p)", "#{exp_q}, #{exp_p}, #{exp_q}, #{local_expectation}->expected.#{param[1]}", "#{act_q}, #{act_p}, #{act_q}, #{param[1]}")
								f.puts "			}"
							else
								f.puts "			PCU_ASSERT_STRING_EQUAL_MESSAGE(#{local_expectation}->expected.#{param[1]}, #{param[1]}, #{msg});"
							end
						elsif param[0] =~ /\bwchar(_t)?\s*\*$/i || param[0] =~ /\bLPC?WSTR$/ || $type_wstring.include?(param[0])
							if $others
								f.puts "			if ((#{local_expectation}->expected.#{param[1]} != #{param[1]}) && (!#{local_expectation}->expected.#{param[1]} || !#{param[1]} || wcscmp((const wchar_t *) #{local_expectation}->expected.#{param[1]}, (const wchar_t *) #{param[1]}))) {"
								exp_p = get_local_var_name(fd, "exp_p_exp_p", 5)
								act_p = get_local_var_name(fd, "act_p_act_p", 5)
								exp_q1 = get_local_var_name(fd, "exp_q1_exp_q1", 6)
								act_q1 = get_local_var_name(fd, "act_q1_act_q1", 6)
								exp_q2 = get_local_var_name(fd, "exp_q2_exp_q2", 6)
								act_q2 = get_local_var_name(fd, "act_q2_act_q2", 6)
								f.puts "				const wchar_t *#{exp_p} = #{local_expectation}->expected.#{param[1]} ? #{local_expectation}->expected.#{param[1]} : L\"NULL\";"
								f.puts "				const wchar_t *#{act_p} = #{param[1]} ? #{param[1]} : L\"NULL\";"
								f.puts "				const char *#{exp_q1} = #{local_expectation}->expected.#{param[1]} ? \"L\\\"\" : \"\";"
								f.puts "				const char *#{act_q1} = #{param[1]} ? \"L\\\"\" : \"\";"
								f.puts "				const char *#{exp_q2} = #{local_expectation}->expected.#{param[1]} ? \"\\\"\" : \"\";"
								f.puts "				const char *#{act_q2} = #{param[1]} ? \"\\\"\" : \"\";"
								puts_param.call("%s%ls%s (%p)", "#{exp_q1}, #{exp_p}, #{exp_q2}, #{local_expectation}->expected.#{param[1]}", "#{act_q1}, #{act_p}, #{act_q2}, #{param[1]}")
								f.puts "			}"
							else
								f.puts "			PCU_ASSERT_STRINGW_EQUAL_MESSAGE(#{local_expectation}->expected.#{param[1]}, #{param[1]}, #{msg});"
							end
						elsif param[0] =~ /\b(TCHAR\s*\*|LPC?TSTR)$/ || $type_tstring.include?(param[0])
							if $others
								f.puts "			if ((#{local_expectation}->expected.#{param[1]} != #{param[1]}) && (!#{local_expectation}->expected.#{param[1]} || !#{param[1]} || _tcscmp((const TCHAR *) #{local_expectation}->expected.#{param[1]}, (const TCHAR *) #{param[1]}))) {"
								exp_p = get_local_var_name(fd, "exp_p_exp_p", 5)
								act_p = get_local_var_name(fd, "act_p_act_p", 5)
								exp_q1 = get_local_var_name(fd, "exp_q1_exp_q1", 6)
								act_q1 = get_local_var_name(fd, "act_q1_act_q1", 6)
								exp_q2 = get_local_var_name(fd, "exp_q2_exp_q2", 6)
								act_q2 = get_local_var_name(fd, "act_q2_act_q2", 6)
								f.puts "				const TCHAR *#{exp_p} = #{local_expectation}->expected.#{param[1]} ? #{local_expectation}->expected.#{param[1]} : _T(\"NULL\");"
								f.puts "				const TCHAR *#{act_p} = #{param[1]} ? #{param[1]} : _T(\"NULL\");"
								f.puts "#if defined(_UNICODE) || defined(UNICODE)"
								f.puts "				const char *#{exp_q1} = #{local_expectation}->expected.#{param[1]} ? \"L\\\"\" : \"\";"
								f.puts "				const char *#{act_q1} = #{param[1]} ? \"L\\\"\" : \"\";"
								f.puts "#define TSTRING_FORMAT	\"%ls\""
								f.puts "#else"
								f.puts "				const char *#{exp_q1} = #{local_expectation}->expected.#{param[1]} ? \"\\\"\" : \"\";"
								f.puts "				const char *#{act_q1} = #{param[1]} ? \"\\\"\" : \"\";"
								f.puts "#define TSTRING_FORMAT	\"%s\""
								f.puts "#endif"
								f.puts "				const char *#{exp_q2} = #{local_expectation}->expected.#{param[1]} ? \"\\\"\" : \"\";"
								f.puts "				const char *#{act_q2} = #{param[1]} ? \"\\\"\" : \"\";"
								puts_param.call("%s\" TSTRING_FORMAT \"%s (%p)", "#{exp_q1}, #{exp_p}, #{exp_q2}, #{local_expectation}->expected.#{param[1]}", "#{act_q1}, #{act_p}, #{act_q2}, #{param[1]}")
								f.puts "#undef TSTRING_FORMAT"
								f.puts "			}"
							else
								f.puts "			PCU_ASSERT_STRINGT_EQUAL_MESSAGE(#{local_expectation}->expected.#{param[1]}, #{param[1]}, #{msg});"
							end
						elsif param[0] =~ /\*/ || $type_ptr.include?(param[0])
							if $others
								f.puts "			if (#{local_expectation}->expected.#{param[1]} != #{param[1]}) {"
								puts_param.call("%p", "#{local_expectation}->expected.#{param[1]}", "#{param[1]}")
								f.puts "			}"
							else
								f.puts "			PCU_ASSERT_PTR_EQUAL_MESSAGE(#{local_expectation}->expected.#{param[1]}, #{param[1]}, #{msg});"
							end
						elsif param[0] =~ /funcptr\d+_t/
							if $others
								f.puts "			if (#{local_expectation}->expected.#{param[1]} != #{param[1]}) {"
								puts_param.call("%p", "#{local_expectation}->expected.#{param[1]}", "#{param[1]}")
								f.puts "			}"
							else
								f.puts "			PCU_ASSERT_PTR_EQUAL_MESSAGE(#{local_expectation}->expected.#{param[1]}, #{param[1]}, #{msg});"
							end
						elsif param[0] =~ /array\d+_t/
							if $others
								f.puts "			if (memcmp(#{local_expectation}->expected.#{param[1]}, #{param[1]}, sizeof(#{local_expectation}->expected.#{param[1]}))) {"
								puts_param.call("%p", "#{local_expectation}->expected.#{param[1]}", "#{param[1]}")
								f.puts "			}"
							else
								f.puts "			PCU_ASSERT_MEMORY_EQUAL_MESSAGE(#{local_expectation}->expected.#{param[1]}, #{param[1]}, sizeof(#{param[1]}), 1, #{msg});"
							end
						else
							if $others
								f.puts "			if (memcmp(&#{local_expectation}->expected.#{param[1]}, &#{param[1]}, sizeof(#{local_expectation}->expected.#{param[1]}))) {"
								i = get_local_var_name(fd, "iiiii", 1)
								exp_p = get_local_var_name(fd, "exp_p_exp_p", 5)
								act_p = get_local_var_name(fd, "act_p_act_p", 5)
								exp_offset = get_local_var_name(fd, "exp_offset_exp_offset", 10)
								act_offset = get_local_var_name(fd, "act_offset_act_offset", 10)
								exp_buf = get_local_var_name(fd, "exp_buf_exp_buf", 7)
								act_buf = get_local_var_name(fd, "act_buf_act_buf", 7)
								max_num = 32
								f.puts "				const unsigned char *#{exp_p} = (const unsigned char *) &#{local_expectation}->expected.#{param[1]};"
								f.puts "				const unsigned char *#{act_p} = (const unsigned char *) &#{param[1]};"
								f.puts "				int #{exp_offset} = 0;"
								f.puts "				int #{act_offset} = 0;"
								f.puts "				char #{exp_buf}[3 * #{(max_num+1).to_s} + 1];"
								f.puts "				char #{act_buf}[3 * #{(max_num+1).to_s} + 1];"
								f.puts "				int #{i};"
								f.puts "				for (#{i} = 0; #{i} < sizeof #{local_expectation}->expected.#{param[1]} && #{i} < #{max_num.to_s}; #{i}++) {"
								f.puts "					#{exp_offset} += sprintf(#{exp_buf} + #{exp_offset}, \"%02x \", #{exp_p}[#{i}]);"
								f.puts "					#{act_offset} += sprintf(#{act_buf} + #{act_offset}, \"%02x \", #{act_p}[#{i}]);"
								f.puts "				}"
								f.puts "				if (sizeof #{local_expectation}->expected.#{param[1]} > #{max_num.to_s}) {"
								f.puts "					#{exp_offset} += sprintf(#{exp_buf} + #{exp_offset}, \"...\");"
								f.puts "					#{act_offset} += sprintf(#{act_buf} + #{act_offset}, \"...\");"
								f.puts "				}"
								puts_param.call("%s", "#{exp_buf}", "#{act_buf}")
								f.puts "			}"
							else
								f.puts "			PCU_ASSERT_MEMORY_EQUAL_MESSAGE(&#{local_expectation}->expected.#{param[1]}, &#{param[1]}, sizeof(#{param[1]}), 1, #{msg});"
							end
						end
						f.puts "		}"
					}
				end
				if fd.ret_type != "void"
					f.puts "		#{local_ret} = #{local_expectation}->retval;"
				end
				f.puts "	}"
				f.puts "	if (#{@mock_basename}.#{fd.name}_funcptr) {"
				if va_list_name != ''
					f.puts "		va_list #{va_list_name};"
				end
				f.puts "		if (#{@mock_basename}.#{fd.name}_expected_num_calls >= 0) {"
				f.puts "			if (#{@mock_basename}.#{fd.name}_expected_num_calls <= #{@mock_basename}.#{fd.name}_actual_num_calls) {"
				printf_args = "\"%s\" LINE_FORMAT \": Expected %d calls of #{fd.name}(), but was more\", #{@mock_basename}.#{fd.name}_file, #{@mock_basename}.#{fd.name}_line, #{@mock_basename}.#{fd.name}_expected_num_calls"
				if $others
					f.puts "				sprintf(#{@mock_basename}.msg, #{printf_args});"
					f.puts "				#{@mock_basename}_fail(#{@mock_basename}.msg);"
				else
					f.puts "				PCU_FAIL(PCU_format(#{printf_args}));"
				end
				f.puts "			}"
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
				f.puts "	}"
				f.puts "	if (!#{@mock_basename}.#{fd.name}_expectations && !#{@mock_basename}.#{fd.name}_funcptr) {"
				printf_args = "\"Call #{fd.name}_#{$function_name[:expect]}() or #{fd.name}_#{$function_name[:set_callback]}()\""
				if $others
					f.puts "		#{@mock_basename}_fail(#{printf_args});"
				else
					f.puts "		PCU_FAIL(#{printf_args});"
				end
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
				f.puts "	if (!#{@mock_basename}.#{fd.name}_expectations) {"
				f.puts "		#{@mock_basename}.#{fd.name}_expected_num_calls = expected_num_calls;"
				f.puts "		#{@mock_basename}.#{fd.name}_file = file;"
				f.puts "		#{@mock_basename}.#{fd.name}_line = line;"
				f.puts "	}"
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
