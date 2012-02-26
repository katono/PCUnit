#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "PCU_Libc.h"
#include "PCUnit.h"

char PCU_msg_buf[PCU_MESSAGE_BUF_SIZE];
char PCU_printf_buf[64];

static PCU_Putchar putchar_func;
static PCU_Getchar getchar_func;

void PCU_set_putchar(PCU_Putchar func)
{
	putchar_func = func;
}

void PCU_set_getchar(PCU_Getchar func)
{
	getchar_func = func;
}

int PCU_getchar(void)
{
	if (!getchar_func) {
		return -1;
	}
	return getchar_func();
}

void PCU_puts(const char *str)
{
	const char *p = str;

	if (!putchar_func) {
		return;
	}
	while (*p) {
		putchar_func((int) *(p++));
	}
}

#if defined(PCU_NO_VSPRINTF) || defined(PCU_NO_LIBC)

#include <stddef.h>
#include <limits.h>

/* 
 * flags:
 *
 * bit 0: left_flag
 * bit 1: zero_flag
 * bit 2: plus_flag
 * bit 3: space_flag
 * bit 4: signed_flag('u':0, 'd'or'i':1)
 * bit 5: X_flag('x':0, 'X':1)
 * bit 6: sharp_flag
 *
 * bit 8-15: width
 */
#define SET_LEFT_FLAG(flags)	((flags) |= 0x0001)
#define SET_ZERO_FLAG(flags)	((flags) |= 0x0002)
#define SET_PLUS_FLAG(flags)	((flags) |= 0x0004)
#define SET_SPACE_FLAG(flags)	((flags) |= 0x0008)
#define SET_SIGNED_FLAG(flags)	((flags) |= 0x0010)
#define SET_LARGEX_FLAG(flags)	((flags) |= 0x0020)
#define SET_SHARP_FLAG(flags)	((flags) |= 0x0040)
#define SET_WIDTH(flags, width)	((flags) |= ((unsigned char)((width) & 0xff) << 8))

#define IS_SET_LEFT_FLAG(flags)		((flags) & 0x0001)
#define IS_SET_ZERO_FLAG(flags)		((flags) & 0x0002)
#define IS_SET_PLUS_FLAG(flags)		((flags) & 0x0004)
#define IS_SET_SPACE_FLAG(flags)	((flags) & 0x0008)
#define IS_SET_SIGNED_FLAG(flags)	((flags) & 0x0010)
#define IS_SET_LARGEX_FLAG(flags)	((flags) & 0x0020)
#define IS_SET_SHARP_FLAG(flags)	((flags) & 0x0040)
#define GET_WIDTH(flags)			((unsigned char)(((flags) & 0xff00) >> 8))

static int set_ascii(char *ascii, int ascii_size, const char *tmp, int size, unsigned long flags)
{
	int i;
	const int width = GET_WIDTH(flags);
	int full_flag = 0;
	if (width > 1) {
		if (IS_SET_LEFT_FLAG(flags)) {
			for (i = 0; i < size; i++) {
				if (i < ascii_size) {
					ascii[i] = tmp[size - i - 1];
				} else {
					full_flag = 1;
					goto end;
				}
			}
			if (width > size) {
				for (i = 0; i < width - size; i++) {
					if (size + i < ascii_size) {
						ascii[size + i] = ' ';
					} else {
						full_flag = 1;
						goto end;
					}
				}
				size = width;
			}
		} else {
			if (width > size) {
				if (IS_SET_ZERO_FLAG(flags) && 
						(IS_SET_SHARP_FLAG(flags) || IS_SET_PLUS_FLAG(flags) || IS_SET_SPACE_FLAG(flags) || 
						(IS_SET_SIGNED_FLAG(flags) && tmp[size - 1] == '-'))) {
					const int n = IS_SET_SHARP_FLAG(flags) ? 2 /* "0[xX]" */ : 1 /* "[ +-]" */;
					for (i = 0; i < n; i++) {
						if (i < ascii_size) {
							ascii[i] = tmp[size - i - 1];
						} else {
							full_flag = 1;
							goto end;
						}
					}
					for (i = 0; i < width - size; i++) {
						if (i + n < ascii_size) {
							ascii[i + n] = '0';
						} else {
							full_flag = 1;
							goto end;
						}
					}
					for (i = 0; i < size - n; i++) {
						if (width - size + i + n < ascii_size) {
							ascii[width - size + i + n] = tmp[size - i - 1 - n];
						} else {
							full_flag = 1;
							goto end;
						}
					}
				} else {
					for (i = 0; i < width - size; i++) {
						if (i < ascii_size) {
							ascii[i] = IS_SET_ZERO_FLAG(flags) ? '0' : ' ';
						} else {
							full_flag = 1;
							goto end;
						}
					}
					for (i = 0; i < size; i++) {
						if (width - size + i < ascii_size) {
							ascii[width - size + i] = tmp[size - i - 1];
						} else {
							full_flag = 1;
							goto end;
						}
					}
				}
				size = width;
			} else {
				for (i = 0; i < size; i++) {
					if (i < ascii_size) {
						ascii[i] = tmp[size - i - 1];
					} else {
						full_flag = 1;
						goto end;
					}
				}
			}
		}
	} else {
		for (i = 0; i < size; i++) {
			if (i < ascii_size) {
				ascii[i] = tmp[size - i - 1];
			} else {
				full_flag = 1;
				goto end;
			}
		}
	}
end:
	return full_flag ? ascii_size : size;
}

