<title>PCUnit - Portable C Unit Testing Framework</title>

# PCUnit - Portable C Unit Testing Framework

## 概要

PCUnitは組み込み開発でテスト駆動開発(Test-Driven Development; TDD)や単体テストをするための
ポータブルなC言語/C++言語用ユニットテストフレームワークです。

PCUnitは以下のような特長があります。

* 機能

    * [CUnit](http://cunit.sourceforge.net/)ライクなアサートマクロをサポートします。
    * アサートマクロで失敗した内容を表示できます。さらにprintf形式で追加メッセージを表示できます。
    * テストフィクスチャ(setup, teardown, initialize, cleanup)をサポートします。
    * 1つのテストを繰り返し行うRepeatedTestをサポートします。これを利用して、データ駆動テストを行うこともできます。
    * ディスプレイやキーボードがないターゲット上でテストするために、ユーザー定義の入出力関数を設定できます。
    * 実行するテストをインタラクティブに選択できるコンソールモードをサポートします。
    * ターミナルが対応していればテスト結果のカラー表示ができます。
    * ユーティリティを使用すればテスト関数の雛形生成や自動登録ができます。

* 移植性

    * プラットフォーム非依存な実装なので、大抵のPC向けまたは組み込み向けのC/C++コンパイラでビルドすることができます。
      つまり、ホスト環境とターゲット環境の両方で動作する共通のテストコードを実装することができます。
    * たとえ標準Cライブラリが使えない環境でも使用可能です。


## リポジトリ

* <https://github.com/katono/PCUnit>
* <https://bitbucket.org/katono/pcunit>


## ビルド方法

GNU開発環境の場合は、PCUnitディレクトリでmakeを実行してください。makeを実行するとlibpcunit.aが生成されるので
それをテストプロジェクトに静的リンクしてください。デフォルトのコンパイラはgccですが、
適宜Makefileを修正してターゲット用のクロスコンパイラやコンパイラオプションを指定してください。
ホスト環境にインストールしたい場合は、make installを実行してください。
make installを実行すると/usr/local/libにlibpcunit.aを、/usr/local/include/PCUnitにヘッダファイルを、
/usr/local/binにユーティリティをインストールします。
make uninstallを実行するとアンインストールします。
インストール先ディレクトリを/usr/localから変更してインストールする場合は、
make install INSTALLDIR=/foo/barのようにINSTALLDIRに任意のインストール先ディレクトリを指定してください。
同様にアンインストールする場合は、make uninstall INSTALLDIR=/foo/barを実行してください。

GNU開発環境でない場合は、PCUnitディレクトリ以下のソースファイルとヘッダファイルをテストプロジェクトに追加して
テストコードと共にビルドしてください。あるいはPCUnitの静的ライブラリをビルドするプロジェクトを作成し、
ビルドしたライブラリをテストプロジェクトに静的リンクしてください。

### ビルド設定

PCUnitはコンパイラオプションのマクロ定義で以下の設定をすることができます。
PCUnitのビルドが失敗する場合は、適宜`PCU_NO_*`マクロを定義することでビルド可能になります。
ただし、PCUnitをビルドするためには最低限`<stddef.h>`と`<limits.h>`が必要です。

* vsprintfが使用できないためにビルドが失敗する場合は、`PCU_NO_VSPRINTF`マクロを定義してください。
* setjmp/longjmpが使用できないためにビルドが失敗する場合は、`PCU_NO_SETJMP`マクロを定義してください。
* `<string.h>`と`<stdlib.h>`の関数が使用できないためにビルドが失敗する場合は、
  `PCU_NO_LIBC`マクロを定義してください。
  このマクロを定義した場合は`PCU_NO_VSPRINTF`、`PCU_NO_SETJMP`を定義する必要はありません。
* `<stdarg.h>`がないためにビルドが失敗する場合は、`PCU_NO_STDARG`マクロを定義してください。
* プロセッサにFPUがなく、ソフトウェア浮動小数点ライブラリも使用できないためにビルドが失敗する場合は、
  `PCU_NO_FLOATINGPOINT`マクロを定義してください。
* 32ビットの除算ができないためにビルドが失敗する(例えばgccで`___divsi3`などのリンクエラーが発生する)場合は、
  `PCU_NO_DIV32`マクロを定義してください。
* コンソールモードが不要な場合は、`PCU_NO_CONSOLE_RUN`マクロを定義してください。
  `PCU_console_run`が使用できなくなりますが、コードサイズが少し小さくなります。
* ワイド文字列をチェックするアサートマクロを使用したい場合は、`PCU_USE_WCHAR`マクロを定義してください。
  `_UNICODE`マクロまたは`UNICODE`マクロが定義済みならば、`PCU_USE_WCHAR`マクロは自動的に定義済みになります。
* `PCU_format`/`PCU_formatW`で使用する静的領域のサイズを変更したい場合は、
  `PCU_FORMAT_BUFSIZE`マクロの値を定義してください。


## テスト構成
PCUnitのテストはこのような構成になっています。

* テスト実行関数
    * スイートメソッド 1
        * テストスイート 1
            * テストケース 1-1
            * テストケース 1-2
            * ・・・
    * スイートメソッド 2
        * テストスイート 2
            * テストケース 2-1
            * テストケース 2-2
            * ・・・
    * スイートメソッド 3
        * テストスイート 3
            * テストケース 3-1
            * テストケース 3-2
            * ・・・
    * ・・・

### テストケース

*テストケース* は1つのテスト関数を実行するためのデータ構造です。
テストケースは`PCU_Test`という構造体で次のように型定義されています。

    typedef struct {
        const char *name;      /* テストケース名 */
        void (*test)(void);    /* テスト関数 */
        int ntimes;            /* テスト関数を実行する回数 */
        ...                    /* 以下、非公開メンバ */
    } PCU_Test;

ntimesによる繰り返しはsetup, teardownの実行も含みます。
ntimesが0の場合は1を指定したと見なします。また、負の数ならばそのテストケースはスキップされます。


### テストスイート

*テストスイート* は複数のテストケースを管理するためのデータ構造です。
テストスイートは`PCU_Suite`という構造体で次のように型定義されています。

    typedef struct {
        const char *name;        /* テストスイート名 */
        PCU_Test *tests;         /* テストケースの配列 */
        int ntests;              /* testsの要素数 */
        int (*setup)(void);      /* testsの各テスト関数の実行前に呼ばれる関数 */
        int (*teardown)(void);   /* testsの各テスト関数の実行後に呼ばれる関数 */
        int (*initialize)(void); /* testsの最初のテストの実行前に呼ばれる関数 */
        int (*cleanup)(void);    /* testsの最後のテストの実行後に呼ばれる関数 */
        ...                      /* 以下、非公開メンバ */
    } PCU_Suite;

setupとteardownの戻り値は、0ならば正常、非0ならば異常です。
setupが異常の場合、テスト関数とteardownは実行されません。

initializeとcleanupの戻り値は、0ならば正常、非0ならば異常です。
initializeが異常の場合、testsの全てのテストとcleanupは実行されません。

setup, teardown, initialize, cleanupは、0が指定された場合は呼ばれません。


### スイートメソッド

*スイートメソッド* はテスト実行関数からテストスイートにアクセスするためのインターフェイスです。
スイートメソッドの詳細については後述します。


## 簡単な使い方

### テスト対象関数

簡単な例として、add関数とsub関数をテスト対象関数とします。

add関数は引数aと引数bを足した値を返し、sub関数は引数aから引数bを引いた値を返す関数です。
プロトタイプ宣言をしたヘッダファイルAddSub.hを作成します。

    [AddSub.h]
     1 #ifndef ADDSUB_H_INCLUDED
     2 #define ADDSUB_H_INCLUDED
     3 int add(int a, int b);
     4 int sub(int a, int b);
     5 #endif

### テスト関数

add関数とsub関数を実装する前に、テスト関数を先に実装します。
テスト関数を実装するソースファイル名は任意ですが、テスト対象のソースファイル名にTestを付けた名前にするとよいでしょう。
次のように実装します。

    [AddSubTest.c]
     1 #include "PCUnit/PCUnit.h"
     2 #include "AddSub.h"
     3
     4 static void test_add(void)
     5 {
     6     PCU_ASSERT_EQUAL(3, add(2, 1));
     7     PCU_ASSERT_EQUAL(0, add(0, 0));
     8     PCU_ASSERT_EQUAL(1, add(-1, 2));
     9 }
    10
    11 static void test_sub(void)
    12 {
    13     PCU_ASSERT_EQUAL(1, sub(2, 1));
    14     PCU_ASSERT_EQUAL(0, sub(0, 0));
    15     PCU_ASSERT_EQUAL(-3, sub(-1, 2));
    16 }
    17

PCUnitを使うためには`PCUnit.h`をインクルードする必要があります。
`PCUnit.h`のインクルードパスは適宜テストプロジェクトで指定してください。

テスト関数は引数・戻り値が共にvoidの関数です。テスト関数名は任意ですが、testを付けた名前にするとよいでしょう。
他のソースファイルから呼び出さないならばstatic関数にしてください。

ここでは`PCU_ASSERT_EQUAL`マクロを使用してテスト対象関数の戻り値が仕様どおりかチェックしています。
`PCU_ASSERT_EQUAL`マクロの2つの引数が等しければOKで、異なっていたらテスト失敗です。
最初の引数に予測される値を指定し、2番目の引数に実際の値を指定します。


### スイートメソッド

次に、スイートメソッドを定義します。
スイートメソッドは *static* な`PCU_Suite`の変数へのポインタを返すグローバル関数です。
スイートメソッドは`PCU_Suite`のメンバのtestsやsetup関数等のスコープを小さくする役割を果たします。
関数名は任意ですが、`テストスイート名_suite()`と付けるとよいでしょう。
次のように定義します。

    [AddSubTest.c]
    18 PCU_Suite *AddSubTest_suite(void)
    19 {
    20     static PCU_Test tests[] = {
    21         { "test_add", test_add },
    22         { "test_sub", test_sub },
    23     };
    24     static PCU_Suite suite = { "AddSubTest", tests, sizeof tests / sizeof tests[0] };
    25     return &suite;
    26 }

スイートメソッドは引数がvoid、戻り値が`PCU_Suite *`の関数です。

スイートメソッドではまず、`PCU_Test`構造体の配列testsを *static* で定義と同時に初期化します。
`PCU_Test`構造体の最初のメンバnameには任意のテストケース名を指定します。
2番目のメンバtestには先に定義したテスト関数を指定します。
3番目のメンバはntimesですが、この例では省略されているので0で初期化され、テスト関数は1回実行されます。
繰り返しが必要な場合はntimesに回数を指定してください。
後で新しいテストケースを追加したい場合は、新しいテスト関数を定義してこの配列の初期化に追加してください。
(`pcunit_register.rb`ユーティリティを使用すればテストケース登録を自動化できます。)

次に、`PCU_Suite`構造体の変数suiteを *static* で定義と同時に初期化します。
`PCU_Suite`構造体の最初のメンバnameには任意のテストスイート名を指定します。
2番目のメンバtestsには`PCU_Test`の配列testsを指定します。
3番目のメンバntestsにはtestsの要素数を指定します。
4番目以下のメンバsetup, teardown, initialize, cleanupはこの例では省略されているのでそれぞれ0で初期化されます。
setup, teardown, initialize, cleanupが必要な場合はそれぞれの関数を定義し指定してください。

この関数の最後でsuiteのアドレスを返します。


### main関数

main関数は次のように定義します。

    [main.c]
     1 #include "PCUnit/PCUnit.h"
     2 #include <stdio.h> /* for putchar */
     3
     4 PCU_Suite *AddSubTest_suite(void);
     5
     6 int main(void)
     7 {
     8     const PCU_SuiteMethod suites[] = {
     9         AddSubTest_suite,
    10     };
    11     PCU_set_putchar(putchar);
    12     PCU_run(suites, sizeof suites / sizeof suites[0]);
    13     return 0;
    14 }

main関数がテストを実装したソースと別ファイルの場合はスイートメソッドのプロトタイプ宣言をしてください。

main関数ではまず、`PCU_SuiteMethod`型の配列を宣言します。
`PCU_SuiteMethod`型はスイートメソッドの関数ポインタのtypedefです。次のように定義されています。

    typedef PCU_Suite *(*PCU_SuiteMethod)(void);

ここではAddSubTest_suiteだけで初期化しています。
後で新しいテストスイートを追加したい場合は、新しいテストスイートに対応するスイートメソッドをこの配列の初期化に追加してください。
(`pcunit_register.rb`ユーティリティを使用すればスイートメソッド登録を自動化できます。)

次に、`PCU_set_putchar`関数でテスト結果の出力関数をputcharに設定しています。
この関数ではユーザー定義のputchar相当の関数を用意することでテスト結果の出力先を変更することができます。
例えばターゲット上で動作させる時はシリアル出力関数などを指定するとよいでしょう。

最後に、テスト実行関数である`PCU_run`関数を呼び出します。
`PCU_run`の最初の引数は`PCU_SuiteMethod`型の配列で、2番目の引数はその配列の要素数です。

これでテストコードの準備は整いました。


### ビルド、実行、修正の繰り返し

ビルドするとadd関数とsub関数の実装がないため、リンクエラーになります。

ここで、AddSub.cを作成し、add関数とsub関数の実装をします。

    [AddSub.c]
     1 #include "AddSub.h"
     2 int add(int a, int b) { return 0; }
     3 int sub(int a, int b) { return 0; }

単純な関数ですが、わざとテストに失敗させるために0を返すようにしておきます。

再度ビルドして実行すると、テストに失敗します。
このように出力されます。

    Suite: AddSubTest
    
    Test: test_add
     AddSubTest.c:6: PCU_ASSERT_EQUAL(3, add(2, 1))
      expected : 0x00000003 (3)
      actual   : 0x00000000 (0)
    
    Test: test_sub
     AddSubTest.c:13: PCU_ASSERT_EQUAL(1, sub(2, 1))
      expected : 0x00000001 (1)
      actual   : 0x00000000 (0)
    
    2 Tests, 2 Failures, 0 Skipped

失敗したテストスイート名、テストケース名、ファイル名、行番号、アサートマクロ、アサートマクロの引数の値が表示されます。
その後、そのテストスイートに登録されている全テストケース数、実行して失敗したテストケース数、
スキップされたテストケース数が表示されます。

テストにパスするために、add関数とsub関数を修正します。

    [AddSub.c]
    - int add(int a, int b) { return 0; }
    - int sub(int a, int b) { return 0; }
    + int add(int a, int b) { return a + b; }
    + int sub(int a, int b) { return a - b; }

修正したらビルドして実行します。
このように出力されます。

    Suite: AddSubTest
    
    2 Tests, 0 Failures, 0 Skipped
    OK

全てのテストにパスすると、OKのメッセージが表示されます。
パスしたテストケースの情報は表示されません。

OKのメッセージは1つのテストスイートにつき1つ表示されます。
この例ではテストスイートは1つでしたが、テストスイートが複数の場合、
全てのテストスイートにOKが表示されるまで修正を繰り返してください。


## アサートマクロ

アサートマクロはテスト関数内で使用可能です。
各アサートマクロの引数は2回以上評価されないので、マクロ展開による副作用を気にする必要はありません。

アサーション失敗時はlongjmpによってテスト関数から抜けます。
ただし、PCUnitが`PCU_NO_SETJMP`マクロまたは`PCU_NO_LIBC`マクロが定義済みでビルドされている場合は、
longjmpの代わりにreturnによってテスト関数から抜けます。
この場合、スタブ等のテスト関数内で呼び出される関数から一気に抜けることはできなくなります。

以下にPCUnitが提供するアサートマクロを示します。

* **`PCU_ASSERT(expr)`**

    任意の式exprが真かどうかチェックします。
    偽ならば失敗を出力し、テスト関数から抜けます。


* **`PCU_ASSERT_TRUE(expr)`**

    任意の式exprが真かどうかチェックします。
    偽ならば失敗を出力し、テスト関数から抜けます。


* **`PCU_ASSERT_FALSE(expr)`**

    任意の式exprが偽かどうかチェックします。
    真ならば失敗を出力し、テスト関数から抜けます。


* **`PCU_ASSERT_EQUAL(expected, actual)`**

    expectedとactualが整数である前提で、expectedとactualが等しいかどうかチェックします。
    等しくないならば失敗を出力し、テスト関数から抜けます。


* **`PCU_ASSERT_NOT_EQUAL(expected, actual)`**

    expectedとactualが整数である前提で、expectedとactualが等しくないかどうかチェックします。
    等しいならば失敗を出力し、テスト関数から抜けます。


* **`PCU_ASSERT_PTR_EQUAL(expected, actual)`**

    expectedとactualがポインタである前提で、expectedとactualのポインタの値が等しいかどうかチェックします。
    等しくないならば失敗を出力し、テスト関数から抜けます。


* **`PCU_ASSERT_PTR_NOT_EQUAL(expected, actual)`**

    expectedとactualがポインタである前提で、expectedとactualのポインタの値が等しくないかどうかチェックします。
    等しいならば失敗を出力し、テスト関数から抜けます。


* **`PCU_ASSERT_PTR_NULL(value)`**

    valueがポインタである前提で、valueがNULLかどうかチェックします。
    NULLでないならば失敗を出力し、テスト関数から抜けます。


* **`PCU_ASSERT_PTR_NOT_NULL(value)`**

    valueがポインタである前提で、valueがNULLでないかどうかチェックします。
    NULLならば失敗を出力し、テスト関数から抜けます。


* **`PCU_ASSERT_STRING_EQUAL(expected, actual)`**

    expectedとactualが文字列(const char *)である前提で、expectedとactualの文字列が等しいかどうかチェックします。
    等しくないならば失敗を出力し、テスト関数から抜けます。


* **`PCU_ASSERT_STRING_NOT_EQUAL(expected, actual)`**

    expectedとactualが文字列(const char *)である前提で、expectedとactualの文字列が等しくないかどうかチェックします。
    等しいならば失敗を出力し、テスト関数から抜けます。


* **`PCU_ASSERT_NSTRING_EQUAL(expected, actual, n)`**

    expectedとactualが文字列(const char *)である前提で、expectedとactualの文字列の先頭からn文字が等しいかどうかチェックします。
    等しくないならば失敗を出力し、テスト関数から抜けます。


* **`PCU_ASSERT_NSTRING_NOT_EQUAL(expected, actual, n)`**

    expectedとactualが文字列(const char *)である前提で、expectedとactualの文字列の先頭からn文字が等しくないかどうかチェックします。
    等しいならば失敗を出力し、テスト関数から抜けます。


* **`PCU_ASSERT_STRINGW_EQUAL(expected, actual)`**

    expectedとactualがワイド文字列(const wchar_t *)である前提で、expectedとactualのワイド文字列が等しいかどうかチェックします。
    等しくないならば失敗を出力し、テスト関数から抜けます。
    失敗時の引数の文字列表示は、現在のロケールの`LC_CTYPE`カテゴリに依存します。
    なお、このマクロを使用するためにはPCUnitが`PCU_USE_WCHAR`マクロが定義済みでビルドされている必要があります。


* **`PCU_ASSERT_STRINGW_NOT_EQUAL(expected, actual)`**

    expectedとactualがワイド文字列(const wchar_t *)である前提で、expectedとactualのワイド文字列が等しくないかどうかチェックします。
    等しいならば失敗を出力し、テスト関数から抜けます。
    失敗時の引数の文字列表示は、現在のロケールの`LC_CTYPE`カテゴリに依存します。
    なお、このマクロを使用するためにはPCUnitが`PCU_USE_WCHAR`マクロが定義済みでビルドされている必要があります。


* **`PCU_ASSERT_NSTRINGW_EQUAL(expected, actual, n)`**

    expectedとactualがワイド文字列(const wchar_t *)である前提で、expectedとactualのワイド文字列の先頭からn文字が等しいかどうかチェックします。
    等しくないならば失敗を出力し、テスト関数から抜けます。
    失敗時の引数の文字列表示は、現在のロケールの`LC_CTYPE`カテゴリに依存します。
    なお、このマクロを使用するためにはPCUnitが`PCU_USE_WCHAR`マクロが定義済みでビルドされている必要があります。


* **`PCU_ASSERT_NSTRINGW_NOT_EQUAL(expected, actual, n)`**

    expectedとactualがワイド文字列(const wchar_t *)である前提で、expectedとactualのワイド文字列の先頭からn文字が等しくないかどうかチェックします。
    等しいならば失敗を出力し、テスト関数から抜けます。
    失敗時の引数の文字列表示は、現在のロケールの`LC_CTYPE`カテゴリに依存します。
    なお、このマクロを使用するためにはPCUnitが`PCU_USE_WCHAR`マクロが定義済みでビルドされている必要があります。


* **`PCU_ASSERT_STRINGT_EQUAL(expected, actual)`**

    `_UNICODE`マクロまたは`UNICODE`マクロが定義されている場合は`PCU_ASSERT_STRINGW_EQUAL`に展開され、
    そうでない場合は`PCU_ASSERT_STRING_EQUAL`に展開されます。


* **`PCU_ASSERT_STRINGT_NOT_EQUAL(expected, actual)`**

    `_UNICODE`マクロまたは`UNICODE`マクロが定義されている場合は`PCU_ASSERT_STRINGW_NOT_EQUAL`に展開され、
    そうでない場合は`PCU_ASSERT_STRING_NOT_EQUAL`に展開されます。


* **`PCU_ASSERT_NSTRINGT_EQUAL(expected, actual, n)`**

    `_UNICODE`マクロまたは`UNICODE`マクロが定義されている場合は`PCU_ASSERT_NSTRINGW_EQUAL`に展開され、
    そうでない場合は`PCU_ASSERT_NSTRING_EQUAL`に展開されます。


* **`PCU_ASSERT_NSTRINGT_NOT_EQUAL(expected, actual, n)`**

    `_UNICODE`マクロまたは`UNICODE`マクロが定義されている場合は`PCU_ASSERT_NSTRINGW_NOT_EQUAL`に展開され、
    そうでない場合は`PCU_ASSERT_NSTRING_NOT_EQUAL`に展開されます。


* **`PCU_ASSERT_DOUBLE_EQUAL(expected, actual, delta)`**

    expectedとactualとdeltaが浮動小数点数である前提で、|expected - actual| <= |delta|が真かどうかチェックします。
    偽ならば失敗を出力し、テスト関数から抜けます。
    なお、PCUnitが`PCU_NO_FLOATINGPOINT`マクロが定義済みでビルドされている場合は使用できません。


* **`PCU_ASSERT_DOUBLE_NOT_EQUAL(expected, actual, delta)`**

    expectedとactualとdeltaが浮動小数点数である前提で、|expected - actual| <= |delta|が偽かどうかチェックします。
    真ならば失敗を出力し、テスト関数から抜けます。
    なお、PCUnitが`PCU_NO_FLOATINGPOINT`マクロが定義済みでビルドされている場合は使用できません。


* **`PCU_ASSERT_OPERATOR(lhs, op, rhs)`**

    lhsとrhsが符号無し整数を返す任意の式でopが代入以外の任意の二項演算子である前提で、((lhs) op (rhs)) が真かどうかチェックします。
    偽ならば失敗を出力し、テスト関数から抜けます。

    例:
    * `PCU_ASSERT_OPERATOR(x < 0, ||, 100 <= x);` xが0未満または100以上かチェック
    * `PCU_ASSERT_OPERATOR(x, &, 0x01);` xの最下位ビットが立っているかチェック


* **`PCU_ASSERT_OPERATOR_INT(lhs, op, rhs)`**

    lhsとrhsが符号付き整数を返す任意の式でopが代入以外の任意の二項演算子である前提で、((lhs) op (rhs)) が真かどうかチェックします。
    偽ならば失敗を出力し、テスト関数から抜けます。

    例:
    * `PCU_ASSERT_OPERATOR_INT(x, <, -1);` xが-1より小さい値かチェック


* **`PCU_ASSERT_OPERATOR_DOUBLE(lhs, op, rhs)`**

    lhsとrhsが浮動小数点数を返す任意の式でopが代入以外の任意の二項演算子である前提で、((lhs) op (rhs)) が真かどうかチェックします。
    偽ならば失敗を出力し、テスト関数から抜けます。
    なお、PCUnitが`PCU_NO_FLOATINGPOINT`マクロが定義済みでビルドされている場合は使用できません。

    例:
    * `PCU_ASSERT_OPERATOR_DOUBLE(x, >=, 1.0);` xが1.0以上の値かチェック


* **`PCU_ASSERT*_MESSAGE(*, msg)`**

    上記の各アサートマクロには、最後の引数にmsgを追加した、マクロ名の末尾に`_MESSAGE`が付いたバージョンがあります。
    アサーション失敗時にmsgで指定したメッセージを追加表示できること以外は各アサートマクロと同じです。
    msgの型は`const char *`です。
    なお、`PCU_format`/`PCU_formatW`を使用すればprintf/wprintfと同じ形式の文字列を指定することができます。

    例:
    * `PCU_ASSERT_MESSAGE(x, "x is false");`
    * `PCU_ASSERT_EQUAL_MESSAGE(x, y, PCU_format("a:%s, b:%x, c:%d", a, b, c));`


* **`PCU_FAIL(msg)`**

    無条件に失敗を出力し、テスト関数から抜けます。
    失敗表示の際にmsgで指定したメッセージを表示します。
    msgの型は`const char *`です。
    なお、`PCU_format`/`PCU_formatW`を使用すればprintf/wprintfと同じ形式の文字列を指定することができます。


* **`PCU_MESSAGE(msg)`**

    合否チェックを行わず、msgで指定したメッセージを表示します。
    このマクロはテスト関数から抜けません。
    msgの型は`const char *`です。
    なお、`PCU_format`/`PCU_formatW`を使用すればprintf/wprintfと同じ形式の文字列を指定することができます。


## API

* **`void PCU_run(const PCU_SuiteMethod *suite_methods, int num)`**

    `PCU_SuiteMethod`の配列`suite_methods`から導かれる全テストを実行します。
    `num`には`suite_methods`の要素数を指定します。


* **`void PCU_console_run(const PCU_SuiteMethod *suite_methods, int num)`**

    コンソールモードでテストを実行します。
    実行するテストを対話的に選択したい場合、`PCU_run`の代わりにこちらを使います。
    引数は`PCU_run`と同じです。
    なお、PCUnitが`PCU_NO_CONSOLE_RUN`マクロが定義済みでビルドされている場合は使用できません。


* **`void PCU_set_putchar(PCU_Putchar func)`**

    1文字出力する関数を設定します。
    `PCU_Putchar`型はputcharと同じ`int(*)(int)`型のtypedefです。
    `PCU_run`または`PCU_console_run`の前に必ず設定してください。


* **`void PCU_set_getchar(PCU_Getchar func)`**

    入力した1文字を取得する関数を設定します。
    `PCU_Getchar`型はgetcharと同じ`int(*)(void)`型のtypedefです。
    `PCU_console_run`の前に必ず設定してください。


* **`void PCU_enable_color(void)`**

    ターミナルがカラー表示に対応していれば、テスト結果のカラー表示を有効にします。


* **`void PCU_disable_color(void)`**

    テスト結果のカラー表示を無効にします。


* **`const char *PCU_suite_name(void)`**

    現在実行中のテストスイート名を返します。
    テスト関数、setup関数、teardown関数、initialize関数、cleanup関数内で使用可能です。


* **`const char *PCU_test_name(void)`**

    現在実行中のテストケース名を返します。
    テスト関数、setup関数、teardown関数内で使用可能です。


* **`int PCU_repeat_counter(void)`**

    テストケースの実行が、現在何回目(0オリジン)であるかを返します。

    * `PCU_Test`のntimesが0の場合は常に0を返します。
    * ntimesが0より大きい場合は0 ～ ntimes - 1の値を返します。

    テスト関数、setup関数、teardown関数内で使用可能です。

    例えば、データ駆動テストにおけるテストデータのテーブルから要素を取得するためのインデックスに利用できます。
    `PCU_Test`のntimesにはテストデータの要素数を指定してください。


* **`const char *PCU_format(const char *format, ...)`**

    引数に指定したprintf形式の文字列を静的領域に展開し、その領域のポインタを返します。
    `PCU_ASSERT*_MESSAGE`、`PCU_FAIL`、`PCU_MESSAGE`のメッセージをprintf形式にしたい場合に利用できます。
    静的領域のサイズはPCUnitのビルド時に`PCU_FORMAT_BUFSIZE`マクロの値で定義できます。定義しない場合のデフォルト値は512です。
    内部でvsprintfを使用しているので文字列展開後のサイズが`PCU_FORMAT_BUFSIZE`を超えないように注意してください。

    例:
    * `PCU_ASSERT_EQUAL_MESSAGE(x, y, PCU_format("a:%s, b:%x, c:%d", a, b, c));`

    なお、PCUnitが`PCU_NO_STDARG`マクロが定義済みでビルドされている場合は使用できません。
    `PCU_NO_STDARG`マクロが定義済みの場合は、代わりに`PCU_format0`～`PCU_format9`(0～9は引数`format`より後の引数の個数)を使用してください。

    例:
    * `PCU_ASSERT_EQUAL_MESSAGE(x, y, PCU_format3("a:%s, b:%x, c:%d", a, b, c));`


* **`const char *PCU_formatW(const wchar_t *format, ...)`**

    引数に指定したwprintf形式のワイド文字列をマルチバイト文字列に変換して静的領域に展開し、その領域のポインタを返します。
    `PCU_ASSERT*_MESSAGE`、`PCU_FAIL`、`PCU_MESSAGE`のメッセージをwprintf形式にしたい場合に利用できます。
    静的領域のサイズはPCUnitのビルド時に`PCU_FORMAT_BUFSIZE`マクロの値で定義できます。定義しない場合のデフォルト値は512です。
    内部でvsprintfを使用しているので文字列展開後のサイズが`PCU_FORMAT_BUFSIZE`を超えないように注意してください。
    なお、この関数を使用するためにはPCUnitが`PCU_USE_WCHAR`マクロが定義済みでビルドされている必要があります。

    例:
    * `PCU_ASSERT_EQUAL_MESSAGE(x, y, PCU_formatW(L"a:%s, b:%x, c:%d", a, b, c));`


* **`const char *PCU_formatT(const TCHAR *format, ...)`**

    `PCU_formatT`はマクロです。
    `_UNICODE`マクロまたは`UNICODE`マクロが定義されている場合は`PCU_formatW`に展開され、
    そうでない場合は`PCU_format`に展開されます。


## ヘルパーマクロ

`PCU_Test`の配列初期化の際に、テストケース名とテスト関数を同じ名前にする場合、ヘルパーマクロが利用できます。
ヘルパーマクロは次のように定義されています。

    #define PCU_TEST(func)                  { #func, func }
    #define PCU_TEST_REPEATED(func, ntimes) { #func, func, ntimes }
    #define PCU_TEST_SKIPPED(func)          { #func, func, -1 }

### 例

    static PCU_Test tests[] = {
        PCU_TEST(test_hoge),            /* test_hoge関数を登録 */
        PCU_TEST(test_piyo),            /* test_piyo関数を登録 */
        PCU_TEST_REPEATED(test_foo, 5), /* test_foo関数を登録(5回実行) */
        PCU_TEST_SKIPPED(test_bar),     /* test_bar関数を登録(スキップ) */
    };


## ユーティリティ

### pcunit_template.rb

`pcunit_template.rb`はPCUnit用のソースファイルの雛形を生成するRubyスクリプトです。
書式は次の通りです。

    pcunit_template.rb [suite_name ...] [-d DIR] [-p [EXT]] [-m [FILE]] [-M [FILE]] [-o]

`suite_name`に1つ以上のテストスイート名を指定すると、`テストスイート名.c`というファイルを生成します。
生成されたファイルにはあらかじめ`テストスイート名_suite()`という名前のスイートメソッドや、
setup関数・テスト関数等の雛形が定義されています。
適宜このファイルを編集して使用してください。
また、main関数やMakefileの雛形を生成することもできます。

#### オプション

* `-d DIR`

    `DIR`にファイルを生成するディレクトリを指定してください。
    このオプションを省略した場合、カレントディレクトリを指定したと見なします。

* `-p [EXT]`

    生成するファイルをC++にします。
    `EXT`にC++の拡張子(cpp, cc, cxx等)を指定してください。`EXT`を省略した場合、拡張子はcppになります。

* `-m [FILE]`

    `PCU_run`を呼び出すmain関数を定義したファイルを生成します。
    `FILE`にファイル名を指定してください。
    `FILE`を省略した場合、ファイル名は`main.c`(`-p`指定の場合は`main.cpp`)になります。

* `-M [FILE]`

    テストプロジェクトのMakefileの雛型を生成します。
    `FILE`にファイル名を指定してください。
    `FILE`を省略した場合、ファイル名は`Makefile`になります。

* `-o`

    生成するファイルと同じ名前のファイルが既に存在している場合でも上書きします。


### pcunit_register.rb

`pcunit_register.rb`はテスト関数を自動登録するRubyスクリプトです。
書式は次の通りです。

    pcunit_register.rb [-d DIR] [-n]

`pcunit_register.rb`は、あるディレクトリ以下の全てのPCUnit用のソースファイルを再帰的に検索し、
`test`で始まるテスト関数の中で、`PCU_Test`のstatic配列の初期化に登録されていない関数を自動的に登録します。
また、`PCU_run`の引数になる`PCU_SuiteMethod`の配列の初期化の登録も行います。

ソースファイルを更新するので、更新前のファイルはファイル名に`.bak`を付けてバックアップします。
更新する必要がないファイルには何もしません。

使用するビルドツールにて、テストプロジェクトのビルド時にこのスクリプトを実行するように設定しておけば、
テスト関数を手動で登録をする必要がなくなります。
例えばMakefileならばallターゲットを次のように修正するとよいでしょう。

    - all: $(TARGET)
    + all: pcunit_register $(TARGET)
    + 
    + pcunit_register:
    +     pcunit_register.rb

#### オプション

* `-d DIR`

    `DIR`にテストプロジェクトのソースファイルのディレクトリを指定してください。
    このオプションを省略した場合、カレントディレクトリを指定したと見なします。

* `-n`

    更新前ファイルのバックアップをしなくなります。


## ライセンス

PCUnitはzlibライセンスに従って配布されます。
ライセンスの詳細は、COPYINGファイル(日本語参考訳はCOPYING.jaファイル)をご覧ください。


<!-- vim:set ts=4 sts=4 sw=4 et ft=mkd: -->
