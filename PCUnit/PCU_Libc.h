#ifndef PCU_LIBC_H_INCLUDED
#define PCU_LIBC_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PCU_MESSAGE_BUF_SIZE
#if !defined(PCU_NO_VSPRINTF) && !defined(PCU_NO_LIBC)
#define PCU_MESSAGE_BUF_SIZE	512
#else
#define PCU_MESSAGE_BUF_SIZE	256
#endif
#endif

int PCU_getchar(void);
void PCU_puts(const char *str);


#ifdef PCU_NO_LIBC
# include <stddef.h>
# define PCU_STRCMP		PCU_strcmp
# define PCU_STRNCMP	PCU_strncmp
# define PCU_MEMSET		PCU_memset
# define PCU_MEMCPY		PCU_memcpy
# define PCU_ATOI		PCU_atoi
int PCU_strcmp(const char *s1, const char *s2);
int PCU_strncmp(const char *s1, const char *s2, size_t len);
void *PCU_memset(void *b, int c, size_t len);
void *PCU_memcpy(void *dst, const void *src, size_t len);
long PCU_strtol(const char *s, char **endptr, int base);
unsigned long PCU_strtoul(const char *s, char **endptr, int base);
int PCU_atoi(const char *s);
#else
# include <string.h>
# include <stdlib.h>
# define PCU_STRCMP		strcmp
# define PCU_STRNCMP	strncmp
# define PCU_MEMSET		memset
# define PCU_MEMCPY		memcpy
# define PCU_ATOI		atoi
#endif


#ifdef PCU_NO_STDARG
extern char PCU_printf_buf[64];
# define PCU_PRINTF0(f)                                     PCU_puts(PCU_format_aux0(PCU_printf_buf, sizeof PCU_printf_buf, f))
# define PCU_PRINTF1(f, a1)                                 PCU_puts(PCU_format_aux1(PCU_printf_buf, sizeof PCU_printf_buf, f, (size_t)(a1)))
# define PCU_PRINTF2(f, a1, a2)                             PCU_puts(PCU_format_aux2(PCU_printf_buf, sizeof PCU_printf_buf, f, (size_t)(a1), (size_t)(a2)))
# define PCU_PRINTF3(f, a1, a2, a3)                         PCU_puts(PCU_format_aux3(PCU_printf_buf, sizeof PCU_printf_buf, f, (size_t)(a1), (size_t)(a2), (size_t)(a3)))
# define PCU_PRINTF4(f, a1, a2, a3, a4)                     PCU_puts(PCU_format_aux4(PCU_printf_buf, sizeof PCU_printf_buf, f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4)))
# define PCU_PRINTF5(f, a1, a2, a3, a4, a5)                 PCU_puts(PCU_format_aux5(PCU_printf_buf, sizeof PCU_printf_buf, f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5)))
# define PCU_PRINTF6(f, a1, a2, a3, a4, a5, a6)             PCU_puts(PCU_format_aux6(PCU_printf_buf, sizeof PCU_printf_buf, f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5), (size_t)(a6)))
# define PCU_PRINTF7(f, a1, a2, a3, a4, a5, a6, a7)         PCU_puts(PCU_format_aux7(PCU_printf_buf, sizeof PCU_printf_buf, f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5), (size_t)(a6), (size_t)(a7)))
# define PCU_PRINTF8(f, a1, a2, a3, a4, a5, a6, a7, a8)     PCU_puts(PCU_format_aux8(PCU_printf_buf, sizeof PCU_printf_buf, f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5), (size_t)(a6), (size_t)(a7), (size_t)(a8)))
# define PCU_PRINTF9(f, a1, a2, a3, a4, a5, a6, a7, a8, a9) PCU_puts(PCU_format_aux9(PCU_printf_buf, sizeof PCU_printf_buf, f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5), (size_t)(a6), (size_t)(a7), (size_t)(a8), (size_t)(a9)))
const char *PCU_format_aux0(char *buf, size_t size, const char *format);
const char *PCU_format_aux1(char *buf, size_t size, const char *format, size_t arg1);
const char *PCU_format_aux2(char *buf, size_t size, const char *format, size_t arg1, size_t arg2);
const char *PCU_format_aux3(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3);
const char *PCU_format_aux4(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4);
const char *PCU_format_aux5(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5);
const char *PCU_format_aux6(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6);
const char *PCU_format_aux7(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6, size_t arg7);
const char *PCU_format_aux8(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6, size_t arg7, size_t arg8);
const char *PCU_format_aux9(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6, size_t arg7, size_t arg8, size_t arg9);
#else
# include <stdarg.h>
# define PCU_PRINTF0	PCU_printf
# define PCU_PRINTF1	PCU_printf
# define PCU_PRINTF2	PCU_printf
# define PCU_PRINTF3	PCU_printf
# define PCU_PRINTF4	PCU_printf
# define PCU_PRINTF5	PCU_printf
# define PCU_PRINTF6	PCU_printf
# define PCU_PRINTF7	PCU_printf
# define PCU_PRINTF8	PCU_printf
# define PCU_PRINTF9	PCU_printf
void PCU_printf(const char *format, ...);
#endif


#if defined(PCU_NO_SETJMP) || defined(PCU_NO_LIBC)
typedef int PCU_jmp_buf;
# define PCU_SETJMP(j)	((void)(j), 0)
# define PCU_LONGJMP(j, v)
#else
# include <setjmp.h>
typedef jmp_buf PCU_jmp_buf;
# define PCU_SETJMP		setjmp
# define PCU_LONGJMP	longjmp
#endif


#if !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
# include <wchar.h>
# define PCU_WCSCMP		wcscmp
# define PCU_WCSNCMP	wcsncmp
# define PCU_WCSLEN		wcslen
#endif

#ifdef __cplusplus
}
#endif

#endif /* PCU_LIBC_H_INCLUDED */
