#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef PCU_NO_STDLIB
#include "../PCUnit/include/PCUnit.h"
size_t PCU_strlen(const char *s);
int PCU_strcmp(const char *s1, const char *s2);
int PCU_strncmp(const char *s1, const char *s2, size_t len);
char *PCU_strcpy(char *dst, const char *src);
char *PCU_strncpy(char *dst, const char *src, size_t len);
void *PCU_memset(void *b, int c, size_t len);


void test_strlen(void)
{
	const char *s;

	s = "hoge";
	PCU_ASSERT_EQUAL(strlen(s), PCU_strlen(s));

	s = "";
	PCU_ASSERT_EQUAL(strlen(s), PCU_strlen(s));
}

void test_strcmp(void)
{
	const char *s1;
	const char *s2;
#define ASSERT_STRCMP(s1, s2)	\
	do { int tmp = strcmp(s1, s2);\
		if (tmp < 0) tmp = -1; else if (tmp > 0) tmp = 1;\
		PCU_ASSERT_EQUAL(tmp, PCU_strcmp(s1, s2)); } while (0)

	s1 = "hoge";
	s2 = "hoge";
	ASSERT_STRCMP(s1, s2);

	s1 = "hoge";
	s2 = "piyo";
	ASSERT_STRCMP(s1, s2);

	s1 = "hoge";
	s2 = "hog";
	ASSERT_STRCMP(s1, s2);

	s1 = "hog";
	s2 = "hoge";
	ASSERT_STRCMP(s1, s2);

	s1 = "";
	s2 = "";
	ASSERT_STRCMP(s1, s2);

	s1 = "hoge";
	s2 = "";
	ASSERT_STRCMP(s1, s2);

	s1 = "";
	s2 = "hoge";
	ASSERT_STRCMP(s1, s2);
}

void test_strncmp(void)
{
	const char *s1;
	const char *s2;
	size_t i;
#define ASSERT_STRNCMP(s1, s2, n)	\
	do { int tmp = strncmp(s1, s2, n);\
		if (tmp < 0) tmp = -1; else if (tmp > 0) tmp = 1;\
		PCU_ASSERT_EQUAL(tmp, PCU_strncmp(s1, s2, n)); } while (0)

	for (i = 0; i < 6; i++) {
		s1 = "hoge";
		s2 = "hoge";
		ASSERT_STRNCMP(s1, s2, i);

		s1 = "hoge";
		s2 = "piyo";
		ASSERT_STRNCMP(s1, s2, i);

		s1 = "hoge";
		s2 = "hog";
		ASSERT_STRNCMP(s1, s2, i);

		s1 = "hog";
		s2 = "hoge";
		ASSERT_STRNCMP(s1, s2, i);

		s1 = "";
		s2 = "";
		ASSERT_STRNCMP(s1, s2, i);

		s1 = "hoge";
		s2 = "";
		ASSERT_STRNCMP(s1, s2, i);

		s1 = "";
		s2 = "hoge";
		ASSERT_STRNCMP(s1, s2, i);
	}
}

void test_strcpy(void)
{
	char std_buf[32];
	char pcu_buf[32];
	const char *std_p;
	const char *pcu_p;
	const char *s;

	s = "hoge";
	memset(std_buf, 0xFF, sizeof(std_buf));
	memset(pcu_buf, 0xFF, sizeof(pcu_buf));
	std_p = strcpy(std_buf, s);
	pcu_p = PCU_strcpy(pcu_buf, s);
	PCU_ASSERT_STRING_EQUAL(std_p, pcu_p);
	PCU_ASSERT_STRING_EQUAL(std_buf, pcu_buf);
	PCU_ASSERT_PTR_EQUAL(pcu_buf, pcu_p);

	s = "";
	memset(std_buf, 0xFF, sizeof(std_buf));
	memset(pcu_buf, 0xFF, sizeof(pcu_buf));
	std_p = strcpy(std_buf, s);
	pcu_p = PCU_strcpy(pcu_buf, s);
	PCU_ASSERT_STRING_EQUAL(std_p, pcu_p);
	PCU_ASSERT_STRING_EQUAL(std_buf, pcu_buf);
	PCU_ASSERT_PTR_EQUAL(pcu_buf, pcu_p);
}

void test_strncpy(void)
{
	size_t i;
	char std_buf[32];
	char pcu_buf[32];
	const char *std_p;
	const char *pcu_p;
	const char *s;

	for (i = 0; i < 10; i++) {
		s = "hoge";
		memset(std_buf, 0xFF, sizeof(std_buf));
		memset(pcu_buf, 0xFF, sizeof(pcu_buf));
		std_p = strncpy(std_buf, s, i);
		pcu_p = PCU_strncpy(pcu_buf, s, i);
		PCU_ASSERT_NSTRING_EQUAL(std_p, pcu_p, i);
		PCU_ASSERT_NSTRING_EQUAL(std_buf, pcu_buf, i);
		PCU_ASSERT_PTR_EQUAL(pcu_buf, pcu_p);

		s = "";
		memset(std_buf, 0xFF, sizeof(std_buf));
		memset(pcu_buf, 0xFF, sizeof(pcu_buf));
		std_p = strncpy(std_buf, s, i);
		pcu_p = PCU_strncpy(pcu_buf, s, i);
		PCU_ASSERT_NSTRING_EQUAL(std_p, pcu_p, i);
		PCU_ASSERT_NSTRING_EQUAL(std_buf, pcu_buf, i);
		PCU_ASSERT_PTR_EQUAL(pcu_buf, pcu_p);
	}
}

void test_memset(void)
{
	static char c_buf[256];
	static int i_buf[256];
	static struct {
		int a;
		char b[3];
	} st_buf[10];
	unsigned char *p;
	size_t i;

	memset(c_buf, 0, sizeof(c_buf));
	memset(i_buf, 0xFF, sizeof(i_buf));
	memset(st_buf, 1, sizeof(st_buf));

	p = PCU_memset(c_buf, 0xFF, sizeof(c_buf));
	PCU_ASSERT_PTR_EQUAL(c_buf, p);
	for (i = 0; i < sizeof(c_buf); i++) {
		PCU_ASSERT_EQUAL(0xFF, p[i]);
	}

	p = PCU_memset(i_buf, 0, sizeof(i_buf));
	PCU_ASSERT_PTR_EQUAL(i_buf, p);
	for (i = 0; i < sizeof(i_buf); i++) {
		PCU_ASSERT_EQUAL(0, p[i]);
	}

	p = PCU_memset(st_buf, 128, sizeof(st_buf));
	PCU_ASSERT_PTR_EQUAL(st_buf, p);
	for (i = 0; i < sizeof(st_buf); i++) {
		PCU_ASSERT_EQUAL(128, p[i]);
	}

}




PCU_Test string_tests[] = {
	{ "test_strlen", test_strlen },
	{ "test_strcmp", test_strcmp },
	{ "test_strncmp", test_strncmp },
	{ "test_strcpy", test_strcpy },
	{ "test_strncpy", test_strncpy },
	{ "test_memset", test_memset },
	PCU_NULL,
};

