# PCUnit - Portable C Unit Testing Framework

PCUnitはポータブルなC言語用ユニットテストフレームワークです。
標準Cライブラリが使えないフリースタンディングな環境でも、putchar相当の関数を用意すればターゲット上でテストすることができます。


## ビルド方法

* GNU開発環境の場合は、PCUnitディレクトリでmakeしてください。makeするとlibpcunit.aが生成されるのでこれをユニットテスト用プロジェクトに静的リンクしてください。デフォルトのコンパイラはgccですが、適宜Makefileを修正してターゲット用のクロスコンパイラやコンパイラオプションを指定してください。
* GNU開発環境でない場合は、PCUnitディレクトリ以下のソースファイルとヘッダファイルをユニットテスト用プロジェクトに追加してビルドしてください。

### 標準Cライブラリが使えない場合

* `PCU_NO_STDLIB`マクロをコンパイラオプションで定義してください。
* 標準C関数のputchar相当の関数を用意してください。
* `PCU_run_interactive`でテストをするならば、標準C関数のgetchar相当の関数を用意してください。
* もしstddef.hがなければ作成し、`size_t`型をtypedefしてください。
* mallocが使えないので、あらかじめ静的領域にメモリプールを確保しています。必要ならば以下のマクロの値をコンパイラオプションで定義してメモリプールのサイズを調整してください。
    * アサートマクロ失敗の最大数を`PCU_MAX_FAILURE_NUM`マクロの値で定義できます。定義しない場合のデフォルト値は64です。この最大数を超えてアサートマクロが失敗した場合はテスト結果が正しく表示されません。なお、内部データであるアサートマクロ失敗構造体のサイズは少なくとも32バイトあります。
    * 文字列表示用メモリプールのバイト数を`PCU_STRING_POOL_SIZE`マクロの値で定義できます。定義しない場合のデフォルト値は4096です。このメモリプールは、`PCU_ASSERT_STRING_*`、`PCU_ASSERT_NSTRING_*`、`PCU_FAIL*`、`PCU_MSG*`のマクロで使用されます。このメモリプールを使い切ってしまった場合はテスト結果が正しく表示されません。


## テスト構成
PCUnitのテストはこのような構成になっています。

* *テストスイート*
    * *テストケース 1*
        * *テスト 1-1*
        * *テスト 1-2*
        * ・・・
    * *テストケース 2*
        * *テスト 2-1*
        * *テスト 2-2*
        * ・・・
    * *テストケース 3*
        * *テスト 3-1*
        * *テスト 3-2*
        * ・・・
    * ・・・

*テスト*は`PCU_Test`という構造体で定義されます。
`PCU_Test`構造体は次のように型定義されています。

    typedef struct {
        const char *name;      /* テスト名 */
        void (*test)(void);    /* テスト関数 */
        int (*setup)(void);    /* testの実行前に呼ばれる関数。0ならば呼ばれない。 */
        int (*teardown)(void); /* testの実行後に呼ばれる関数。0ならば呼ばれない。 */
        int repeat;            /* setup,test,teardownを繰り返し実行する回数。1以下の値ならば1回だけ実行する。 */
        ...                    /* 以下、非公開メンバ */
    } PCU_Test;

`PCU_Test`のsetupとteardownの戻り値は、0ならば正常、非0ならば異常です。
setupが異常の場合、testとteardownは実行されません。


*テストケース*は`PCU_TestCase`という構造体で定義されます。
`PCU_TestCase`構造体は次のように型定義されています。

    typedef struct {
        const char *name;        /* テストケース名 */
        PCU_Test *tests;         /* PCU_Testの配列 */
        int (*initialize)(void); /* testsの最初のテストの実行前に呼ばれる関数。0ならば呼ばれない。 */
        int (*cleanup)(void);    /* testsの最後のテストの実行後に呼ばれる関数。0ならば呼ばれない。 */
        ...                      /* 以下、非公開メンバ */
    } PCU_TestCase;