#define TMP_SIZE	32

#ifdef PCU_NO_DIV32
typedef unsigned int PCU_udec_t;
typedef int PCU_dec_t;
#define MIN_DEC		INT_MIN
#else
typedef PCU_size_t PCU_udec_t;
typedef PCU_ssize_t PCU_dec_t;
#if !defined(PCU_NO_LONGLONG) && !defined(PCU_NO_STDARG)
#if (defined(_MSC_VER) && _MSC_VER < 1400) /* VC2005 */
#define MIN_DEC		_I64_MIN
#else
#define MIN_DEC		LLONG_MIN
#endif
#else
#define MIN_DEC		LONG_MIN
#endif
#endif
static int dec2ascii(char *ascii, int ascii_size, PCU_udec_t dec, unsigned long flags)
{
	int i;
	char tmp[TMP_SIZE];
	const char * const num_str = "0123456789";
	PCU_dec_t signed_dec = (PCU_dec_t) dec;
	if (dec == 0) {
		tmp[0] = '0';
		i = 1;
		if (IS_SET_PLUS_FLAG(flags) || IS_SET_SPACE_FLAG(flags)) {
			tmp[i++] = IS_SET_PLUS_FLAG(flags) ? '+' : ' ';
		}
	} else if (IS_SET_SIGNED_FLAG(flags) && signed_dec < 0 && signed_dec != MIN_DEC) {
		signed_dec = -signed_dec;
		for (i = 0; signed_dec > 0 && i < TMP_SIZE - 1; i++) {
			tmp[i] = num_str[signed_dec % 10];
			signed_dec /= 10;
		}
		tmp[i++] = '-';
	} else {
		for (i = 0; dec > 0 && i < TMP_SIZE - 1; i++) {
			tmp[i] = num_str[dec % 10];
			dec /= 10;
		}
		if (signed_dec == MIN_DEC) {
			tmp[i++] = '-';
		} else if (IS_SET_PLUS_FLAG(flags) || IS_SET_SPACE_FLAG(flags)) {
			tmp[i++] = IS_SET_PLUS_FLAG(flags) ? '+' : ' ';
		}
	}
	return set_ascii(ascii, ascii_size, tmp, i, flags);
}

static int hex2ascii(char *ascii, int ascii_size, PCU_size_t hex, unsigned long flags)
{
	int i;
	char tmp[TMP_SIZE];
	const char * const num_str = IS_SET_LARGEX_FLAG(flags) ? "0123456789ABCDEF" : "0123456789abcdef";
	if (hex == 0) {
		tmp[0] = '0';
		i = 1;
	} else {
		for (i = 0; hex > 0 && i < TMP_SIZE; i++) {
			tmp[i] = num_str[hex & 0xf];
			hex >>= 4;
		}
		if (IS_SET_SHARP_FLAG(flags)) {
			tmp[i++] = IS_SET_LARGEX_FLAG(flags) ? 'X' : 'x';
			tmp[i++] = '0';
		}
	}
	return set_ascii(ascii, ascii_size, tmp, i, flags);
}

