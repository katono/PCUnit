#!/usr/bin/ruby


File.open("target.h", "w") {|f|
	f.print <<-"EOB"
#ifndef TARGET_H_INCLUDED
#define TARGET_H_INCLUDED

#include <stddef.h>
#include <stdint.h>

#define IGNORE	\\
void ignore(void);\\
void ignore(void);

typedef struct STRUCT_t {
	int a;
	int (*b)(int, int);
	struct {
		int a;
		int (*b)(int, int);
	} c;
} STRUCT_t;

typedef union UNION_t {
	unsigned int a;
	double b;
	struct {
		unsigned int a:4;
		unsigned int b:1;
		unsigned int  :3;
	} c;
} UNION_t;

typedef enum ENUM_t {
	E_000,
	E_001
} ENUM_t;

	EOB

	types = [
		"char",
		"signed char",
		"unsigned char",
		"const char",
		"const signed char",
		"const unsigned char",
		"int",
		"signed int",
		"unsigned int",
		"signed",
		"unsigned",
		"const int",
		"const signed int",
		"const unsigned int",
		"const signed",
		"const unsigned",
		"short int",
		"signed short int",
		"unsigned short int",
		"const short int",
		"const signed short int",
		"const unsigned short int",
		"short",
		"signed short",
		"unsigned short",
		"const short",
		"const signed short",
		"const unsigned short",
		"long int",
		"signed long int",
		"unsigned long int",
		"const long int",
		"const signed long int",
		"const unsigned long int",
		"long",
		"signed long",
		"unsigned long",
		"const long",
		"const signed long",
		"const unsigned long",
		"long long int",
		"signed long long int",
		"unsigned long long int",
		"const long long int",
		"const signed long long int",
		"const unsigned long long int",
		"long long",
		"signed long long",
		"unsigned long long",
		"const long long",
		"const signed long long",
		"const unsigned long long",
		#  "__int64",
		#  "signed __int64",
		#  "unsigned __int64",
		#  "const __int64",
		#  "const signed __int64",
		#  "const unsigned __int64",
		"uint32_t",
		"const uint32_t",
		"size_t",
		"const size_t",
		"ptrdiff_t",
		"const ptrdiff_t",
		"wchar_t",
		"const wchar_t",
		#  "TCHAR",
		#  "const TCHAR",
		#  "LPCTSTR",
		#  "LPTSTR",
		"float",
		"const float",
		"double",
		"const double",
		"long double",
		"const long double",
		"struct STRUCT_t",
		"const struct STRUCT_t",
		"STRUCT_t",
		"const STRUCT_t",
		"union UNION_t",
		"const union UNION_t",
		"UNION_t",
		"const UNION_t",
		"enum ENUM_t",
		"const enum ENUM_t",
		"ENUM_t",
		"const ENUM_t",
		"void",
		"const void",
		"",
	]

	types.each {|t|
		count = 0
		name = t.gsub(/\s/, '_')
		if t == ""
			f.puts "void f_none();"
			next
		end
		t2 = t.sub(/const /, '')
		if !(t =~ /void/)
			f.puts "#{t2} f_#{name}_arg(#{t} a);"
			f.puts "void f_#{name}_#{count += 1}(#{t}\ne);"
			f.puts "#{t2} f_#{name}_args(#{t} a, #{t} b, #{t} c, #{t} d);"
		end
		f.puts "#{t} *f_#{name}_ptr(#{t} *a);"
		f.puts "void f_#{name}_varg(#{t}* a, ...);"
		f.puts "#{t}* f_#{name}_#{count += 1}(#{t} * ret);"
		f.puts "#{t}* f_#{name}_#{count += 1}(#{t} * const a);"
		f.puts "#{t} *f_#{name}_#{count += 1}(#{t}*a);"
		f.puts "#{t} *f_#{name}_#{count += 1}(#{t} *a, #{t} *b,\n	#{t} *c);"
		f.puts "#{t} **f_#{name}_#{count += 1}(#{t} **a);"
		f.puts "#{t}** f_#{name}_#{count += 1}(#{t}** a);"
		f.puts "#{t} ** f_#{name}_#{count += 1}(#{t} ** a);"
		f.puts "#{t}**f_#{name}_#{count += 1}(#{t}**a);"
		if !(t =~ /void/)
			f.puts "#{t} *f_#{name}_ary(#{t} a[1]);"
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t} a[]);"
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t} a []);"
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t} a[], #{t} b[]);"
		end
		f.puts "#{t} *f_#{name}_#{count += 1}(#{t} *a[]);"
		f.puts "#{t}* f_#{name}_#{count += 1}(#{t}* a[]);"
		f.puts "#{t} *f_#{name}_#{count += 1}(#{t} * a[]);"
		f.puts "#{t} *f_#{name}_#{count += 1}(#{t} * a []);"
		f.puts "#{t} *f_#{name}_#{count += 1}(#{t}*a[]);"
		if t != "const void"
			f.puts "#{t2} f_#{name}_omitted(#{t});"
		end
		if !(t =~ /void/)
			f.puts "#{t2} f_#{name}_#{count += 1}(#{t}, #{t});"
		end
		f.puts "#{t} *f_#{name}_#{count += 1}(#{t} *,... );"
		f.puts "#{t}* f_#{name}_#{count += 1}(#{t}*);"
		f.puts "#{t} *f_#{name}_#{count += 1}(#{t} * const);"
		f.puts "#{t} *f_#{name}_#{count += 1}(#{t} *, #{t} *, #{t} *);"
		f.puts "#{t} **f_#{name}_#{count += 1}(#{t} **);"
		f.puts "#{t}**f_#{name}_#{count += 1}(#{t}**);"
		if !(t =~ /void/)
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t} [1]);"
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t} []);"
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t}[]);"
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t} [], #{t} [], #{t} [], #{t} []);"
		end
		f.puts "#{t} *f_#{name}_#{count += 1}(#{t} *[]);"
		f.puts "#{t} *f_#{name}_#{count += 1}(#{t}* []);"
		f.puts "#{t} *f_#{name}_#{count += 1}(#{t}*[]);"
		f.puts "#{t} *f_#{name}_#{count += 1}(void);"
		f.puts "#{t} *f_#{name}_#{count += 1}();"

		if !(t =~ /void/)
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t2} (*)(#{t}));"
			f.puts "#{t} *f_#{name}_#{count += 1}(void (*)(#{t}),\n	#{t2} (*)(#{t}),\n	#{t2} (*)(#{t}));"
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t2}(*)(#{t}, #{t}));"
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t}*(*)(#{t}, #{t}, #{t}));"
			f.puts "#{t} *f_#{name}_fptr(#{t2} (*a)(#{t} A));"
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t2}(*a)(#{t} A, #{t} B));"
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t2}(*a)(#{t} A, #{t} B), #{t2}(*b)(#{t} A, #{t} B));"
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t}*(*a)(#{t} A, #{t} B, #{t} C));"
		elsif t == "const void"
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t2} (*)(#{t} *));"
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t}*(*)(#{t} *, #{t} *));"
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t2} (*a)(#{t} *));"
			f.puts "#{t} *f_#{name}_fptr(#{t}*(*a)(#{t} *A, #{t} *B));"
			f.puts "#{t} *f_#{name}_#{count += 1}(void (*a)(#{t} *A, #{t} *B), #{t}*(*b)(#{t} *A, #{t} *B));"
		else
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t2} (*)(#{t}));"
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t2} (*)(#{t}),\n	#{t2} (*)(#{t}),\n	#{t2} (*)(#{t}));"
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t}*(*)(#{t} *, #{t} *));"
			f.puts "#{t} *f_#{name}_#{count += 1}(#{t2} (*a)(#{t}));"
			f.puts "#{t} *f_#{name}_fptr(#{t}*(*a)(#{t} *A, #{t} *B));"
			f.puts "#{t} *f_#{name}_#{count += 1}(void (*a)(#{t} *A, #{t} *B), #{t}*(*b)(#{t} *A, #{t} *B));"
		end
		f.puts
	}

	f.print <<-"EOB"

#endif /* TARGET_H_INCLUDED */
	EOB
}