`PCU_TestCase`のinitializeとcleanupの戻り値は、0ならば正常、非0ならば異常です。
initializeが異常の場合、testsとcleanupは実行されません。


*テストスイート*は`PCU_TestCase`の配列で定義されます。

テストの実行は、*テストスイート*を`PCU_run`関数または`PCU_run_interactive`関数に渡して行います。


## 簡単な使い方

簡単な例として、add関数とsub関数をテスト対象関数とします。

add関数は引数aと引数bを足した値を返し、sub関数は引数aから引数bを引いた値を返す関数です。
プロトタイプ宣言は次のとおりです。

    int add(int a, int b);
    int sub(int a, int b);

add関数とsub関数を実装する前に、テスト関数を先に実装します。
`PCUnit.h`をインクルードする必要があります。

    #include "PCUnit.h"
    void test_add(void)
    {
        PCU_ASSERT_EQUAL(3, add(2, 1));
        PCU_ASSERT_EQUAL(0, add(0, 0));
        PCU_ASSERT_EQUAL(1, add(-1, 2));
    }

    void test_sub(void)
    {
        PCU_ASSERT_EQUAL(1, sub(2, 1));
        PCU_ASSERT_EQUAL(0, sub(0, 0));
        PCU_ASSERT_EQUAL(-3, sub(-1, 2));
    }

次に、`PCU_Test`構造体の配列を静的領域に定義と同時に初期化します。constを付けてはいけません。
`PCU_Test`構造体の最初のメンバnameにはテスト名を、2番目のメンバtestにはテスト関数を指定します。
この例では3番目のメンバsetup以下のメンバは省略されているので0で初期化されます。
setup, teardown, repeatが必要な場合は省略しないで指定してください。
`PCU_Test`の配列は必ず`PCU_NULL`で終端してください。

    PCU_Test example_tests[] = {
        { "test_add", test_add },
        { "test_sub", test_sub },
        PCU_NULL,
    };

同じように、`PCU_TestCase`構造体の配列を静的領域に定義と同時に初期化します。constを付けてはいけません。
`PCU_TestCase`構造体の最初のメンバnameにはテストケース名を、2番目のメンバtestsには先に定義した`PCU_Test`の配列を指定します。
この例では3番目のメンバinitialize以下のメンバは省略されているので0で初期化されます。
initialize, cleanupが必要な場合は省略しないで指定してください。
`PCU_TestCase`の配列は必ず`PCU_NULL`で終端してください。

    PCU_TestCase suite[] = {
        { "example_tests", example_tests },
        PCU_NULL,
    };

main関数で、テスト実行関数である`PCU_run`関数または`PCU_run_interactive`関数を呼び出します。

    int main(void)
    {
        PCU_run(suite);
        return 0;
    }

ビルドするとadd関数とsub関数の実装がないため、リンクエラーになります。

ここで、add関数とsub関数の実装をします。
単純な関数ですが、わざとテストに失敗させるために0を返すようにしておきます。

    int add(int a, int b) { return 0; }
    int sub(int a, int b) { return 0; }

ビルドして実行すると、テストに失敗します。
どのテストケースのどのテストで失敗したか、ファイル名・行番号、マクロの引数の値などが表示されます。

    TestCase: example_tests
    
     !!!! Failure !!!!
     Test: test_add
      1) test.c:6
       PCU_ASSERT_EQUAL(3, add(2, 1))
        expected <3(0x3)>
        actual   <0(0x0)>
      2) test.c:8
       PCU_ASSERT_EQUAL(1, add(-1, 2))
        expected <1(0x1)>
        actual   <0(0x0)>
    
     !!!! Failure !!!!
     Test: test_sub
      1) test.c:13
       PCU_ASSERT_EQUAL(1, sub(2, 1))
        expected <1(0x1)>
        actual   <0(0x0)>
      2) test.c:15
       PCU_ASSERT_EQUAL(-3, sub(-1, 2))
        expected <-3(0xfffffffd)>
        actual   <0(0x0)>
    
    2 Tests, 2 Failures

