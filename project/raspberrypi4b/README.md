### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

UART Pin: TX/RX GPIO14/GPIO15.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```
#### 2.2 Configuration

Enable serial port.

Disable serial console.

#### 2.3 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.4 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(scd30 REQUIRED)
```

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
./scd30 -i

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
./scd30 -p

scd30: IIC interface SCL connected to GPIO3(BCM).
scd30: IIC interface SDA connected to GPIO2(BCM).
scd30: UART interface TX connected to GPIO15(BCM).
scd30: UART interface RX connected to GPIO14(BCM).
```

```shell
./scd30 -t reg --interface=uart

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
scd30: set measurement interval 385 seconds.
scd30: check measurement interval ok.
scd30: scd30_get_data_ready_status test.
scd30: get data ready status 0x0000.
scd30: scd30_set_automatic_self_calibration/scd30_get_automatic_self_calibration test.
scd30: disable automatic self calibration.
scd30: check automatic self calibration ok.
scd30: enable automatic self calibration.
scd30: check automatic self calibration ok.
scd30: scd30_set_forced_recalibration/scd30_get_forced_recalibration test.
scd30: set forced recalibration 406 ppm.
scd30: check forced recalibration ok.
scd30: scd30_set_temperature_offset/scd30_get_temperature_offset test.
scd30: set temperature offset 7.
scd30: check temperature offset ok.
scd30: scd30_temperature_offset_convert_to_register/scd30_temperature_offset_convert_to_data test.
scd30: set temperature offset 0.15.
scd30: temperature offset before is 0.15.
scd30: temperature offset register is 0x0F.
scd30: temperature offset convert is 0.15.
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
./scd30 -t read --interface=uart --times=3

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
scd30: co2 is 421.71ppm.
scd30: temperature is 28.10C.
scd30: humidity is 57.18%.
scd30: co2 is 1015.22ppm.
scd30: temperature is 28.11C.
scd30: humidity is 57.09%.
scd30: co2 is 1026.14ppm.
scd30: temperature is 28.11C.
scd30: humidity is 57.09%.
scd30: start measurement test.
scd30: co2 is 904.12ppm.
scd30: temperature is 28.11C.
scd30: humidity is 57.09%.
scd30: co2 is 849.71ppm.
scd30: temperature is 28.14C.
scd30: humidity is 57.13%.
scd30: co2 is 895.86ppm.
scd30: temperature is 28.15C.
scd30: humidity is 57.10%.
scd30: finish read test.
```

```shell
./scd30 -e read --interface=uart --times=3

scd30: 1/3.
scd30: co2 is 553.71ppm.
scd30: temperature is 28.49C.
scd30: humidity is 56.43%.
scd30: 2/3.
scd30: co2 is 776.27ppm.
scd30: temperature is 28.49C.
scd30: humidity is 56.40%.
scd30: 3/3.
scd30: co2 is 832.70ppm.
scd30: temperature is 28.52C.
scd30: humidity is 56.40%.
```

```shell
./scd30 -e version --interface=uart

scd30: major is 0x03 minor is 0x42.
```

```shell
./scd30 -e reset --interface=uart

scd30: reset the chip.
```

```shell
./scd30 -h

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
