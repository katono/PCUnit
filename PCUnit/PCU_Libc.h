#ifndef PCU_LIBC_H_INCLUDED
#define PCU_LIBC_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

int PCU_getchar(void);


#if defined(PCU_NO_MALLOC) || defined(PCU_NO_LIBC)
# include <stddef.h> /* size_t */
# define PCU_MALLOC		PCU_malloc
# define PCU_FREE		PCU_free
# define PCU_STR_MALLOC	PCU_str_malloc
# define PCU_STR_FREE	PCU_str_free
void *PCU_malloc(size_t size);
void PCU_free(void *ptr);
char *PCU_str_malloc(size_t size);
void PCU_str_free(char *ptr);
#else
# include <stdlib.h>
# define PCU_MALLOC		malloc
# define PCU_FREE		free
# define PCU_STR_MALLOC	malloc
# define PCU_STR_FREE	free
#endif


#if defined(PCU_NO_VSNPRINTF) || defined(PCU_NO_LIBC)
# include <stddef.h> /* size_t */
# define PCU_SNPRINTF0(b, s, f)                                     PCU_snprintf0(b, s, f)
# define PCU_SNPRINTF1(b, s, f, a1)                                 PCU_snprintf1(b, s, f, (size_t)(a1))
# define PCU_SNPRINTF2(b, s, f, a1, a2)                             PCU_snprintf2(b, s, f, (size_t)(a1), (size_t)(a2))
# define PCU_SNPRINTF3(b, s, f, a1, a2, a3)                         PCU_snprintf3(b, s, f, (size_t)(a1), (size_t)(a2), (size_t)(a3))
# define PCU_SNPRINTF4(b, s, f, a1, a2, a3, a4)                     PCU_snprintf4(b, s, f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4))
# define PCU_SNPRINTF5(b, s, f, a1, a2, a3, a4, a5)                 PCU_snprintf5(b, s, f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5))
# define PCU_SNPRINTF6(b, s, f, a1, a2, a3, a4, a5, a6)             PCU_snprintf6(b, s, f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5), (size_t)(a6))
# define PCU_SNPRINTF7(b, s, f, a1, a2, a3, a4, a5, a6, a7)         PCU_snprintf7(b, s, f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5), (size_t)(a6), (size_t)(a7))
# define PCU_SNPRINTF8(b, s, f, a1, a2, a3, a4, a5, a6, a7, a8)     PCU_snprintf8(b, s, f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5), (size_t)(a6), (size_t)(a7), (size_t)(a8))
# define PCU_SNPRINTF9(b, s, f, a1, a2, a3, a4, a5, a6, a7, a8, a9) PCU_snprintf9(b, s, f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5), (size_t)(a6), (size_t)(a7), (size_t)(a8), (size_t)(a9))
# define PCU_PRINTF0(f)                                     PCU_printf0(f)
# define PCU_PRINTF1(f, a1)                                 PCU_printf1(f, (size_t)(a1))
# define PCU_PRINTF2(f, a1, a2)                             PCU_printf2(f, (size_t)(a1), (size_t)(a2))
# define PCU_PRINTF3(f, a1, a2, a3)                         PCU_printf3(f, (size_t)(a1), (size_t)(a2), (size_t)(a3))
# define PCU_PRINTF4(f, a1, a2, a3, a4)                     PCU_printf4(f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4))
# define PCU_PRINTF5(f, a1, a2, a3, a4, a5)                 PCU_printf5(f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5))
# define PCU_PRINTF6(f, a1, a2, a3, a4, a5, a6)             PCU_printf6(f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5), (size_t)(a6))
# define PCU_PRINTF7(f, a1, a2, a3, a4, a5, a6, a7)         PCU_printf7(f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5), (size_t)(a6), (size_t)(a7))
# define PCU_PRINTF8(f, a1, a2, a3, a4, a5, a6, a7, a8)     PCU_printf8(f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5), (size_t)(a6), (size_t)(a7), (size_t)(a8))
# define PCU_PRINTF9(f, a1, a2, a3, a4, a5, a6, a7, a8, a9) PCU_printf9(f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5), (size_t)(a6), (size_t)(a7), (size_t)(a8), (size_t)(a9))
int PCU_printf0(const char *format);
int PCU_printf1(const char *format, size_t arg1);
int PCU_printf2(const char *format, size_t arg1, size_t arg2);
int PCU_printf3(const char *format, size_t arg1, size_t arg2, size_t arg3);
int PCU_printf4(const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4);
int PCU_printf5(const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5);
int PCU_printf6(const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6);
int PCU_printf7(const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6, size_t arg7);
int PCU_printf8(const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6, size_t arg7, size_t arg8);
int PCU_printf9(const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6, size_t arg7, size_t arg8, size_t arg9);
int PCU_snprintf0(char *buf, size_t size, const char *format);
int PCU_snprintf1(char *buf, size_t size, const char *format, size_t arg1);
int PCU_snprintf2(char *buf, size_t size, const char *format, size_t arg1, size_t arg2);
int PCU_snprintf3(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3);
int PCU_snprintf4(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4);
int PCU_snprintf5(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5);
int PCU_snprintf6(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6);
int PCU_snprintf7(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6, size_t arg7);
int PCU_snprintf8(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6, size_t arg7, size_t arg8);
int PCU_snprintf9(char *buf, size_t size, const char *format, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6, size_t arg7, size_t arg8, size_t arg9);
#else
# define PCU_SNPRINTF0	PCU_snprintf
# define PCU_SNPRINTF1	PCU_snprintf
# define PCU_SNPRINTF2	PCU_snprintf
# define PCU_SNPRINTF3	PCU_snprintf
# define PCU_SNPRINTF4	PCU_snprintf
# define PCU_SNPRINTF5	PCU_snprintf
# define PCU_SNPRINTF6	PCU_snprintf
# define PCU_SNPRINTF7	PCU_snprintf
# define PCU_SNPRINTF8	PCU_snprintf
# define PCU_SNPRINTF9	PCU_snprintf
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
int PCU_snprintf(char *buf, size_t size, const char *format, ...);
int PCU_printf(const char *format, ...);
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