#ifdef PCU_NO_STDARG
static int PCU_vsnprintf(char *buf, size_t size, const char *format, PCU_size_t *arg_list)
#else
static int PCU_vsnprintf(char *buf, size_t size, const char *format, va_list arg_list)
#endif
{
	int i;
	const char *p = format;
#ifdef PCU_NO_STDARG
	size_t arg_idx = 0;
#endif
	const char *tmp_str;
	PCU_size_t tmp_val;
	unsigned long flags = 0;
	int inc;
	const int buf_size = (int) size;

	if (buf_size <= 0) return -1;

	i = 0;
	if (buf_size == 1) goto end;
	while (*p != '\0') {
		int long_flag = 0;
		if (*p != '%') {
			buf[i++] = *(p++);
			if (i >= buf_size - 1) goto end;
			continue;
		}
		p++;
		if (*p == '%') {
			buf[i++] = *(p++);
			if (i >= buf_size - 1) goto end;
			continue;
		}
		if (*p == ' ') {
			SET_SPACE_FLAG(flags);
			p++;
		}
		if (*p == '+') {
			SET_PLUS_FLAG(flags);
			p++;
		}
		if (*p == '#') {
			SET_SHARP_FLAG(flags);
			p++;
		}
		if (*p == '-') {
			SET_LEFT_FLAG(flags);
			p++;
		}
		if (*p == ' ') {
			SET_SPACE_FLAG(flags);
			p++;
		}
		if (*p == '+') {
			SET_PLUS_FLAG(flags);
			p++;
		}
		if (*p == '0') {
			SET_ZERO_FLAG(flags);
			p++;
		}
		if ('1' <= *p && *p <= '9') {
			int width = *p - '0';
			p++;
			if ('0' <= *p && *p <= '9') {
				width = 10 * width + (*p - '0');
				p++;
			}
			SET_WIDTH(flags, width);
		} else if (*p == '*') {
#ifdef PCU_NO_STDARG
			int width = (int) arg_list[arg_idx++];
#else
			int width = va_arg(arg_list, int);
#endif
			if (width < 0) {
				SET_LEFT_FLAG(flags);
				width = -width;
			}
			SET_WIDTH(flags, width);
			p++;
		}
		if (*p == 'h') {
			p++;
		} else if (*p == 'l') {
			long_flag = 1;
			p++;
			if (*p == 'l') {
				p++;
				long_flag = 2;
			}
		}
#ifdef _MSC_VER
		else if (*p == 'I') {
			long_flag = 1;
			p++;
			if (*p == '6') {
				p++;
				if (*p == '4') {
					p++;
					long_flag = 2;
				}
			} else if (*p == '3') {
				p++;
				if (*p == '2') {
					p++;
				}
			}
		}
#endif
		switch (*p) {
		case 'c':
#ifdef PCU_NO_STDARG
			buf[i++] = (char) arg_list[arg_idx++];
#else
			buf[i++] = (char) va_arg(arg_list, int);
#endif
			if (i >= buf_size - 1) goto end;
			p++;
			break;
		case 'd':
		case 'i':
		case 'u':
#ifdef PCU_NO_STDARG
			tmp_val = arg_list[arg_idx++];
#else
			switch (long_flag) {
			case 1:
				tmp_val = va_arg(arg_list, ptrdiff_t);
				break;
			case 2:
				tmp_val = va_arg(arg_list, PCU_ssize_t);
				break;
			case 0:
			default:
				tmp_val = va_arg(arg_list, int);
				break;
			}
#endif
			if (*p != 'u') {
				SET_SIGNED_FLAG(flags);
			} else if (long_flag != 2) {
				tmp_val &= ((size_t) -1);
			}
			inc = dec2ascii(&buf[i], buf_size - 1 - i, (PCU_udec_t) tmp_val, flags);
			i += inc;
			if (i >= buf_size - 1) goto end;
			p++;
			break;
		case 'p':
			buf[i++] = '0';
			if (i >= buf_size - 1) goto end;
			buf[i++] = 'x';
			if (i >= buf_size - 1) goto end;
			long_flag = 1;
		case 'x':
		case 'X':
#ifdef PCU_NO_STDARG
			tmp_val = arg_list[arg_idx++];
#else
			switch (long_flag) {
			case 1:
				tmp_val = va_arg(arg_list, size_t);
				break;
			case 2:
				tmp_val = va_arg(arg_list, PCU_size_t);
				break;
			case 0:
			default:
				tmp_val = va_arg(arg_list, unsigned int);
				break;
			}
#endif
			if (*p == 'X') {
				SET_LARGEX_FLAG(flags);
			}
			inc = hex2ascii(&buf[i], buf_size - 1 - i, tmp_val, flags);
			i += inc;
			if (i >= buf_size - 1) goto end;
			p++;
			break;
		case 's':
#ifdef PCU_NO_STDARG
			tmp_str = (const char *) arg_list[arg_idx++];
#else
			tmp_str = va_arg(arg_list, const char *);
#endif
			while (*tmp_str != '\0') {
				buf[i++] = *(tmp_str++);
				if (i >= buf_size - 1) goto end;
			}
			p++;
			break;
		default:
			p++;
			break;
		}
		flags = 0;
	}
end:
	buf[i] = '\0';
	return i;
}
#endif