add関数とsub関数を修正します。

    - int add(int a, int b) { return 0; }
    - int sub(int a, int b) { return 0; }
    + int add(int a, int b) { return a + b; }
    + int sub(int a, int b) { return a - b; }


ビルドして実行します。
全てのテストにパスすると、OKのメッセージが表示されます。

    TestCase: example_tests
    
    OK (2 Tests)

OKのメッセージは1つのテストケースにつき1つ表示されます。
この例ではテストケースは1つでしたが、テストケースが複数の場合、すべてのテストケースにOKが表示されるまで修正を繰り返してください。



## アサートマクロ
アサートマクロはテスト関数内で使用可能です。マクロの引数は副作用のないように指定してください。
アサートマクロの戻り値は、失敗時に`PCU_MSG*`マクロで追加メッセージを表示させたい場合などに利用できます。

* **`PCU_ASSERT(expr)`**

    exprが真かどうかチェックします。偽ならば失敗を登録し、偽を返します。真ならば真を返します。


* **`PCU_ASSERT_FATAL(expr)`**

    exprが真かどうかチェックします。偽ならば失敗を登録し、テスト関数からreturnします。このマクロは値を返しません。


* **`PCU_ASSERT_TRUE(expr)`**

    exprが真かどうかチェックします。偽ならば失敗を登録し、偽を返します。真ならば真を返します。


* **`PCU_ASSERT_TRUE_FATAL(expr)`**

    exprが真かどうかチェックします。偽ならば失敗を登録し、テスト関数からreturnします。このマクロは値を返しません。


* **`PCU_ASSERT_FALSE(expr)`**

    exprが偽かどうかチェックします。真ならば失敗を登録し、偽を返します。偽ならば真を返します。


* **`PCU_ASSERT_FALSE_FATAL(expr)`**

    exprが偽かどうかチェックします。真ならば失敗を登録し、テスト関数からreturnします。このマクロは値を返しません。


* **`PCU_ASSERT_EQUAL(expected, actual)`**

    expectedとactualが等しいかどうかチェックします。等しくないならば失敗を登録し、偽を返します。等しいならば真を返します。


* **`PCU_ASSERT_EQUAL_FATAL(expected, actual)`**

    expectedとactualが等しいかどうかチェックします。等しくないならば失敗を登録し、テスト関数からreturnします。このマクロは値を返しません。


* **`PCU_ASSERT_NOT_EQUAL(expected, actual)`**

    expectedとactualが等しくないかどうかチェックします。等しいならば失敗を登録し、偽を返します。等しくないならば真を返します。


* **`PCU_ASSERT_NOT_EQUAL_FATAL(expected, actual)`**

    expectedとactualが等しくないかどうかチェックします。等しいならば失敗を登録し、テスト関数からreturnします。このマクロは値を返しません。


* **`PCU_ASSERT_PTR_EQUAL(expected, actual)`**

    expectedとactualのポインタの値が等しいかどうかチェックします。等しくないならば失敗を登録し、偽を返します。等しいならば真を返します。


* **`PCU_ASSERT_PTR_EQUAL_FATAL(expected, actual)`**

    expectedとactualのポインタの値が等しいかどうかチェックします。等しくないならば失敗を登録し、テスト関数からreturnします。このマクロは値を返しません。


* **`PCU_ASSERT_PTR_NOT_EQUAL(expected, actual)`**

    expectedとactualのポインタの値が等しくないかどうかチェックします。等しいならば失敗を登録し、偽を返します。等しくないならば真を返します。


* **`PCU_ASSERT_PTR_NOT_EQUAL_FATAL(expected, actual)`**

    expectedとactualのポインタの値が等しくないかどうかチェックします。等しいならば失敗を登録し、テスト関数からreturnします。このマクロは値を返しません。


