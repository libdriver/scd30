[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SCD30

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/scd30/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

用於IR探測的CMOSens ®  科技有助於以具有競爭力的價格實現最高精度的CO₂量測。 Sensirion多樣的感測器產品和算灋使溫濕度感測器更好地集成在其中，而小體積讓此產品更易於集成到不同應用當中。 二氧化碳是室內空氣品質的一個關鍵名額。 由於新的能源標準和更好的隔熱性，房屋已變得越來越節能，但空氣品質卻在迅速惡化。 為了改善健康和提高生產效率，可以通過自動換風來維持一個舒適而健康的室內環境。 Sensirion感測器解決方案可提供準確而穩定的CO₂和溫濕度監測，使客戶能够開發新的方案，提高能源效率並兼顧人體健康。 最重要的是，SCD30符合RESET ® 與WELL ™ 相關的健康建築標準。

LibDriver SCD30是LibDriver推出的SCD30的全功能驅動，該驅動提供CO2讀取、溫度讀取、濕度讀取等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver SCD30的源文件。

/interface目錄包含了LibDriver SCD30與平台無關的IIC、UART總線模板。

/test目錄包含了LibDriver SCD30驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver SCD30編程範例。

/doc目錄包含了LibDriver SCD30離線文檔。

/datasheet目錄包含了SCD30數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC、UART總線模板，完成指定平台的IIC、UART總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/scd30/index.html](https://www.libdriver.com/docs/scd30/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。