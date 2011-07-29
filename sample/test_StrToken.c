/* 
 * Data Driven Test
 */
#include "PCUnit.h"
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

	/* _RETURNまたは_FATALの付いたASSERTが失敗すると強制的に関数から戻る */
	PCU_ASSERT_EQUAL_RETURN(data->expected_count, StrToken_count(tok));
	for (i = 0; i < data->expected_count; i++) {
		if (!PCU_ASSERT_STRING_EQUAL(data->expected_token[i], StrToken_get(tok, i))) {
			/* 失敗したら追加メッセージを表示する */
			PCU_MESSAGE1("%dth token err", i);
		}
	}
}


PCU_Suite *StrTokenTest_suite(void)
{
	static PCU_Test tests[] = {
		{ "test_StrToken", test_StrToken, sizeof test_data / sizeof test_data[0] },
	};
	static PCU_Suite suite = { "StrTokenTest", tests, sizeof tests / sizeof tests[0], setup, teardown };
	return &suite;
}