* **`PCU_ASSERT_STRING_EQUAL(expected, actual)`**

    expectedとactualの文字列が等しいかどうかチェックします。等しくないならば失敗を登録し、偽を返します。等しいならば真を返します。


* **`PCU_ASSERT_STRING_EQUAL_FATAL(expected, actual)`**

    expectedとactualの文字列が等しいかどうかチェックします。等しくないならば失敗を登録し、テスト関数からreturnします。このマクロは値を返しません。


* **`PCU_ASSERT_STRING_NOT_EQUAL(expected, actual)`**

    expectedとactualの文字列が等しくないかどうかチェックします。等しいならば失敗を登録し、偽を返します。等しくないならば真を返します。


* **`PCU_ASSERT_STRING_NOT_EQUAL_FATAL(expected, actual)`**

    expectedとactualの文字列が等しくないかどうかチェックします。等しいならば失敗を登録し、テスト関数からreturnします。このマクロは値を返しません。


* **`PCU_ASSERT_NSTRING_EQUAL(expected, actual, n)`**

    expectedとactualの文字列の先頭からn文字が等しいかどうかチェックします。等しくないならば失敗を登録し、偽を返します。等しいならば真を返します。


* **`PCU_ASSERT_NSTRING_EQUAL_FATAL(expected, actual, n)`**

    expectedとactualの文字列の先頭からn文字が等しいかどうかチェックします。等しくないならば失敗を登録し、テスト関数からreturnします。このマクロは値を返しません。


* **`PCU_ASSERT_NSTRING_NOT_EQUAL(expected, actual, n)`**

    expectedとactualの文字列の先頭からn文字が等しくないかどうかチェックします。等しいならば失敗を登録し、偽を返します。等しくないならば真を返します。


* **`PCU_ASSERT_NSTRING_NOT_EQUAL_FATAL(expected, actual, n)`**

    expectedとactualの文字列の先頭からn文字が等しくないかどうかチェックします。等しいならば失敗を登録し、テスト関数からreturnします。このマクロは値を返しません。


* **`PCU_ASSERT_DOUBLE_EQUAL(expected, actual, delta)`**

    |expected - actual| <= |delta|が真かどうかチェックします。偽ならば失敗を登録し、偽を返します。真ならば真を返します。


* **`PCU_ASSERT_DOUBLE_EQUAL_FATAL(expected, actual, delta)`**

    |expected - actual| <= |delta|が真かどうかチェックします。偽ならば失敗を登録し、テスト関数からreturnします。このマクロは値を返しません。


* **`PCU_ASSERT_DOUBLE_NOT_EQUAL(expected, actual, delta)`**

    |expected - actual| <= |delta|が偽かどうかチェックします。真ならば失敗を登録し、偽を返します。偽ならば真を返します。


* **`PCU_ASSERT_DOUBLE_NOT_EQUAL_FATAL(expected, actual, delta)`**

    |expected - actual| <= |delta|が偽かどうかチェックします。真ならば失敗を登録し、テスト関数からreturnします。このマクロは値を返しません。


`PCU_FAIL*`マクロは無条件に失敗を登録します。表示メッセージをprintfと同じ形式でformatとarg[1-9]に指定します。arg[1-9]の個数によって使用するマクロを選択してください。これらのマクロは値を返しません。`PCU_FAIL*_FATAL`はテスト関数からreturnします。`PCU_FAIL`と`PCU_FAIL_FATAL`はマクロの可変長引数が使えるC99`(__STDC_VERSION__ >= 199901L)`の場合に使用できます。

