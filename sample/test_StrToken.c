/* 
 * Data Driven Test
 */
#include "PCUnit/PCUnit.h"
#include "StrToken.h"

typedef struct {
	const char *src;
	const char *delims;
	size_t expected_count;
	const char *expected_token[9];
} StrTokenTestData;

static const StrTokenTestData test_data[] = {
     { "abc;def;gh;ij"     , ";"  , 4 , { "abc", "def", "gh", "ij"}},
     { "abc;def;gh;ij;"    , ";"  , 4 , { "abc", "def", "gh", "ij"}},
     { ";abc;def;gh;ij"    , ";"  , 4 , { "abc", "def", "gh", "ij"}},
     { "abc,def;,;gh"      , ",;" , 3 , { "abc", "def", "gh"      }},
     { ",abc,def;gh,"      , ";"  , 2 , { ",abc,def", "gh,"       }},
     { ",,abc,def,gh"      , ","  , 3 , { "abc", "def", "gh"      }},
     { ",,,abc,def,gh,,i," , ","  , 4 , { "abc", "def", "gh", "i" }},
     { ""                  , ","  , 0 , { ""                      }},
     { "abc"               , ","  , 1 , { "abc"                   }},
     { "abc"               , ""   , 1 , { "abc"                   }},
     { ""                  , ""   , 0 , { ""                      }},
     { ",,,;,,;,,;,;"      , ",;" , 0 , { ""                      }},
};

static StrToken *tok;
static const StrTokenTestData *data;

static int setup(void)
{
	data = &test_data[PCU_repeat_counter()];

	tok = StrToken_new(data->src, data->delims);
	return tok == 0;
}

static int teardown(void)
{
	StrToken_delete(tok);
	tok = 0;
	return 0;
}

static void test_StrToken(void)
{
	size_t i;

	PCU_ASSERT_EQUAL(data->expected_count, StrToken_count(tok));
	for (i = 0; i < data->expected_count; i++) {
		PCU_ASSERT_STRING_EQUAL_MESSAGE(data->expected_token[i], StrToken_get(tok, i), PCU_format("%dth token err", i));
	}
}


PCU_Suite *StrTokenTest_suite(void)
{
	static PCU_Test tests[] = {
		PCU_TEST_REPEATED(test_StrToken, sizeof test_data / sizeof test_data[0]),
	};
	static PCU_Suite suite = { "StrTokenTest", tests, sizeof tests / sizeof tests[0], setup, teardown };
	return &suite;
}

