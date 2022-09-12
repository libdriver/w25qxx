[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.png"/>
</div>

## LibDriver W25QXX

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/w25qxx/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

W25Q25XXシリアルフラッシュメモリは、スペース、ピン、および電力が限られているシステムにストレージソリューションを提供します。 25Qシリーズは、通常のシリアルフラッシュデバイスをはるかに超える柔軟性とパフォーマンスを提供します。 これらは、RAMへのコードシャドウイング、Dual / Quad SPI（XIP）から直接コードを実行し、音声、テキスト、およびデータを保存するのに理想的です。 このデバイスは、2.7V〜3.6Vの単一電源で動作し、消費電流はアクティブで4mA、パワーダウンで1μAと低くなっています。 すべてのデバイスは、省スペースパッケージで提供されます。

LibDriver W25QXXは、LibDriverによって起動されたW25QXXの全機能ドライバーです。フラッシュ読み取り、フラッシュ書き込みなどの機能を提供します。LibDriverはMISRAに準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver W25QXXのソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver W25QXX用のプラットフォームに依存しないSPI, QSPIバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver W25QXXドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver W25QXXプログラミング例が含まれています。

/ docディレクトリには、LibDriver W25QXXオフラインドキュメントが含まれています。

/ datasheetディレクトリには、W25QXXデータシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないSPI, QSPIバステンプレートを参照して、指定したプラットフォームのSPI, QSPIバスドライバを完成させます。

/ srcディレクトリ、/ interfaceディレクトリ、および/exampleディレクトリをプロジェクトに追加します。

### 使用

#### example basic

```C
#include "driver_w25qxx_basic.h"

uint8_t res;
uint8_t manufacturer;
uint8_t device_id;
uint8_t data[8];

res = w25qxx_basic_init(W25Q128, W25QXX_INTERFACE_SPI, W25QXX_BOOL_TRUE);
if (res != 0)
{
    return 1;
}

...
    
res = w25qxx_basic_get_id((uint8_t *)&manufacturer, (uint8_t *)&device_id);
if (res != 0)
{
    (void)w25qxx_basic_deinit();
    
    return 1;
}
w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X device id is 0x%02X.\n", manufacturer, device_id);
    
...    
    
res = w25qxx_basic_write(0x00000000, (uint8_t *)data, 8);
if (res != 0)
{
    (void)w25qxx_basic_deinit();
    
    return 1;
}

...

res = w25qxx_basic_read(0x00000000, (uint8_t *)data, 8);
if (res != 0)
{
    (void)w25qxx_basic_deinit();
    
    return 1;
}

...
    
(void)w25qxx_basic_deinit();

return 0;
```

#### example advance

```c
#include "driver_w25qxx_advance.h"

uint8_t res;
uint8_t manufacturer;
uint8_t device_id;
uint8_t data[8];

res = w25qxx_advance_init(W25Q128, W25QXX_INTERFACE_SPI, W25QXX_BOOL_TRUE);
if (res != 0)
{
    return 1;
}

...
    
res = w25qxx_advance_get_id((uint8_t *)&manufacturer, (uint8_t *)&device_id);
if (res != 0)
{
    (void)w25qxx_advance_deinit();
    
    return 1;
}
w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X device id is 0x%02X.\n", manufacturer, device_id);
    
...    
    
res = w25qxx_advance_write(0x00000000, (uint8_t *)data, 8);
if (res != 0)
{
    (void)w25qxx_advance_deinit();
    
    return 1;
}

...

res = w25qxx_advance_read(0x00000000, (uint8_t *)data, 8);
if (res != 0)
{
    (void)w25qxx_advance_deinit();
    
    return 1;
}

...
    
(void)w25qxx_advance_deinit();

return 0;
```

### ドキュメント

オンラインドキュメント: https://www.libdriver.com/docs/w25qxx/index.html

オフラインドキュメント: /doc/html/index.html

### 貢献

お問い合わせくださいlishifenging@outlook.com

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com