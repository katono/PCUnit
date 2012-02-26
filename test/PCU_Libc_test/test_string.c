#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../PCUnit/PCUnit.h"
int PCU_strcmp(const char *s1, const char *s2);
int PCU_strncmp(const char *s1, const char *s2, size_t len);
void *PCU_memset(void *b, int c, size_t len);
void *PCU_memcpy(void *dst, const void *src, size_t len);


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

void test_memcpy(void)
{
	static char c_buf[256];
	static char c_buf2[256];
	static int i_buf[256];
	static int i_buf2[256];
	static struct {
		int a;
		char b[3];
	} st_buf[10], st_buf2[10];
	char *p;
	size_t i;

	srand(time(0));
	for (i = 0; i < sizeof c_buf; i++) {
		c_buf[i] = (char) rand();
	}
	for (i = 0; i < sizeof i_buf; i++) {
		((char *)i_buf)[i] = (char) rand();
	}
	for (i = 0; i < sizeof st_buf; i++) {
		((char *)st_buf)[i] = (char) rand();
	}

	p = PCU_memcpy(c_buf2, c_buf, sizeof(c_buf2));
	PCU_ASSERT_PTR_EQUAL(c_buf2, p);
	for (i = 0; i < sizeof(c_buf2); i++) {
		PCU_ASSERT_EQUAL(c_buf[i], p[i]);
	}

	p = PCU_memcpy(i_buf2, i_buf, sizeof(i_buf2));
	PCU_ASSERT_PTR_EQUAL(i_buf2, p);
	for (i = 0; i < sizeof(i_buf2); i++) {
		PCU_ASSERT_EQUAL(((char *)i_buf)[i], p[i]);
	}

	p = PCU_memcpy(st_buf2, st_buf, sizeof(st_buf2));
	PCU_ASSERT_PTR_EQUAL(st_buf2, p);
	for (i = 0; i < sizeof(st_buf2); i++) {
		PCU_ASSERT_EQUAL(((char *)st_buf)[i], p[i]);
	}
}

PCU_Suite *StringTest_suite(void)
{
	static PCU_Test tests[] = {
		{ "test_strcmp", test_strcmp },
		{ "test_strncmp", test_strncmp },
		{ "test_memset", test_memset },
		{ "test_memcpy", test_memcpy },
	};
	static PCU_Suite suite = { "StringTest", tests, sizeof tests / sizeof tests[0] };
	return &suite;
}
