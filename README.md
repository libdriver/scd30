[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SCD30

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/scd30/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

CMOSens® Technology for IR detection enables carbon dioxide measurements of the highest accuracy at a competitive price.Along with the NDIR measurement technology for detecting CO2 comes a best-in-class Sensirion humidity and temperature sensor integrated on the very same sensor module. Ambient humidity and temperature can be measured by Sensirion’s algorithm expertise through modelling and compensating of external heat sources without the need of any additional components. The very small module height allows easy integration into different applications. Carbon Dioxide is a key indicator for indoor air quality. Thanks to new energy standards and better insulation, houses have become increasingly energy-efficient, but the air quality can deteriorate rapidly. Active ventilation is needed to maintain a comfortable and healthy indoor environment and improve the well-being and productivity of the inhabitants. Sensirion sensor solutions offer an accurate and stable monitoring of CO2 in the air, as well as temperature and humidity. This enables our customers to develop new solutions that increase energy efficiency and simultaneously support the well-being of everyone.

LibDriver SCD30 is the full function driver of SCD30 launched by LibDriver. It provides CO2 reading, temperature reading, humidity reading and other functions. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver SCD30 source files.

/interface includes LibDriver SCD30 IIC,UART platform independent template.

/test includes LibDriver SCD30 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver SCD30 sample code.

/doc includes LibDriver SCD30 offline document.

/datasheet includes SCD30 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface IIC,UART platform independent template and finish your platform IIC,UART driver. 

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

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

### Document

Online documents: [https://www.libdriver.com/docs/scd30/index.html](https://www.libdriver.com/docs/scd30/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

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

Please send an e-mail to lishifenging@outlook.com.