#ifdef PCU_NO_STDARG

const char *PCU_format_aux0(char *buf, size_t size, const char *format)
{
	size_t arg_list[1];
	arg_list[0] = 0;
	PCU_vsnprintf(buf, size, format, arg_list);
	return buf;
}

const char *PCU_format_aux1(char *buf, size_t size, const char *format, 
		size_t arg1)
{
	size_t arg_list[1];
	arg_list[0] = arg1;
	PCU_vsnprintf(buf, size, format, arg_list);
	return buf;
}

const char *PCU_format_aux2(char *buf, size_t size, const char *format, 
		size_t arg1, size_t arg2)
{
	size_t arg_list[2];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	PCU_vsnprintf(buf, size, format, arg_list);
	return buf;
}

const char *PCU_format_aux3(char *buf, size_t size, const char *format, 
		size_t arg1, size_t arg2, size_t arg3)
{
	size_t arg_list[3];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	PCU_vsnprintf(buf, size, format, arg_list);
	return buf;
}

const char *PCU_format_aux4(char *buf, size_t size, const char *format, 
		size_t arg1, size_t arg2, size_t arg3, size_t arg4)
{
	size_t arg_list[4];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	arg_list[3] = arg4;
	PCU_vsnprintf(buf, size, format, arg_list);
	return buf;
}

const char *PCU_format_aux5(char *buf, size_t size, const char *format, 
		size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5)
{
	size_t arg_list[5];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	arg_list[3] = arg4;
	arg_list[4] = arg5;
	PCU_vsnprintf(buf, size, format, arg_list);
	return buf;
}

const char *PCU_format_aux6(char *buf, size_t size, const char *format, 
		size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6)
{
	size_t arg_list[6];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	arg_list[3] = arg4;
	arg_list[4] = arg5;
	arg_list[5] = arg6;
	PCU_vsnprintf(buf, size, format, arg_list);
	return buf;
}

const char *PCU_format_aux7(char *buf, size_t size, const char *format, 
		size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6, 
		size_t arg7)
{
	size_t arg_list[7];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	arg_list[3] = arg4;
	arg_list[4] = arg5;
	arg_list[5] = arg6;
	arg_list[6] = arg7;
	PCU_vsnprintf(buf, size, format, arg_list);
	return buf;
}

const char *PCU_format_aux8(char *buf, size_t size, const char *format, 
		size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6, 
		size_t arg7, size_t arg8)
{
	size_t arg_list[8];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	arg_list[3] = arg4;
	arg_list[4] = arg5;
	arg_list[5] = arg6;
	arg_list[6] = arg7;
	arg_list[7] = arg8;
	PCU_vsnprintf(buf, size, format, arg_list);
	return buf;
}

const char *PCU_format_aux9(char *buf, size_t size, const char *format, 
		size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6, 
		size_t arg7, size_t arg8, size_t arg9)
{
	size_t arg_list[9];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	arg_list[3] = arg4;
	arg_list[4] = arg5;
	arg_list[5] = arg6;
	arg_list[6] = arg7;
	arg_list[7] = arg8;
	arg_list[8] = arg9;
	PCU_vsnprintf(buf, size, format, arg_list);
	return buf;
}

#else

#if !defined(PCU_NO_VSPRINTF) && !defined(PCU_NO_LIBC)
#include <stdio.h>
#endif

const char *PCU_format(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
#if !defined(PCU_NO_VSPRINTF) && !defined(PCU_NO_LIBC)
	vsprintf(PCU_msg_buf, format, ap);
#else
	PCU_vsnprintf(PCU_msg_buf, PCU_MESSAGE_BUF_SIZE, format, ap);
#endif
	va_end(ap);
	return PCU_msg_buf;
}

void PCU_printf(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
#if !defined(PCU_NO_VSPRINTF) && !defined(PCU_NO_LIBC)
	vsprintf(PCU_printf_buf, format, ap);
#else
	PCU_vsnprintf(PCU_printf_buf, sizeof PCU_printf_buf, format, ap);
#endif
	va_end(ap);
	PCU_puts(PCU_printf_buf);
}

#if !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)

