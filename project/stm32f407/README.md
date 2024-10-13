### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

UART2 Pin:  TX/RX PA2/PA3.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. SCD30

#### 3.1 Command Instruction

1. Show scd30 chip and driver information.

   ```shell
   scd30 (-i | --information)
   ```

2. Show scd30 help.

   ```shell
   scd30 (-h | --help)
   ```

3. Show scd30 pin connections of the current board.

   ```shell
   scd30 (-p | --port)
   ```

4. Run scd30 register test.

   ```shell
   scd30 (-t reg | --test=reg) [--interface=<iic | uart>]
   ```

5. Run scd30 read test, num means the test times.

   ```shell
   scd30 (-t read | --test=read) [--interface=<iic | uart>] [--times=<num>]
   ```

6. Run scd30 basic read function, num means the read times.

   ```shell
   scd30 (-e read | --example=read) [--interface=<iic | uart>] [--times=<num>]
   ```

10. Run scd30 basic get version function.  

    ```shell
    scd30 (-e version | --example=version) [--interface=<iic | uart>]
    ```

11. Run scd30 basic reset function.  

    ```shell
    scd30 (-e reset | --example=reset) [--interface=<iic | uart>]
    ```

#### 3.2 Command Example

```shell
scd30 -i

scd30: chip is Sensirion SCD30.
scd30: manufacturer is Sensirion.
scd30: interface is UART IIC.
scd30: driver version is 1.0.
scd30: min supply voltage is 3.3V.
scd30: max supply voltage is 5.5V.
scd30: max current is 75.00mA.
scd30: max temperature is 70.0C.
scd30: min temperature is -40.0C.
```

```shell
scd30 -p

scd30: IIC interface SCL connected to GPIOB PIN8.
scd30: IIC interface SDA connected to GPIOB PIN9.
scd30: UART interface TX connected to GPIOA PIN3.
scd30: UART interface RX connected to GPIOA PIN2.
```

```shell
scd30 -t reg --interface=iic

scd30: chip is Sensirion SCD30.
scd30: manufacturer is Sensirion.
scd30: interface is UART IIC.
scd30: driver version is 1.0.
scd30: min supply voltage is 3.3V.
scd30: max supply voltage is 5.5V.
scd30: max current is 75.00mA.
scd30: max temperature is 70.0C.
scd30: min temperature is -40.0C.
scd30: start register test.
scd30: scd30_set_interface/scd30_get_interface test.
scd30: set interface iic.
scd30: check interface ok.
scd30: set interface uart.
scd30: check interface ok.
scd30: scd30_set_measurement_interval/scd30_get_measurement_interval test.
scd30: set measurement interval 770 seconds.
scd30: check measurement interval ok.
scd30: scd30_get_data_ready_status test.
scd30: get data ready status 0x0000.
scd30: scd30_set_automatic_self_calibration/scd30_get_automatic_self_calibration test.
scd30: disable automatic self calibration.
scd30: check automatic self calibration ok.
scd30: enable automatic self calibration.
scd30: check automatic self calibration ok.
scd30: scd30_set_forced_recalibration/scd30_get_forced_recalibration test.
scd30: set forced recalibration 407 ppm.
scd30: check forced recalibration ok.
scd30: scd30_set_temperature_offset/scd30_get_temperature_offset test.
scd30: set temperature offset 9.
scd30: check temperature offset ok.
scd30: scd30_temperature_offset_convert_to_register/scd30_temperature_offset_convert_to_data test.
scd30: set temperature offset 0.60.
scd30: temperature offset before is 0.60.
scd30: temperature offset register is 0x3C.
scd30: temperature offset convert is 0.60.
scd30: scd30_set_altitude_compensation/scd30_get_altitude_compensation test.
scd30: set altitude compensation 3m.
scd30: check altitude compensation ok.
scd30: scd30_get_firmware_version test.
scd30: get firmware version 0x0342.
scd30: scd30_soft_reset test.
scd30: check soft reset ok.
scd30: finish register test.
```

```shell
scd30 -t read --interface=iic --times=3

scd30: chip is Sensirion SCD30.
scd30: manufacturer is Sensirion.
scd30: interface is UART IIC.
scd30: driver version is 1.0.
scd30: min supply voltage is 3.3V.
scd30: max supply voltage is 5.5V.
scd30: max current is 75.00mA.
scd30: max temperature is 70.0C.
scd30: min temperature is -40.0C.
scd30: start read test.
scd30: start measurement with pressure compensation test.
scd30: co2 is 689.91ppm.
scd30: temperature is 26.96C.
scd30: humidity is 59.36%.
scd30: co2 is 1371.15ppm.
scd30: temperature is 26.98C.
scd30: humidity is 59.33%.
scd30: co2 is 1428.93ppm.
scd30: temperature is 27.00C.
scd30: humidity is 59.37%.
scd30: start measurement test.
scd30: co2 is 1297.65ppm.
scd30: temperature is 26.98C.
scd30: humidity is 59.29%.
scd30: co2 is 1207.07ppm.
scd30: temperature is 27.02C.
scd30: humidity is 59.30%.
scd30: co2 is 1364.35ppm.
scd30: temperature is 27.02C.
scd30: humidity is 59.26%.
scd30: finish read test.
```

```shell
scd30 -e read --interface=iic --times=3

scd30: 1/3.
scd30: co2 is 819.81ppm.
scd30: temperature is 26.96C.
scd30: humidity is 59.27%.
scd30: 2/3.
scd30: co2 is 1540.82ppm.
scd30: temperature is 26.96C.
scd30: humidity is 59.33%.
scd30: 3/3.
scd30: co2 is 1490.01ppm.
scd30: temperature is 26.95C.
scd30: humidity is 59.41%.
```

```shell
scd30 -e version --interface=iic

scd30: major is 0x03 minor is 0x42.
```

```shell
scd30 -e reset --interface=iic

scd30: reset the chip.
```

```shell
scd30 -h

Usage:
  scd30 (-i | --information)
  scd30 (-h | --help)
  scd30 (-p | --port)
  scd30 (-t reg | --test=reg) [--interface=<iic | uart>]
  scd30 (-t read | --test=read) [--interface=<iic | uart>] [--times=<num>]
  scd30 (-e read | --example=read) [--interface=<iic | uart>] [--times=<num>]
  scd30 (-e reset | --example=reset) [--interface=<iic | uart>]
  scd30 (-e version | --example=version) [--interface=<iic | uart>]

Options:
  -e <read | version | reset>, --example=<read | version | reset> 
                                          Run the driver example.
  -h, --help                              Show the help.
  -i, --information                       Show the chip information.
      --interface=<iic | uart>            Set the chip interface.([default: iic])
  -p, --port                              Display the pin connections of the current board.
  -t <reg | read>, --test=<reg | read>    Run the driver test.
      --times=<num>                       Set the running times.([default: 3])
```

