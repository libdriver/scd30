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
 * @file      driver_scd30_register_test.c
 * @brief     driver scd30 register test source file
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

#include "driver_scd30_register_test.h"
#include <stdlib.h>

static scd30_handle_t gs_handle;        /**< scd30 handle */

/**
 * @brief     register test
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t scd30_register_test(scd30_interface_t interface)
{
    uint8_t res;
    uint16_t second;
    uint16_t second_check;
    uint16_t co2_ppm;
    uint16_t co2_ppm_check;
    uint16_t status;
    uint16_t deg;
    uint16_t deg_check;
    uint16_t reg;
    uint16_t m;
    uint16_t m_check;
    uint16_t version;
    float f;
    float f_check;
    scd30_info_t info;
    scd30_bool_t enable;
    scd30_interface_t interface_check;
    
    /* link functions */
    DRIVER_SCD30_LINK_INIT(&gs_handle, scd30_handle_t);
    DRIVER_SCD30_LINK_UART_INIT(&gs_handle, scd30_interface_uart_init);
    DRIVER_SCD30_LINK_UART_DEINIT(&gs_handle, scd30_interface_uart_deinit);
    DRIVER_SCD30_LINK_UART_READ(&gs_handle, scd30_interface_uart_read);
    DRIVER_SCD30_LINK_UART_WRITE(&gs_handle, scd30_interface_uart_write);
    DRIVER_SCD30_LINK_UART_FLUSH(&gs_handle, scd30_interface_uart_flush);
    DRIVER_SCD30_LINK_IIC_INIT(&gs_handle, scd30_interface_iic_init);
    DRIVER_SCD30_LINK_IIC_DEINIT(&gs_handle, scd30_interface_iic_deinit);
    DRIVER_SCD30_LINK_IIC_WRITE_COMMAND(&gs_handle, scd30_interface_iic_write_cmd);
    DRIVER_SCD30_LINK_IIC_READ_COMMAND(&gs_handle, scd30_interface_iic_read_cmd);
    DRIVER_SCD30_LINK_DELAY_MS(&gs_handle, scd30_interface_delay_ms);
    DRIVER_SCD30_LINK_DEBUG_PRINT(&gs_handle, scd30_interface_debug_print);
    
    /* get information */
    res = scd30_info(&info);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        scd30_interface_debug_print("scd30: chip is %s.\n", info.chip_name);
        scd30_interface_debug_print("scd30: manufacturer is %s.\n", info.manufacturer_name);
        scd30_interface_debug_print("scd30: interface is %s.\n", info.interface);
        scd30_interface_debug_print("scd30: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        scd30_interface_debug_print("scd30: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        scd30_interface_debug_print("scd30: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        scd30_interface_debug_print("scd30: max current is %0.2fmA.\n", info.max_current_ma);
        scd30_interface_debug_print("scd30: max temperature is %0.1fC.\n", info.temperature_max);
        scd30_interface_debug_print("scd30: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    scd30_interface_debug_print("scd30: start register test.\n");
    
    /* scd30_set_interface/scd30_get_interface test */
    scd30_interface_debug_print("scd30: scd30_set_interface/scd30_get_interface test.\n");
    
    /* set iic interface */
    res = scd30_set_interface(&gs_handle, SCD30_INTERFACE_IIC);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: set interface failed.\n");
    
        return 1;
    }
    scd30_interface_debug_print("scd30: set interface iic.\n");
    res = scd30_get_interface(&gs_handle, &interface_check);
    if (res != 0)
    {
        scd30_interface_debug_print("gps30: set interface failed.\n");
    
        return 1;
    }
    scd30_interface_debug_print("scd30: check interface %s.\n", interface_check == SCD30_INTERFACE_IIC ? "ok" : "error");
    
    /* set uart interface */
    res = scd30_set_interface(&gs_handle, SCD30_INTERFACE_UART);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: set interface failed.\n");
    
        return 1;
    }
    scd30_interface_debug_print("scd30: set interface uart.\n");
    res = scd30_get_interface(&gs_handle, &interface_check);
    if (res != 0)
    {
        scd30_interface_debug_print("gps30: set interface failed.\n");
    
        return 1;
    }
   scd30_interface_debug_print("scd30: check interface %s.\n", interface_check == SCD30_INTERFACE_UART ? "ok" : "error");
    
    /* set the interface */
    res = scd30_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: set interface failed.\n");
    
        return 1;
    }
    
    /* init the chip */
    res = scd30_init(&gs_handle);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: init failed.\n");
    
        return 1;
    }

    /* scd30_set_measurement_interval/scd30_get_measurement_interval test */
    scd30_interface_debug_print("scd30: scd30_set_measurement_interval/scd30_get_measurement_interval test.\n");
    
    second = rand() % 1000 + 2;
    res = scd30_set_measurement_interval(&gs_handle, second);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: set measurement interval failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    scd30_interface_debug_print("scd30: set measurement interval %d seconds.\n", second);
    res = scd30_get_measurement_interval(&gs_handle, &second_check);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: get measurement interval failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    scd30_interface_debug_print("scd30: check measurement interval %s.\n", second == second_check ? "ok" : "error");
    
    /* scd30_get_data_ready_status test */
    scd30_interface_debug_print("scd30: scd30_get_data_ready_status test.\n");
    
    /* get data ready status */
    res = scd30_get_data_ready_status(&gs_handle, &status);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: get data ready status failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    scd30_interface_debug_print("scd30: get data ready status 0x%04X.\n", status);
    
    /* scd30_set_automatic_self_calibration/scd30_get_automatic_self_calibration test */
    scd30_interface_debug_print("scd30: scd30_set_automatic_self_calibration/scd30_get_automatic_self_calibration test.\n");
    
    /* disable automatic self calibration */
    res = scd30_set_automatic_self_calibration(&gs_handle, SCD30_BOOL_FALSE);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: set automatic self calibration failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    scd30_interface_debug_print("scd30: disable automatic self calibration.\n");
    res = scd30_get_automatic_self_calibration(&gs_handle, &enable);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: get automatic self calibration failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    scd30_interface_debug_print("scd30: check automatic self calibration %s.\n", enable == SCD30_BOOL_FALSE ? "ok" : "error");
    
    /* enable automatic self calibration */
    res = scd30_set_automatic_self_calibration(&gs_handle, SCD30_BOOL_TRUE);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: set automatic self calibration failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    scd30_interface_debug_print("scd30: enable automatic self calibration.\n");
    res = scd30_get_automatic_self_calibration(&gs_handle, &enable);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: get automatic self calibration failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    scd30_interface_debug_print("scd30: check automatic self calibration %s.\n", enable == SCD30_BOOL_TRUE ? "ok" : "error");
    
    /* scd30_set_forced_recalibration/scd30_get_forced_recalibration test */
    scd30_interface_debug_print("scd30: scd30_set_forced_recalibration/scd30_get_forced_recalibration test.\n");
    
    co2_ppm = rand() % 10 + 400;
    res = scd30_set_forced_recalibration(&gs_handle, co2_ppm);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: set forced recalibration failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    scd30_interface_debug_print("scd30: set forced recalibration %d ppm.\n", co2_ppm);
    res = scd30_get_forced_recalibration(&gs_handle, &co2_ppm_check);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: get forced recalibration failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    scd30_interface_debug_print("scd30: check forced recalibration %s.\n", co2_ppm == co2_ppm_check ? "ok" : "error");
    
    /* scd30_set_temperature_offset/scd30_get_temperature_offset test */
    scd30_interface_debug_print("scd30: scd30_set_temperature_offset/scd30_get_temperature_offset test.\n");
    
    deg = rand() % 10;
    res = scd30_set_temperature_offset(&gs_handle, deg);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: set temperature offset failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    scd30_interface_debug_print("scd30: set temperature offset %d.\n", deg);
    res = scd30_get_temperature_offset(&gs_handle, &deg_check);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: get temperature offset failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    scd30_interface_debug_print("scd30: check temperature offset %s.\n", deg == deg_check ? "ok" : "error");
    
    /* scd30_temperature_offset_convert_to_register/scd30_temperature_offset_convert_to_data test */
    scd30_interface_debug_print("scd30: scd30_temperature_offset_convert_to_register/scd30_temperature_offset_convert_to_data test.\n");
    
    f = (float)(rand() % 100) / 100.0f;
    res = scd30_temperature_offset_convert_to_register(&gs_handle, f, &reg);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: temperature offset convert to register failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    scd30_interface_debug_print("scd30: set temperature offset %0.2f.\n", f);
    res = scd30_temperature_offset_convert_to_data(&gs_handle, reg, &f_check);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: temperature offset convert to data failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    scd30_interface_debug_print("scd30: temperature offset before is %0.2f.\n", f);
    scd30_interface_debug_print("scd30: temperature offset register is 0x%02X.\n", reg);
    scd30_interface_debug_print("scd30: temperature offset convert is %0.2f.\n", f_check);
    
    /* scd30_set_altitude_compensation/scd30_get_altitude_compensation test */
    scd30_interface_debug_print("scd30: scd30_set_altitude_compensation/scd30_get_altitude_compensation test.\n");
    
    m = rand() % 10;
    res = scd30_set_altitude_compensation(&gs_handle, m);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: set altitude compensation failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    scd30_interface_debug_print("scd30: set altitude compensation %dm.\n", m);
    res = scd30_get_altitude_compensation(&gs_handle, &m_check);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: get altitude compensation failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    scd30_interface_debug_print("scd30: check altitude compensation %s.\n", m == m_check ? "ok" : "error");
    
    /* scd30_get_firmware_version test */
    scd30_interface_debug_print("scd30: scd30_get_firmware_version test.\n");
    
    /* get firmware version */
    res = scd30_get_firmware_version(&gs_handle, &version);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: get firmware version failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    scd30_interface_debug_print("scd30: get firmware version 0x%04X.\n", version);
    
    /* scd30_soft_reset test */
    scd30_interface_debug_print("scd30: scd30_soft_reset test.\n");
    scd30_interface_delay_ms(1000);
    res = scd30_soft_reset(&gs_handle);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: soft rest failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    scd30_interface_debug_print("scd30: check soft reset %s.\n", res == 0 ? "ok" : "error");
    
    /* finish register test */
    scd30_interface_debug_print("scd30: finish register test.\n");
    (void)scd30_deinit(&gs_handle);
    
    return 0;
}
