/* 
 * Data Driven Test
 */
#include "PCUnit.h"
#include "StrToken.h"

struct StrTokenTestData {
	const char *src;
	const char *delims;
	size_t expected_count;
	const char *expected_token[9];
};

static struct StrTokenTestData test_data[] = {
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
static struct StrTokenTestData *data;

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

static void StrToken_test(void)
{
	size_t i;

	/* _FATALの付いたASSERTが失敗すると強制的に関数から戻る */
	PCU_ASSERT_EQUAL_FATAL(data->expected_count, StrToken_count(tok));
	for (i = 0; i < data->expected_count; i++) {
		if (!PCU_ASSERT_STRING_EQUAL(data->expected_token[i], StrToken_get(tok, i))) {
			/* 失敗したら追加メッセージを表示する */
			PCU_MSG1("%dth token err", i);
		}
	}
}


PCU_Test StrToken_tests[] = {
	{ "StrToken_test", StrToken_test, setup, teardown, sizeof test_data / sizeof test_data[0] },
	PCU_NULL,
};

