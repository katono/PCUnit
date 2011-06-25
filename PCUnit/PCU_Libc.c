#include "PCU_Libc.h"
#include "PCUnit.h"

#if defined(PCU_NO_VSNPRINTF) || defined(PCU_NO_MALLOC) || defined(PCU_NO_SETJMP) || defined(PCU_NO_LIBC) || defined(PCU_NO_FLOATINGPOINT)
static PCU_Putchar putchar_func;
static PCU_Getchar getchar_func;
#else
#include <stdio.h>
static PCU_Putchar putchar_func = putchar;
static PCU_Getchar getchar_func = getchar;
#endif

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

#if defined(PCU_NO_VSNPRINTF) || defined(PCU_NO_LIBC)

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
static int dec2ascii(char *ascii, int ascii_size, unsigned int dec, unsigned long flags)
{
	int i;
	char tmp[TMP_SIZE];
	const char * const num_str = "0123456789";
	int signed_dec = (int) dec;
	if (dec == 0) {
		tmp[0] = '0';
		i = 1;
		if (IS_SET_PLUS_FLAG(flags) || IS_SET_SPACE_FLAG(flags)) {
			tmp[i++] = IS_SET_PLUS_FLAG(flags) ? '+' : ' ';
		}
	} else if (IS_SET_SIGNED_FLAG(flags) && signed_dec < 0) {
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
		if (IS_SET_PLUS_FLAG(flags) || IS_SET_SPACE_FLAG(flags)) {
			tmp[i++] = IS_SET_PLUS_FLAG(flags) ? '+' : ' ';
		}
	}
	return set_ascii(ascii, ascii_size, tmp, i, flags);
}

static int hex2ascii(char *ascii, int ascii_size, size_t hex, unsigned long flags)
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

static int PCU_snprintf_aux(char *buf, size_t size, const char *format, size_t *arg_list)
{
	int i;
	const char *p = format;
	size_t arg_idx = 0;
	const char *tmp_str;
	size_t tmp_val;
	unsigned long flags = 0;
	int inc;
	const int buf_size = (int) size;

	if (buf_size <= 0) return -1;

	i = 0;
	if (buf_size == 1) goto end;
	while (*p != '\0') {
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
			int width = (int) arg_list[arg_idx++];
			if (width < 0) {
				SET_LEFT_FLAG(flags);
				width = -width;
			}
			SET_WIDTH(flags, width);
			p++;
		}
		if (*p == 'h' || *p == 'l') {
			p++;
		}
		switch (*p) {
		case 'c':
			buf[i++] = (char) arg_list[arg_idx++];
			if (i >= buf_size - 1) goto end;
			p++;
			break;
		case 'd':
		case 'i':
		case 'u':
			tmp_val = arg_list[arg_idx++];
			if (*p != 'u') {
				SET_SIGNED_FLAG(flags);
			}
			inc = dec2ascii(&buf[i], buf_size - 1 - i, (unsigned int) tmp_val, flags);
			i += inc;
			if (i >= buf_size - 1) goto end;
			p++;
			break;
		case 'p':
			buf[i++] = '0';
			if (i >= buf_size - 1) goto end;
			buf[i++] = 'x';
			if (i >= buf_size - 1) goto end;
		case 'x':
		case 'X':
			tmp_val = arg_list[arg_idx++];
			if (*p == 'X') {
				SET_LARGEX_FLAG(flags);
			}
			inc = hex2ascii(&buf[i], buf_size - 1 - i, tmp_val, flags);
			i += inc;
			if (i >= buf_size - 1) goto end;
			p++;
			break;
		case 's':
			tmp_str = (const char *) arg_list[arg_idx++];
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

int PCU_snprintf0(char *buf, size_t size, const char *format)
{
	size_t arg_list[1];
	arg_list[0] = 0;
	return PCU_snprintf_aux(buf, size, format, arg_list);
}

int PCU_snprintf1(char *buf, size_t size, const char *format, size_t arg1)
{
	size_t arg_list[1];
	arg_list[0] = arg1;
	return PCU_snprintf_aux(buf, size, format, arg_list);
}

int PCU_snprintf2(char *buf, size_t size, const char *format, size_t arg1, size_t arg2)
{
	size_t arg_list[2];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	return PCU_snprintf_aux(buf, size, format, arg_list);
}

int PCU_snprintf3(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3)
{
	size_t arg_list[3];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	return PCU_snprintf_aux(buf, size, format, arg_list);
}

int PCU_snprintf4(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4)
{
	size_t arg_list[4];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	arg_list[3] = arg4;
	return PCU_snprintf_aux(buf, size, format, arg_list);
}

int PCU_snprintf5(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5)
{
	size_t arg_list[5];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	arg_list[3] = arg4;
	arg_list[4] = arg5;
	return PCU_snprintf_aux(buf, size, format, arg_list);
}

int PCU_snprintf6(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6)
{
	size_t arg_list[6];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	arg_list[3] = arg4;
	arg_list[4] = arg5;
	arg_list[5] = arg6;
	return PCU_snprintf_aux(buf, size, format, arg_list);
}

int PCU_snprintf7(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6, size_t arg7)
{
	size_t arg_list[7];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	arg_list[3] = arg4;
	arg_list[4] = arg5;
	arg_list[5] = arg6;
	arg_list[6] = arg7;
	return PCU_snprintf_aux(buf, size, format, arg_list);
}

int PCU_snprintf8(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6, size_t arg7, size_t arg8)
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
	return PCU_snprintf_aux(buf, size, format, arg_list);
}

int PCU_snprintf9(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6, size_t arg7, size_t arg8, size_t arg9)
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
	return PCU_snprintf_aux(buf, size, format, arg_list);
}

