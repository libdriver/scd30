/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_scd30.h
 * @brief     driver scd30 header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2024-08-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2024/08/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_SCD30_H
#define DRIVER_SCD30_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup scd30_driver scd30 driver function
 * @brief    scd30 driver modules
 * @{
 */

/**
 * @addtogroup scd30_basic_driver
 * @{
 */

/**
 * @brief scd30 interface enumeration definition
 */
typedef enum
{
    SCD30_INTERFACE_IIC  = 0x00,       /**< iic interface */
    SCD30_INTERFACE_UART = 0x01,       /**< uart interface */
} scd30_interface_t;

/**
 * @brief scd30 bool enumeration definition
 */
typedef enum
{
    SCD30_BOOL_FALSE = 0x0000,        /**< false */
    SCD30_BOOL_TRUE  = 0x0001,        /**< true */
} scd30_bool_t;

/**
 * @brief scd30 handle structure definition
 */
typedef struct scd30_handle_s
{
    uint8_t (*iic_init)(void);                                                /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                              /**< point to an iic_deinit function address */
    uint8_t (*iic_write_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);       /**< point to an iic_write_cmd function address */
    uint8_t (*iic_read_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);        /**< point to an iic_read_cmd function address */
    uint8_t (*uart_init)(void);                                               /**< point to a uart_init function address */
    uint8_t (*uart_deinit)(void);                                             /**< point to a uart_deinit function address */
    uint16_t (*uart_read)(uint8_t *buf, uint16_t len);                        /**< point to a uart_read function address */
    uint8_t (*uart_flush)(void);                                              /**< point to a uart_flush function address */
    uint8_t (*uart_write)(uint8_t *buf, uint16_t len);                        /**< point to a uart_write function address */
    void (*delay_ms)(uint32_t ms);                                            /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                          /**< point to a debug_print function address */
    uint8_t inited;                                                           /**< inited flag */
    uint8_t iic_uart;                                                         /**< iic uart */
} scd30_handle_t;

/**
 * @brief scd30 data structure definition
 */
typedef struct scd30_data_s
{
    uint16_t co2_raw;                /**< co2 raw */
    float co2_ppm;                   /**< co2 in ppm */
    uint16_t temperature_raw;        /**< temperature raw */
    float temperature_deg;           /**< temperature in degree */
    uint16_t humidity_raw;           /**< humidity raw */
    float humidity_percent;          /**< humidity in percent */
} scd30_data_t;

/**
 * @brief scd30 information structure definition
 */
typedef struct scd30_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[16];                /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} scd30_info_t;

/**
 * @}
 */

/**
 * @defgroup scd30_link_driver scd30 link driver function
 * @brief    scd30 link driver modules
 * @ingroup  scd30_driver
 * @{
 */

/**
 * @brief     initialize scd30_handle_t structure
 * @param[in] HANDLE points to an scd30 handle structure
 * @param[in] STRUCTURE is scd30_handle_t
 * @note      none
 */
#define DRIVER_SCD30_LINK_INIT(HANDLE, STRUCTURE)              memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link uart_init function
 * @param[in] HANDLE points to an scd30 handle structure
 * @param[in] FUC points to a uart_init function address
 * @note      none
 */
#define DRIVER_SCD30_LINK_UART_INIT(HANDLE, FUC)              (HANDLE)->uart_init = FUC

/**
 * @brief     link uart_deinit function
 * @param[in] HANDLE points to an scd30 handle structure
 * @param[in] FUC points to a uart_deinit function address
 * @note      none
 */
#define DRIVER_SCD30_LINK_UART_DEINIT(HANDLE, FUC)            (HANDLE)->uart_deinit = FUC

/**
 * @brief     link uart_read function
 * @param[in] HANDLE points to an scd30 handle structure
 * @param[in] FUC points to a uart_read function address
 * @note      none
 */
#define DRIVER_SCD30_LINK_UART_READ(HANDLE, FUC)              (HANDLE)->uart_read = FUC

/**
 * @brief     link uart_write function
 * @param[in] HANDLE points to an scd30 handle structure
 * @param[in] FUC points to a uart_write function address
 * @note      none
 */
#define DRIVER_SCD30_LINK_UART_WRITE(HANDLE, FUC)             (HANDLE)->uart_write = FUC

/**
 * @brief     link uart_flush function
 * @param[in] HANDLE points to an scd30 handle structure
 * @param[in] FUC points to a uart_flush function address
 * @note      none
 */
