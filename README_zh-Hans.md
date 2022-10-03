[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.png"/>
</div>

## LibDriver W25QXX

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/w25qxx/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

W25QXX串行Flash存储器面向受限于空间、引脚和功耗的系统，提供了一种存储解决方案。25Q系列存储器相比于普通的串行Flash器件提供更好的灵活性和性能表现。它们是代码存储、代码直接通过双线/四线SPI运行、存储音频、文本和数据的理想选择。该器件可在2.7V到3.6V的供电电压下工作，工作时电流消耗最低4mA，在掉电模式下消耗仅1uA。所有的器件都提供节省空间的封装。

LibDriver W25QXX是LibDriver推出的W25QXX全功能驱动，该驱动提供Flash读取，Flash写入等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver W25QXX的源文件。

/interface目录包含了LibDriver W25QXX与平台无关的SPI或QSPI总线模板。

/test目录包含了LibDriver W25QXX驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver W25QXX编程范例。

/doc目录包含了LibDriver W25QXX离线文档。

/datasheet目录包含了W25QXX数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

### 安装

参考/interface目录下与平台无关的SPI和QSPI总线模板，完成指定平台的SPI或QSPI总线驱动。

将/src目录，/interface目录和/example目录加入工程。

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

```C
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

### 文档

在线文档: https://www.libdriver.com/docs/w25qxx/index.html

离线文档: /doc/html/index.html

### 贡献

请联系lishifenging@outlook.com

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com