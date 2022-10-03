[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.png"/>
</div>

## LibDriver W25QXX

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/w25qxx/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The W25Q25XX Serial Flash memory provides a storage solution for systems with limited space, pins and power. The 25Q series offers flexibility and performance well beyond ordinary Serial Flash devices. They are ideal for code shadowing to RAM, executing code directly from Dual/Quad SPI (XIP) and storing voice, text and data. The device operates on a single 2.7V to 3.6V power supply with current consumption as low as 4mA active and 1μA for power-down. All devices are offered in space-saving packages.

LibDriver W25QXX is the full function driver of W25QXX launched by LibDriver.It provides functions of flash reading,flash writing, etc. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver W25QXX source files.

/interface includes LibDriver W25QXX SPI or QSPI platform independent template.

/test includes LibDriver W25QXX driver test code and this code can test the chip necessary function simply.

/example includes LibDriver W25QXX sample code.

/doc includes LibDriver W25QXX offline document.

/datasheet includes W25QXX datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

### Install

Reference /interface SPI or QSPI platform independent template and finish your platform SPI or QSPI driver.

Add /src, /interface and /example to your project.

### Usage

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

### Document

Online documents: https://www.libdriver.com/docs/w25qxx/index.html

Offline documents: /doc/html/index.html

### Contributing

Please sent an e-mail to lishifenging@outlook.com

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please sent an e-mail to lishifenging@outlook.com