static int PCU_printf_aux(const char *format, size_t *arg_list)
{
	extern char PCU_msg_buf[];
	char *p = PCU_msg_buf;
	int ret;

	if (!putchar_func) {
		return -1;
	}
	ret = PCU_snprintf_aux(p, PCU_MSG_BUF_SIZE, format, arg_list);
	while (*p) {
		putchar_func((int) *(p++));
	}
	return ret;
}

int PCU_printf0(const char *format)
{
	size_t arg_list[1];
	arg_list[0] = 0;
	return PCU_printf_aux(format, arg_list);
}

int PCU_printf1(const char *format, size_t arg1)
{
	size_t arg_list[1];
	arg_list[0] = arg1;
	return PCU_printf_aux(format, arg_list);
}

int PCU_printf2(const char *format, size_t arg1, size_t arg2)
{
	size_t arg_list[2];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	return PCU_printf_aux(format, arg_list);
}

int PCU_printf3(const char *format, size_t arg1, size_t arg2, size_t arg3)
{
	size_t arg_list[3];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	return PCU_printf_aux(format, arg_list);
}

#if 0 /* NOT USED */
int PCU_printf4(const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4)
{
	size_t arg_list[4];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	arg_list[3] = arg4;
	return PCU_printf_aux(format, arg_list);
}

int PCU_printf5(const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5)
{
	size_t arg_list[5];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	arg_list[3] = arg4;
	arg_list[4] = arg5;
	return PCU_printf_aux(format, arg_list);
}

int PCU_printf6(const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6)
{
	size_t arg_list[6];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	arg_list[3] = arg4;
	arg_list[4] = arg5;
	arg_list[5] = arg6;
	return PCU_printf_aux(format, arg_list);
}

int PCU_printf7(const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6, size_t arg7)
{
	size_t arg_list[7];
	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	arg_list[3] = arg4;
	arg_list[4] = arg5;
	arg_list[5] = arg6;
	arg_list[6] = arg7;
	return PCU_printf_aux(format, arg_list);
}

int PCU_printf8(const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6, size_t arg7, size_t arg8)
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
	return PCU_printf_aux(format, arg_list);
}

int PCU_printf9(const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6, size_t arg7, size_t arg8, size_t arg9)
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
	return PCU_printf_aux(format, arg_list);
}
#endif

#else

#include <stdarg.h>
#include <stdio.h>
int PCU_snprintf(char *buf, size_t size, const char *format, ...)
{
	int ret;
	va_list ap;
	va_start(ap, format);
	ret = vsnprintf(buf, size, format, ap);
	va_end(ap);
	return ret;
}

int PCU_printf(const char *format, ...)
{
	extern char PCU_msg_buf[];
	char *p = PCU_msg_buf;
	int ret;
	va_list ap;

	if (!putchar_func) {
		return -1;
	}
	va_start(ap, format);
	ret = vsnprintf(p, PCU_MSG_BUF_SIZE, format, ap);
	va_end(ap);
	while (*p) {
		putchar_func((int) *(p++));
	}
	return ret;
}

#endif

#if defined(PCU_NO_MALLOC) || defined(PCU_NO_LIBC)

#ifndef PCU_MAX_FAILURE_NUM
#define PCU_MAX_FAILURE_NUM	64
#endif

#ifndef PCU_STRING_POOL_SIZE
#define PCU_STRING_POOL_SIZE	4096
#endif

/* 
 * malloc for PCU_TestFailure
 */
static PCU_TestFailure failure_pool[PCU_MAX_FAILURE_NUM];

void *PCU_malloc(size_t size)
{
	PCU_TestFailure *p = failure_pool;
	const PCU_TestFailure * const end = &failure_pool[PCU_MAX_FAILURE_NUM];
	(void) size;
	for (; p != end; p++) {
		/* exprを使用フラグとして使う */
		if (p->expr == 0) {
			return p;
		}
	}
	return 0;
}

void PCU_free(void *ptr)
{
	if (!ptr) return;
	((PCU_TestFailure *) ptr)->expr = 0;
}

/* 
 * malloc for string
 */
static char str_pool[PCU_STRING_POOL_SIZE];
static char *str_pool_next_ptr = str_pool;

char *PCU_str_malloc(size_t size)
{
	char *p = str_pool_next_ptr;
	const char * const end = &str_pool[PCU_STRING_POOL_SIZE];
	if (str_pool_next_ptr + size > end) {
		return 0;
	}
	str_pool_next_ptr += size;
	return p;
}

void PCU_str_free(char *ptr)
{
	(void) ptr;
	str_pool_next_ptr = str_pool;
}

#endif

#ifdef PCU_NO_LIBC

size_t PCU_strlen(const char *s)
{
	size_t i = 0;
	for (; *s; i++, s++) ;
	return i;
}

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

char *PCU_strcpy(char *dst, const char *src)
{
	char *d = dst;
	while (*src) {
		*(d++) = *(src++);
	}
	*d = '\0';
	return dst;
}

char *PCU_strncpy(char *dst, const char *src, size_t len)
{
	size_t i;
	for (i = 0; i < len && src[i] != '\0'; i++) {
		dst[i] = src[i];
	}
	for (; i < len; i++) {
		dst[i] = '\0';
	}
	return dst;
}

void *PCU_memset(void *b, int c, size_t len)
{
	char *p = b;
	for (; len > 0; len--) {
		*(p++) = (char) c;
	}
	return b;
}

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

/* baseは0,10,16のみ対応
 * baseが0の場合の8進数未対応
 * オーバーフロー未対応 */
long PCU_strtol(const char *s, char **endptr, int base)
{
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