const char *PCU_formatW(const wchar_t *format, ...)
{
	wchar_t PCU_msgw_buf[PCU_MESSAGE_BUF_SIZE];
	va_list ap;
	va_start(ap, format);
#if (defined(_MSC_VER) && _MSC_VER < 1400) /* VC2005 */
	_vsnwprintf(PCU_msgw_buf, PCU_MESSAGE_BUF_SIZE, format, ap);
#else
	vswprintf(PCU_msgw_buf, PCU_MESSAGE_BUF_SIZE, format, ap);
#endif
	va_end(ap);
	return PCU_format("%ls", PCU_msgw_buf);
}

#endif
#endif


#ifdef PCU_NO_LIBC

int PCU_strcmp(const char *s1, const char *s2)
{
	while (*s1 || *s2) {
		if (*s1 != *s2) {
			return (*s1 < *s2) ? -1 : 1;
		}
		s1++;
		s2++;
	}
	return 0;
}

int PCU_strncmp(const char *s1, const char *s2, size_t len)
{
	while (len > 0 && (*s1 || *s2)) {
		if (*s1 != *s2) {
			return (*s1 < *s2) ? -1 : 1;
		}
		s1++;
		s2++;
		len--;
	}
	return 0;
}

void *PCU_memset(void *b, int c, size_t len)
{
	char *p = b;
	for (; len > 0; len--) {
		*(p++) = (char) c;
	}
	return b;
}

void *PCU_memcpy(void *dst, const void *src, size_t len)
{
	char *p = dst;
	const char *q = src;
	for (; len > 0; len--) {
		*(p++) = *(q++);
	}
	return dst;
}

#ifndef PCU_NO_CONSOLE_RUN
static int ascii2hex(char c)
{
	if ('0' <= c && c <= '9') {
		return c - '0';
	}
	if ('a' <= c && c <= 'f') {
		return c - 'a' + 0xA;
	}
	if ('A' <= c && c <= 'F') {
		return c - 'A' + 0xA;
	}
	return 0;
}

static int PCU_isdigit(char c)
{
	if ('0' <= c && c <= '9') {
		return 1;
	}
	return 0;
}

static int PCU_isxdigit(char c)
{
	if ('0' <= c && c <= '9') {
		return 1;
	}
	if ('a' <= c && c <= 'f') {
		return 1;
	}
	if ('A' <= c && c <= 'F') {
		return 1;
	}
	return 0;
}

static int PCU_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\f' || c == '\r' || c == '\n' || c == '\v') {
		return 1;
	}
	return 0;
}

long PCU_strtol(const char *s, char **endptr, int base)
{
	/* 
	 * supported base: 0, 10, 16
	 * no overflow check
	 */
	long ret = 0;
	int state = 0;
	int signed_state = 0;
	const char *p = s;

	if (base == 0) {
		ret = PCU_strtol(s, endptr, 10);
		if (ret == 0) {
			return PCU_strtol(s, endptr, 16);
		}
		return ret;
	} else if (base == 10) {
		for (; *p; p++) {
			if (state == 0 && signed_state == 0 && PCU_isspace(*p)) {
				continue;
			}
			if (PCU_isdigit(*p)) {
				state = 1;
				ret = (ret * 10) + ascii2hex(*p);
			} else if (state == 0 && signed_state == 0 && *p == '+') {
				signed_state = 2;
			} else if (state == 0 && signed_state == 0 && *p == '-') {
				signed_state = 1;
			} else {
				break;
			}
		}
	} else if (base == 16) {
		for (; *p; p++) {
			if (state == 0 && signed_state == 0 && PCU_isspace(*p)) {
				continue;
			}
			if (PCU_isxdigit(*p)) {
				if (state == 0 && *p == '0') {
					state = 1;
				} else {
					state = 3;
					ret = (ret << 4) + ascii2hex(*p);
				}
			} else if (state == 0 && signed_state == 0 && *p == '+') {
				signed_state = 2;
			} else if (state == 0 && signed_state == 0 && *p == '-') {
				signed_state = 1;
			} else if (state == 1 && (*p == 'x' || *p == 'X')) {
				state = 2;
			} else {
				break;
			}
		}
	}
	if (endptr) {
		if (state == 2 || (signed_state && state == 0)) {
			p = s;
		}
		*endptr = (char *) p;
	}
	return (signed_state == 1) ? -ret : ret;
}

unsigned long PCU_strtoul(const char *s, char **endptr, int base)
{
	return (unsigned long) PCU_strtol(s, endptr, base);
}

int PCU_atoi(const char *s)
{
	return (int) PCU_strtol(s, 0, 10);
}
#endif

#endif
