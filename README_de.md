[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.png"/>
</div>

## LibDriver W25QXX

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/w25qxx/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der serielle Flash-Speicher W25Q25XX bietet eine Speicherlösung für Systeme mit begrenztem Platz, Pins und Leistung. Die 25Q-Serie bietet Flexibilität und Leistung, die weit über gewöhnliche serielle Flash-Geräte hinausgeht. Sie sind ideal für das Code-Shadowing in den RAM, das Ausführen von Code direkt von Dual/Quad SPI (XIP) und das Speichern von Sprache, Text und Daten. Das Bauteil wird mit einer einzigen 2,7-V- bis 3,6-V-Stromversorgung mit einem Stromverbrauch von nur 4 mA aktiv und 1 μA beim Ausschalten betrieben. Alle Geräte werden in platzsparenden Verpackungen angeboten.

LibDriver W25QXX ist der voll funktionsfähige Treiber von W25QXX, der von LibDriver gestartet wurde. Er bietet Funktionen zum Flash-Lesen, Flash-Schreiben usw. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver W25QXX-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver W25QXX SPI, QSPI.

/test enthält den Testcode des LibDriver W25QXX-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver W25QXX-Beispielcode.

/doc enthält das LibDriver W25QXX-Offlinedokument.

/Datenblatt enthält W25QXX-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

### Installieren

Verweisen Sie auf eine plattformunabhängige SPI, QSPI-Schnittstellenvorlage und stellen Sie Ihren Plattform-SPI, QSPI-Treiber fertig.

Fügen Sie /src, /interface und /example zu Ihrem Projekt hinzu.

### Nutzung

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

### Dokument

Online-Dokumente: https://www.libdriver.com/docs/w25qxx/index.html

Offline-Dokumente: /doc/html/index.html

### Beitrag

Bitte senden Sie eine E-Mail an lishifenging@outlook.com

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com