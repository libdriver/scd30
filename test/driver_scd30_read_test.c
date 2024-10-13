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
 * @file      driver_scd30_read_test.c
 * @brief     driver scd30 read test source file
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

#include "driver_scd30_read_test.h"

static scd30_handle_t gs_handle;        /**< scd30 handle */

/**
 * @brief     read test
 * @param[in] interface is the chip interface
 * @param[in] times is the test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t scd30_read_test(scd30_interface_t interface, uint32_t times)
{
    uint8_t res;
    uint32_t i;
    scd30_info_t info;
    scd30_data_t data;
    
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
    
    /* start read test */
    scd30_interface_debug_print("scd30: start read test.\n");
    
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
    
    /* set measurement interval 2s */
    res = scd30_set_measurement_interval(&gs_handle, 2);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: set measurement interval failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable automatic self calibration */
    res = scd30_set_automatic_self_calibration(&gs_handle, SCD30_BOOL_TRUE);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: set automatic self calibration failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    scd30_interface_debug_print("scd30: start measurement with pressure compensation test.\n");
    
    /* start measurement */
    res = scd30_start_measurement_with_pressure_compensation(&gs_handle, 1013);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: start measurement with pressure compensation failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 2000 ms */
    scd30_interface_delay_ms(2000);
    
    for (i = 0; i < times; i++)
    {
        res = scd30_read(&gs_handle, &data);
        if (res != 0)
        {
            scd30_interface_debug_print("scd30: read failed.\n");
            (void)scd30_deinit(&gs_handle);
            
            return 1;
        }
        scd30_interface_debug_print("scd30: co2 is %0.2fppm.\n", data.co2_ppm);
        scd30_interface_debug_print("scd30: temperature is %0.2fC.\n", data.temperature_deg);
        scd30_interface_debug_print("scd30: humidity is %0.2f%%.\n", data.humidity_percent);
        
        /* delay 2000 ms */
        scd30_interface_delay_ms(2000);
    }
    
    /* stop measurement */
    res = scd30_stop_measurement(&gs_handle);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: stop measurement failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    scd30_interface_debug_print("scd30: start measurement test.\n");
    
    /* start measurement */
    res = scd30_start_measurement(&gs_handle);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: start measurement failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 2000 ms */
    scd30_interface_delay_ms(2000);
    
    for (i = 0; i < times; i++)
    {
        res = scd30_read(&gs_handle, &data);
        if (res != 0)
        {
            scd30_interface_debug_print("scd30: read failed.\n");
            (void)scd30_deinit(&gs_handle);
            
            return 1;
        }
        scd30_interface_debug_print("scd30: co2 is %0.2fppm.\n", data.co2_ppm);
        scd30_interface_debug_print("scd30: temperature is %0.2fC.\n", data.temperature_deg);
        scd30_interface_debug_print("scd30: humidity is %0.2f%%.\n", data.humidity_percent);
        
        /* delay 2000 ms */
        scd30_interface_delay_ms(2000);
    }
    
    /* stop measurement */
    res = scd30_stop_measurement(&gs_handle);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: stop measurement failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish read test */
    scd30_interface_debug_print("scd30: finish read test.\n");
    (void)scd30_deinit(&gs_handle);
    
    return 0;
}
