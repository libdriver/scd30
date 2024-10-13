[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SCD30

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/scd30/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

IR 検出用の CMOSens® テクノロジーにより、競争力のある価格で最高精度の二酸化炭素測定が可能になります。CO2 検出用の NDIR 測定テクノロジーとともに、同じセンサー モジュールにクラス最高の Sensirion 湿度および温度センサーが統合されています。Sensirion のアルゴリズム専門知識により、外部熱源のモデリングと補正を通じて周囲の湿度と温度を測定でき、追加コンポーネントは必要ありません。モジュールの高さが非常に小さいため、さまざまなアプリケーションに簡単に統合できます。二酸化炭素は、室内空気質の重要な指標です。新しいエネルギー基準と優れた断熱性により、住宅のエネルギー効率はますます向上していますが、空気質は急速に悪化する可能性があります。快適で健康的な室内環境を維持し、居住者の健康と生産性を向上させるには、積極的な換気が必要です。Sensirion センサー ソリューションは、空気中の CO2 だけでなく、温度と湿度も正確かつ安定して監視します。これにより、お客様はエネルギー効率を高め、同時にすべての人の健康をサポートする新しいソリューションを開発できます。

LibDriver SCD30 は、LibDriver がリリースした SCD30 のフル機能ドライバーです。CO2 読み取り、温度読み取り、湿度読み取りなどの機能を提供します。LibDriver は MISRA に準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver SCD30のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver SCD30用のプラットフォームに依存しないIIC,UARTバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver SCD30ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver SCD30プログラミング例が含まれています。

/ docディレクトリには、LibDriver SCD30オフラインドキュメントが含まれています。

/ datasheetディレクトリには、SCD30データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIIC,UARTバステンプレートを参照して、指定したプラットフォームのIIC,UARTバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

#### example basic

```C
#include "driver_scd30_basic.h"

uint8_t res;
uint32_t i;

/* init */
res = scd30_basic_init(interface, 0);
if (res != 0)
{
    return 1;
}

...
    
/* delay 2000 ms */
scd30_interface_delay_ms(2000);

/* loop */
for (i = 0; i < 3; i++)
{
    scd30_data_t data;

    /* read data */
    res = scd30_basic_read(&data);
    if (res != 0)
    {
        (void)scd30_basic_deinit();

        return 1;
    }

    /* print */
    scd30_interface_debug_print("scd30: %d/%d.\n", i + 1, 3);
    scd30_interface_debug_print("scd30: co2 is %0.2fppm.\n", data.co2_ppm);
    scd30_interface_debug_print("scd30: temperature is %0.2fC.\n", data.temperature_deg);
    scd30_interface_debug_print("scd30: humidity is %0.2f%%.\n", data.humidity_percent);

    /* delay 2000 ms */
    scd30_interface_delay_ms(2000);
    
    ...
}

...
    
/* deinit */
(void)scd30_basic_deinit();

return 0;
```

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/scd30/index.html](https://www.libdriver.com/docs/scd30/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。