#ifdef PCU_NO_LIBC
# include <stddef.h> /* size_t */
# define PCU_STRLEN		PCU_strlen
# define PCU_STRCMP		PCU_strcmp
# define PCU_STRNCMP	PCU_strncmp
# define PCU_STRCPY		PCU_strcpy
# define PCU_STRNCPY	PCU_strncpy
# define PCU_MEMSET		PCU_memset
# define PCU_MEMCPY		PCU_memcpy
# define PCU_ATOI		PCU_atoi
size_t PCU_strlen(const char *s);
int PCU_strcmp(const char *s1, const char *s2);
int PCU_strncmp(const char *s1, const char *s2, size_t len);
char *PCU_strcpy(char *dst, const char *src);
char *PCU_strncpy(char *dst, const char *src, size_t len);
void *PCU_memset(void *b, int c, size_t len);
void *PCU_memcpy(void *dst, const void *src, size_t len);
long PCU_strtol(const char *s, char **endptr, int base);
unsigned long PCU_strtoul(const char *s, char **endptr, int base);
int PCU_atoi(const char *s);
#else
# include <string.h>
# include <stdlib.h>
# define PCU_STRLEN		strlen
# define PCU_STRCMP		strcmp
# define PCU_STRNCMP	strncmp
# define PCU_STRCPY		strcpy
# define PCU_STRNCPY	strncpy
# define PCU_MEMSET		memset
# define PCU_MEMCPY		memcpy
# define PCU_ATOI		atoi
#endif

#if !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
# include <wchar.h> /* wchar_t, mbstate_t */
# include <limits.h> /* MB_LEN_MAX */
# include <stdlib.h> /* MB_CUR_MAX */
# define PCU_WCSCMP		wcscmp
# define PCU_WCSNCMP	wcsncmp
# define PCU_WCSLEN		wcslen
# define PCU_WCRTOMB	wcrtomb
# define PCU_WCSRTOMBS	wcsrtombs
# define PCU_SWPRINTF	swprintf
#endif

#ifdef __cplusplus
}
#endif

#endif /* PCU_LIBC_H_INCLUDED */
