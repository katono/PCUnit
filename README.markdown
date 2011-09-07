<title>PCUnit - Portable C Unit Testing Framework</title>

# PCUnit - Portable C Unit Testing Framework

## 概要

PCUnitは組み込み開発でテスト駆動開発(Test-Driven Development; TDD)を実践するための
ポータブルなC言語/C++言語用ユニットテストフレームワークです。

PCUnitは以下のような特長があります。

* 機能

    * [CUnit](http://cunit.sourceforge.net/)ライクなアサートマクロをサポートします。
    * アサートマクロで失敗した内容を表示できます。さらにprintf形式で追加メッセージを表示できます。
    * テストフィクスチャ(setup, teardown, initialize, cleanup)をサポートします。
    * 1つのテストを繰り返し行うRepeatedTestをサポートします。これを利用して、データ駆動テストを行うこともできます。
    * ディスプレイやキーボードがないターゲット上でテストするために、ユーザー定義の入出力関数を設定できます。
    * インタラクティブなコンソールモードをサポートします。これはターゲット上でのシンプルなテストツールとしても使えます。
    * ターミナルが対応していればテスト結果のカラー表示ができます。
    * ユーティリティを使用すればテスト関数の雛形生成や自動登録ができます。

* 移植性

    * プラットフォーム非依存な実装なので、大抵のPC向けまたは組み込み向けのC/C++コンパイラでビルドすることができます。
      つまり、ホスト環境とターゲット環境の両方で動作する共通のテストコードを実装することができます。
    * たとえ標準Cライブラリが使えない環境でも、条件コンパイルの設定をすることで使用可能です。


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
インストール先ディレクトリを/usr/localから変更したい場合は、
make install INSTALLDIR=dirのように、dirに任意のインストール先ディレクトリを指定してください。

GNU開発環境でない場合は、PCUnitディレクトリ以下のソースファイルとヘッダファイルをテストプロジェクトに追加して
テストコードと共にビルドしてください。あるいはPCUnitの静的ライブラリをビルドするプロジェクトを作成し、
ビルドしたライブラリをテストプロジェクトに静的リンクしてください。

### 条件コンパイル

開発環境によって適宜コンパイラオプションでマクロを定義してください。

* vsnprintfが使用できない場合は、`PCU_NO_VSNPRINTF`マクロを定義してください。
* malloc/freeが使用できない場合は、`PCU_NO_MALLOC`マクロを定義してください。
* setjmp/longjmpが使用できない場合は、`PCU_NO_SETJMP`マクロを定義してください。
* strlen, strcmp, strncmp, strcpy, strncpy, memset, memcpy, atoiのいずれか1つでも使用できない場合は、
  `PCU_NO_LIBC`マクロを定義してください。
  このマクロを定義した場合は`PCU_NO_VSNPRINTF`、`PCU_NO_MALLOC`、`PCU_NO_SETJMP`を定義する必要はありません。
* プロセッサにFPUがなく、ソフトウェア浮動小数点ライブラリも使用できない場合は、
  `PCU_NO_FLOATINGPOINT`マクロを定義してください。
* コンソールモードが不要な場合は、`PCU_NO_CONSOLE_RUN`マクロを定義してください。
  `PCU_console_run`が使用できなくなりますが、コードサイズが少し小さくなります。

#### `PCU_NO_MALLOC` または `PCU_NO_LIBC` を定義した場合

標準のmallocが使えない場合、PCUnitはあらかじめ静的領域に2つのメモリプールを確保します。
必要ならばコンパイラオプションでそれぞれのメモリプールのサイズを調整してください。

1つ目のメモリプールはアサートマクロ失敗用データのメモリプールです。
アサートマクロ失敗の最大数を`PCU_MAX_FAILURES`マクロの値で定義できます。定義しない場合のデフォルト値は64です。
この最大数を超えてアサートマクロが失敗した場合はテスト結果が正しく表示されません。
なお、アサートマクロ失敗用データの構造体サイズは少なくとも32バイトあります。
つまりデフォルトで少なくとも32 * 64 = 2048バイトの静的領域を使用します。

2つ目のメモリプールは文字列表示用のメモリプールです。
このメモリプールのバイト数を`PCU_STRING_POOL_SIZE`マクロの値で定義できます。定義しない場合のデフォルト値は4096です。
このメモリプールは、`PCU_ASSERT_STRING_*`、`PCU_ASSERT_NSTRING_*`、`PCU_FAIL*`、
`PCU_MESSAGE*`のマクロで使用されます(`PCU_ASSERT_STRING_*`、`PCU_ASSERT_NSTRING_*`は失敗時のみ使用)。
このメモリプールを使い切ってしまった場合は`PCU_ASSERT_STRING_*`、`PCU_ASSERT_NSTRING_*`の引数や
`PCU_FAIL*`、`PCU_MESSAGE*`のメッセージが表示されません。
これらの文字列表示が不要ならば`PCU_STRING_POOL_SIZE`に1を指定してください。

#### `PCU_NO_*` のいずれかを定義した場合

* `size_t`型をtypedefした`stddef.h`が必要です。
  もし`stddef.h`がなければ作成してインクルードパスの通ったディレクトリに置いてください。
* テストプロジェクトのコンパイラオプションで、必ずPCUnitビルド時と同じ`PCU_NO_*`マクロを定義してください。


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
      Test: test_add ... FAILED
        1. AddSubTest.c:6
          PCU_ASSERT_EQUAL(3, add(2, 1))
            expected : 0x00000003 (3)
            actual   : 0x00000000 (0)
        2. AddSubTest.c:8
          PCU_ASSERT_EQUAL(1, add(-1, 2))
            expected : 0x00000001 (1)
            actual   : 0x00000000 (0)
      Test: test_sub ... FAILED
        1. AddSubTest.c:13
          PCU_ASSERT_EQUAL(1, sub(2, 1))
            expected : 0x00000001 (1)
            actual   : 0x00000000 (0)
        2. AddSubTest.c:15
          PCU_ASSERT_EQUAL(-3, sub(-1, 2))
            expected : 0xfffffffd (-3)
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
すべてのテストスイートにOKが表示されるまで修正を繰り返してください。


## アサートマクロ

全てのアサートマクロはテスト関数、またはテスト関数内で呼び出される関数で使用可能です。

### `PCU_ASSERT*`

`PCU_ASSERT*`マクロの引数は複数回展開されるので、副作用のないように指定してください。
`PCU_ASSERT*`マクロの戻り値は、失敗時に`PCU_MESSAGE*`マクロで追加メッセージを表示させたい場合などに利用できます。


* **`PCU_ASSERT(expr)`**

    任意の式exprが真かどうかチェックします。
    偽ならば失敗を登録し、偽を返します。真ならば真を返します。


* **`PCU_ASSERT_TRUE(expr)`**

    任意の式exprが真かどうかチェックします。
    偽ならば失敗を登録し、偽を返します。真ならば真を返します。


* **`PCU_ASSERT_FALSE(expr)`**

    任意の式exprが偽かどうかチェックします。
    真ならば失敗を登録し、偽を返します。偽ならば真を返します。


* **`PCU_ASSERT_EQUAL(expected, actual)`**

    expectedとactualが整数である前提で、expectedとactualが等しいかどうかチェックします。
    等しくないならば失敗を登録し、偽を返します。等しいならば真を返します。


* **`PCU_ASSERT_NOT_EQUAL(expected, actual)`**

    expectedとactualが整数である前提で、expectedとactualが等しくないかどうかチェックします。
    等しいならば失敗を登録し、偽を返します。等しくないならば真を返します。


* **`PCU_ASSERT_PTR_EQUAL(expected, actual)`**

    expectedとactualがポインタである前提で、expectedとactualのポインタの値が等しいかどうかチェックします。
    等しくないならば失敗を登録し、偽を返します。等しいならば真を返します。


* **`PCU_ASSERT_PTR_NOT_EQUAL(expected, actual)`**

    expectedとactualがポインタである前提で、expectedとactualのポインタの値が等しくないかどうかチェックします。
    等しいならば失敗を登録し、偽を返します。等しくないならば真を返します。


* **`PCU_ASSERT_PTR_NULL(value)`**

    valueがポインタである前提で、valueがNULLかどうかチェックします。
    NULLでないならば失敗を登録し、偽を返します。NULLならば真を返します。


* **`PCU_ASSERT_PTR_NOT_NULL(value)`**

    valueがポインタである前提で、valueがNULLでないかどうかチェックします。
    NULLならば失敗を登録し、偽を返します。NULLでないならば真を返します。


* **`PCU_ASSERT_STRING_EQUAL(expected, actual)`**

    expectedとactualが文字列である前提で、expectedとactualの文字列が等しいかどうかチェックします。
    等しくないならば失敗を登録し、偽を返します。等しいならば真を返します。


* **`PCU_ASSERT_STRING_NOT_EQUAL(expected, actual)`**

    expectedとactualが文字列である前提で、expectedとactualの文字列が等しくないかどうかチェックします。
    等しいならば失敗を登録し、偽を返します。等しくないならば真を返します。


* **`PCU_ASSERT_NSTRING_EQUAL(expected, actual, n)`**

    expectedとactualが文字列である前提で、expectedとactualの文字列の先頭からn文字が等しいかどうかチェックします。
    等しくないならば失敗を登録し、偽を返します。等しいならば真を返します。


* **`PCU_ASSERT_NSTRING_NOT_EQUAL(expected, actual, n)`**

    expectedとactualが文字列である前提で、expectedとactualの文字列の先頭からn文字が等しくないかどうかチェックします。
    等しいならば失敗を登録し、偽を返します。等しくないならば真を返します。


* **`PCU_ASSERT_DOUBLE_EQUAL(expected, actual, delta)`**

    expectedとactualとdeltaが浮動小数点数である前提で、|expected - actual| <= |delta|が真かどうかチェックします。
    偽ならば失敗を登録し、偽を返します。真ならば真を返します。
    なお、PCUnitが`PCU_NO_FLOATINGPOINT`マクロが定義済みでビルドされている場合は使用できません。


* **`PCU_ASSERT_DOUBLE_NOT_EQUAL(expected, actual, delta)`**

    expectedとactualとdeltaが浮動小数点数である前提で、|expected - actual| <= |delta|が偽かどうかチェックします。
    真ならば失敗を登録し、偽を返します。偽ならば真を返します。
    なお、PCUnitが`PCU_NO_FLOATINGPOINT`マクロが定義済みでビルドされている場合は使用できません。


* **`PCU_ASSERT_OPERATOR(lhs, op, rhs)`**

    lhsとrhsが任意の式でopが代入以外の任意の二項演算子である前提で、((lhs) op (rhs)) が真かどうかチェックします。
    偽ならば失敗を登録し、偽を返します。真ならば真を返します。

    例:

    * `PCU_ASSERT_OPERATOR(x, <, y);` xがyより小さい値かチェック
    * `PCU_ASSERT_OPERATOR(0 <= x, &&, x < 100);` xが0以上かつ100未満かチェック
    * `PCU_ASSERT_OPERATOR(x, &, 0x01);` xの最下位ビットが立っているかチェック


### `PCU_ASSERT*_FATAL`

* **`PCU_ASSERT_FATAL(expr)`**
* **`PCU_ASSERT_TRUE_FATAL(expr)`**
* **`PCU_ASSERT_FALSE_FATAL(expr)`**
* **`PCU_ASSERT_EQUAL_FATAL(expected, actual)`**
* **`PCU_ASSERT_NOT_EQUAL_FATAL(expected, actual)`**
* **`PCU_ASSERT_PTR_EQUAL_FATAL(expected, actual)`**
* **`PCU_ASSERT_PTR_NOT_EQUAL_FATAL(expected, actual)`**
* **`PCU_ASSERT_PTR_NULL_FATAL(value)`**
* **`PCU_ASSERT_PTR_NOT_NULL_FATAL(value)`**
* **`PCU_ASSERT_STRING_EQUAL_FATAL(expected, actual)`**
* **`PCU_ASSERT_STRING_NOT_EQUAL_FATAL(expected, actual)`**
* **`PCU_ASSERT_NSTRING_EQUAL_FATAL(expected, actual, n)`**
* **`PCU_ASSERT_NSTRING_NOT_EQUAL_FATAL(expected, actual, n)`**
* **`PCU_ASSERT_DOUBLE_EQUAL_FATAL(expected, actual, delta)`**
* **`PCU_ASSERT_DOUBLE_NOT_EQUAL_FATAL(expected, actual, delta)`**
* **`PCU_ASSERT_OPERATOR_FATAL(lhs, op, rhs)`**

全ての`PCU_ASSERT*`マクロにはマクロ名の末尾に`_FATAL`が付いたバージョンがあります。
`PCU_ASSERT*_FATAL`マクロは失敗時にlongjmpによりテスト関数から強制的に抜けます。
例えばスタブ内から一気に抜けたい場合などに利用できます。
なお、PCUnitが`PCU_NO_SETJMP`マクロまたは`PCU_NO_LIBC`マクロが定義済みでビルドされている場合は使用できません。


### `PCU_ASSERT*_RETURN`

* **`PCU_ASSERT_RETURN(expr)`**
* **`PCU_ASSERT_TRUE_RETURN(expr)`**
* **`PCU_ASSERT_FALSE_RETURN(expr)`**
* **`PCU_ASSERT_EQUAL_RETURN(expected, actual)`**
* **`PCU_ASSERT_NOT_EQUAL_RETURN(expected, actual)`**
* **`PCU_ASSERT_PTR_EQUAL_RETURN(expected, actual)`**
* **`PCU_ASSERT_PTR_NOT_EQUAL_RETURN(expected, actual)`**
* **`PCU_ASSERT_PTR_NULL_RETURN(value)`**
* **`PCU_ASSERT_PTR_NOT_NULL_RETURN(value)`**
* **`PCU_ASSERT_STRING_EQUAL_RETURN(expected, actual)`**
* **`PCU_ASSERT_STRING_NOT_EQUAL_RETURN(expected, actual)`**
* **`PCU_ASSERT_NSTRING_EQUAL_RETURN(expected, actual, n)`**
* **`PCU_ASSERT_NSTRING_NOT_EQUAL_RETURN(expected, actual, n)`**
* **`PCU_ASSERT_DOUBLE_EQUAL_RETURN(expected, actual, delta)`**
* **`PCU_ASSERT_DOUBLE_NOT_EQUAL_RETURN(expected, actual, delta)`**
* **`PCU_ASSERT_OPERATOR_RETURN(lhs, op, rhs)`**

全ての`PCU_ASSERT*`マクロにはマクロ名の末尾に`_RETURN`が付いたバージョンがあります。
`PCU_ASSERT*_RETURN`マクロは失敗時にテスト関数からreturnします。
returnなのでスタブ等のテスト関数内で呼び出される関数から一気に抜けることはできません。
これらのマクロは値を返しません。


### `PCU_FAIL*`

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
* **`PCU_FAIL(...)`**

`PCU_FAIL*`マクロは無条件に失敗を登録します。
表示メッセージをprintfと同じ形式でformatとarg[1-9]に指定します。
arg[1-9]の個数によって使用するマクロを選択してください。これらのマクロは値を返しません。
`PCU_FAIL`はマクロの可変長引数(`__VA_ARGS__`)と標準C関数のvsnprintfが使用可能な場合に使用できます。


### `PCU_FAIL*_FATAL`

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
* **`PCU_FAIL_FATAL(...)`**

`PCU_FAIL*`マクロにはマクロ名の末尾に`_FATAL`が付いたバージョンがあります。
`PCU_FAIL*_FATAL`マクロは常にlongjmpによりテスト関数から強制的に抜けます。
例えばスタブ内から一気に抜けたい場合などに利用できます。
なお、PCUnitが`PCU_NO_SETJMP`マクロまたは`PCU_NO_LIBC`マクロが定義済みでビルドされている場合は使用できません。


### `PCU_FAIL*_RETURN`

* **`PCU_FAIL0_RETURN(format)`**
* **`PCU_FAIL1_RETURN(format, arg1)`**
* **`PCU_FAIL2_RETURN(format, arg1, arg2)`**
* **`PCU_FAIL3_RETURN(format, arg1, arg2, arg3)`**
* **`PCU_FAIL4_RETURN(format, arg1, arg2, arg3, arg4)`**
* **`PCU_FAIL5_RETURN(format, arg1, arg2, arg3, arg4, arg5)`**
* **`PCU_FAIL6_RETURN(format, arg1, arg2, arg3, arg4, arg5, arg6)`**
* **`PCU_FAIL7_RETURN(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7)`**
* **`PCU_FAIL8_RETURN(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)`**
* **`PCU_FAIL9_RETURN(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)`**
* **`PCU_FAIL_RETURN(...)`**

`PCU_FAIL*`マクロにはマクロ名の末尾に`_RETURN`が付いたバージョンがあります。
`PCU_FAIL*_RETURN`マクロは常にテスト関数からreturnします。
returnなのでスタブ等のテスト関数内で呼び出される関数から一気に抜けることはできません。


### `PCU_MESSAGE*`

* **`PCU_MESSAGE0(format)`**
* **`PCU_MESSAGE1(format, arg1)`**
* **`PCU_MESSAGE2(format, arg1, arg2)`**
* **`PCU_MESSAGE3(format, arg1, arg2, arg3)`**
* **`PCU_MESSAGE4(format, arg1, arg2, arg3, arg4)`**
* **`PCU_MESSAGE5(format, arg1, arg2, arg3, arg4, arg5)`**
* **`PCU_MESSAGE6(format, arg1, arg2, arg3, arg4, arg5, arg6)`**
* **`PCU_MESSAGE7(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7)`**
* **`PCU_MESSAGE8(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)`**
* **`PCU_MESSAGE9(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)`**
* **`PCU_MESSAGE(...)`**

`PCU_MESSAGE*`マクロは合否チェックを行わず、表示メッセージのみを登録します。
表示メッセージをprintfと同じ形式でformatとarg[1-9]に指定します。
arg[1-9]の個数によって使用するマクロを選択してください。これらのマクロは値を返しません。
`PCU_MESSAGE`はマクロの可変長引数(`__VA_ARGS__`)と標準C関数のvsnprintfが使用可能な場合に使用できます。


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
    `PCU_Putchar`型はputcharと同じ`int(*)(int)`型のtypedefです。指定したfuncによって、テスト結果が出力されます。
    なお、`PCU_NO_*`マクロのいずれも定義されていない場合はデフォルトで`PCU_set_putchar(putchar)`の状態に設定されています。


* **`void PCU_set_getchar(PCU_Getchar func)`**

    `PCU_console_run`で使用する、入力した1文字を取得する関数を設定します。
    `PCU_Getchar`型はgetcharと同じ`int(*)(void)`型のtypedefです。指定したfuncによって、入力した文字を取得します。
    なお、`PCU_NO_*`マクロのいずれも定義されていない場合はデフォルトで`PCU_set_getchar(getchar)`の状態に設定されています。


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

    pcunit_template.rb suite_name1 [suite_name2 ...] [-d DIR] [-p [EXT]] [-m [FILE]] [-o]

引数`suite_name*`に1つ以上のテストスイート名を指定すると、`テストスイート名.c`というファイルを生成します。
生成されたファイルにはあらかじめ`テストスイート名_suite()`という名前のスイートメソッドや、
setup関数・テスト関数等の雛形が定義されています。
適宜このファイルを編集して使用してください。

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