#define DRIVER_SCD30_LINK_UART_FLUSH(HANDLE, FUC)             (HANDLE)->uart_flush = FUC

/**
 * @brief     link iic_init function
 * @param[in] HANDLE points to an scd30 handle structure
 * @param[in] FUC points to an iic_init function address
 * @note      none
 */
#define DRIVER_SCD30_LINK_IIC_INIT(HANDLE, FUC)               (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to an scd30 handle structure
 * @param[in] FUC points to an iic_deinit function address
 * @note      none
 */
#define DRIVER_SCD30_LINK_IIC_DEINIT(HANDLE, FUC)             (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_write_cmd function
 * @param[in] HANDLE points to an scd30 handle structure
 * @param[in] FUC points to an iic_write_cmd function address
 * @note      none
 */
#define DRIVER_SCD30_LINK_IIC_WRITE_COMMAND(HANDLE, FUC)      (HANDLE)->iic_write_cmd = FUC

/**
 * @brief     link iic_read_cmd function
 * @param[in] HANDLE points to an scd30 handle structure
 * @param[in] FUC points to an iic_read_cmd function address
 * @note      none
 */
#define DRIVER_SCD30_LINK_IIC_READ_COMMAND(HANDLE, FUC)       (HANDLE)->iic_read_cmd = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an scd30 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_SCD30_LINK_DELAY_MS(HANDLE, FUC)               (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an scd30 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_SCD30_LINK_DEBUG_PRINT(HANDLE, FUC)            (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup scd30_basic_driver scd30 basic driver function
 * @brief    scd30 basic driver modules
 * @ingroup  scd30_driver
 * @{
 */

/**
 * @brief      get chip information
 * @param[out] *info points to an scd30 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t scd30_info(scd30_info_t *info);

/**
 * @brief     set the chip interface
 * @param[in] *handle points to an scd30 handle structure
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t scd30_set_interface(scd30_handle_t *handle, scd30_interface_t interface);

/**
 * @brief      get the chip interface
 * @param[in]  *handle points to an scd30 handle structure
 * @param[out] *interface points to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t scd30_get_interface(scd30_handle_t *handle, scd30_interface_t *interface);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an scd30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or uart initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 reset failed
 * @note      none
 */
uint8_t scd30_init(scd30_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to an scd30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or uart deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 soft reset failed
 * @note      none
 */
uint8_t scd30_deinit(scd30_handle_t *handle);

/**
 * @brief      read the result
 * @param[in]  *handle points to an scd30 handle structure
 * @param[out] *data points to an scd30 data structure
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 read timeout
 * @note       none
 */
uint8_t scd30_read(scd30_handle_t *handle, scd30_data_t *data);

/**
 * @brief     start the measurement with pressure compensation
 * @param[in] *handle points to an scd30 handle structure
 * @param[in] mbar is the ambient pressure compensation
 * @return    status code
 *            - 0 success
 *            - 1 start measurement failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 mbar is invalid
 * @note      700 <= mbar <= 1400
 */
uint8_t scd30_start_measurement_with_pressure_compensation(scd30_handle_t *handle, uint16_t mbar);

/**
 * @brief     start the measurement
 * @param[in] *handle points to an scd30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 start measurement failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t scd30_start_measurement(scd30_handle_t *handle);

/**
 * @brief     stop the measurement
 * @param[in] *handle points to an scd30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 stop measurement failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t scd30_stop_measurement(scd30_handle_t *handle);

/**
 * @brief     set the measurement interval
 * @param[in] *handle points to an scd30 handle structure
 * @param[in] second is the set interval
 * @return    status code
 *            - 0 success
 *            - 1 set measurement interval failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 second is invalid
 * @note      2 <= second <= 1800
 */
uint8_t scd30_set_measurement_interval(scd30_handle_t *handle, uint16_t second);

/**
 * @brief      get the measurement interval
 * @param[in]  *handle points to an scd30 handle structure
 * @param[out] *second points to a second buffer
 * @return     status code
 *             - 0 success
 *             - 1 get measurement interval failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd30_get_measurement_interval(scd30_handle_t *handle, uint16_t *second);

/**
 * @brief      get data ready status
 * @param[in]  *handle points to an scd30 handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get data ready status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd30_get_data_ready_status(scd30_handle_t *handle, uint16_t *status);

/**
 * @brief     set automatic self calibration
 * @param[in] *handle points to an scd30 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set automatic self calibration failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t scd30_set_automatic_self_calibration(scd30_handle_t *handle, scd30_bool_t enable);

/**
 * @brief      get automatic self calibration
 * @param[in]  *handle points to an scd30 handle structure
 * @param[out] *enable points to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 1 get automatic self calibration failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd30_get_automatic_self_calibration(scd30_handle_t *handle, scd30_bool_t *enable);

/**
 * @brief     set forced recalibration
 * @param[in] *handle points to an scd30 handle structure
 * @param[in] co2_ppm is the set co2 ppm
 * @return    status code
 *            - 0 success
 *            - 1 set forced recalibration failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 co2_ppm is invalid
 * @note      400 <= co2_ppm <= 2000
 */
uint8_t scd30_set_forced_recalibration(scd30_handle_t *handle, uint16_t co2_ppm);

/**
 * @brief      get forced recalibration
 * @param[in]  *handle points to an scd30 handle structure
 * @param[out] *co2_ppm points to a co2 ppm buffer
 * @return     status code
 *             - 0 success
 *             - 1 get forced recalibration failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd30_get_forced_recalibration(scd30_handle_t *handle, uint16_t *co2_ppm);

/**
 * @brief     set temperature offset
 * @param[in] *handle points to an scd30 handle structure
 * @param[in] deg is the set degree
 * @return    status code
 *            - 0 success
 *            - 1 set temperature offset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t scd30_set_temperature_offset(scd30_handle_t *handle, uint16_t deg);

/**
 * @brief      get temperature offset
 * @param[in]  *handle points to an scd30 handle structure
 * @param[out] *deg points to a degree buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature offset failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd30_get_temperature_offset(scd30_handle_t *handle, uint16_t *deg);

/**
 * @brief      convert the temperature offset to the register raw data
 * @param[in]  *handle points to an scd30 handle structure
 * @param[in]  deg is the degree
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd30_temperature_offset_convert_to_register(scd30_handle_t *handle, float deg, uint16_t *reg);

/**
 * @brief      convert the register raw data to the temperature offset
 * @param[in]  *handle points to an scd30 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *deg points to a degree buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd30_temperature_offset_convert_to_data(scd30_handle_t *handle, uint16_t reg, float *deg);

/**
 * @brief     set altitude compensation
 * @param[in] *handle points to an scd30 handle structure
 * @param[in] m is the set meter
 * @return    status code
 *            - 0 success
 *            - 1 set altitude compensation failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t scd30_set_altitude_compensation(scd30_handle_t *handle, uint16_t m);

/**
 * @brief      get altitude compensation
 * @param[in]  *handle points to an scd30 handle structure
 * @param[out] *m points to a meter buffer
 * @return     status code
 *             - 0 success
 *             - 1 get altitude compensation failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd30_get_altitude_compensation(scd30_handle_t *handle, uint16_t *m);

/**
 * @brief      get firmware version
 * @param[in]  *handle points to an scd30 handle structure
 * @param[out] *version points to a version buffer
 * @return     status code
 *             - 0 success
 *             - 1 get firmware version failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd30_get_firmware_version(scd30_handle_t *handle, uint16_t *version);

/**
 * @brief     soft reset
 * @param[in] *handle points to an scd30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 soft reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t scd30_soft_reset(scd30_handle_t *handle);

/**
 * @}
 */

/**
 * @defgroup scd30_extern_driver scd30 extern driver function
 * @brief    scd30 extern driver modules
 * @ingroup  scd30_driver
 * @{
 */

/**
 * @brief     set the chip register with iic interface
 * @param[in] *handle points to an scd30 handle structure
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t scd30_set_reg_iic(scd30_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register with iic interface
 * @param[in]  *handle points to an scd30 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd30_get_reg_iic(scd30_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      set and get the chip register with uart interface
 * @param[in]  *handle points to an scd30 handle structure
 * @param[in]  *input points to an input buffer
 * @param[in]  in_len is the input length
 * @param[out] *output points to an output buffer
 * @param[in]  out_len is the output length
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       none
 */
uint8_t scd30_set_get_reg_uart(scd30_handle_t *handle, uint8_t *input, uint16_t in_len, uint8_t *output, uint16_t out_len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