* **`PCU_FAIL0(format)`**
* **`PCU_FAIL1(format, arg1)`**
* **`PCU_FAIL2(format, arg1, arg2)`**
* **`PCU_FAIL3(format, arg1, arg2, arg3)`**
* **`PCU_FAIL4(format, arg1, arg2, arg3, arg4)`**
* **`PCU_FAIL5(format, arg1, arg2, arg3, arg4, arg5)`**
* **`PCU_FAIL6(format, arg1, arg2, arg3, arg4, arg5, arg6)`**
* **`PCU_FAIL7(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7)`**
* **`PCU_FAIL8(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)`**
* **`PCU_FAIL9(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)`**
* **`PCU_FAIL(format, ...)`**
* **`PCU_FAIL0_FATAL(format)`**
* **`PCU_FAIL1_FATAL(format, arg1)`**
* **`PCU_FAIL2_FATAL(format, arg1, arg2)`**
* **`PCU_FAIL3_FATAL(format, arg1, arg2, arg3)`**
* **`PCU_FAIL4_FATAL(format, arg1, arg2, arg3, arg4)`**
* **`PCU_FAIL5_FATAL(format, arg1, arg2, arg3, arg4, arg5)`**
* **`PCU_FAIL6_FATAL(format, arg1, arg2, arg3, arg4, arg5, arg6)`**
* **`PCU_FAIL7_FATAL(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7)`**
* **`PCU_FAIL8_FATAL(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)`**
* **`PCU_FAIL9_FATAL(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)`**
* **`PCU_FAIL_FATAL(format, ...)`**


`PCU_MSG*`マクロは合否チェックを行わず、表示メッセージのみを登録します。表示メッセージをprintfと同じ形式でformatとarg[1-9]に指定します。arg[1-9]の個数によって使用するマクロを選択してください。これらのマクロは値を返しません。`PCU_MSG`はマクロの可変長引数が使えるC99の場合`(__STDC_VERSION__ >= 199901L)`に使用できます。

* **`PCU_MSG0(format)`**
* **`PCU_MSG1(format, arg1)`**
* **`PCU_MSG2(format, arg1, arg2)`**
* **`PCU_MSG3(format, arg1, arg2, arg3)`**
* **`PCU_MSG4(format, arg1, arg2, arg3, arg4)`**
* **`PCU_MSG5(format, arg1, arg2, arg3, arg4, arg5)`**
* **`PCU_MSG6(format, arg1, arg2, arg3, arg4, arg5, arg6)`**
* **`PCU_MSG7(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7)`**
* **`PCU_MSG8(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)`**
* **`PCU_MSG9(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)`**
* **`PCU_MSG(format, ...)`**



## API

* **`void PCU_run(PCU_TestCase *suite)`**

    suiteに含まれる全テストを実行します。suiteには`PCU_NULL`で終端した`PCU_TestCase`の配列を指定します。


* **`void PCU_run_interactive(PCU_TestCase *suite)`**

    インタラクティブモードでテストを実行します。対話的に実行するテストを選択することができます。suiteには`PCU_NULL`で終端した`PCU_TestCase`の配列を指定します。


* **`const char *PCU_test_name(void)`**

    現在実行中のテスト名を返します。


* **`const char *PCU_case_name(void)`**

    現在実行中のテストケース名を返します。


* **`int PCU_repeat_counter(void)`**

    `PCU_Test`のrepeatメンバに2以上の値を指定した場合、現在何回目(0オリジン)のテスト実行であるかを返します。


* **`void PCU_enable_color(void)`**

    テスト結果のカラー表示を有効にします。


* **`void PCU_disable_color(void)`**

    テスト結果のカラー表示を無効にします。


次の2つの関数は`PCU_NO_STDLIB`マクロが定義されていない場合は何もしません。

* **`void PCU_set_putchar(PCU_Putchar func)`**

    標準C関数のputcharに相当する関数を設定します。`PCU_Putchar`型はputcharと同じ`int(*)(int)`型のtypedefです。


* **`void PCU_set_getchar(PCU_Getchar func)`**

    標準C関数のgetcharに相当する関数を設定します。`PCU_run_interactive`でテストをする場合に必要となります。`PCU_Getchar`型はgetcharと同じ`int(*)(void)`型のtypedefです。



<!-- vim:set ts=8 sts=4 sw=4 et ft=mkd: -->
