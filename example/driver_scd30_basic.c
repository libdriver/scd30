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
 * @file      driver_scd30_basic.c
 * @brief     driver scd30 basic source file
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

#include "driver_scd30_basic.h"

static scd30_handle_t gs_handle;        /**< scd30 handle */

/**
 * @brief     basic example init
 * @param[in] interface is the chip interface
 * @param[in] mbar is the pressure compensation
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      mbar = 0: no pressure compensation
 */
uint8_t scd30_basic_init(scd30_interface_t interface, uint16_t mbar)
{
    uint8_t res;
    uint16_t second;
    scd30_bool_t enable;
    
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
    
    /* get measurement interval */
    res = scd30_get_measurement_interval(&gs_handle, &second);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: get measurement interval failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    
    /* check config */
    if (second != SCD30_BASIC_DEFAULT_MEASUREMENT_INTERVAL)
    {
        /* set default measurement interval */
        res = scd30_set_measurement_interval(&gs_handle, SCD30_BASIC_DEFAULT_MEASUREMENT_INTERVAL);
        if (res != 0)
        {
            scd30_interface_debug_print("scd30: set measurement interval failed.\n");
            (void)scd30_deinit(&gs_handle);
            
            return 1;
        }
    }
    
    /* get automatic self calibration */
    res = scd30_get_automatic_self_calibration(&gs_handle, &enable);
    if (res != 0)
    {
        scd30_interface_debug_print("scd30: get automatic self calibration failed.\n");
        (void)scd30_deinit(&gs_handle);
        
        return 1;
    }
    
    /* check config */
    if (enable != SCD30_BASIC_DEFAULT_AUTO_SELF_CALIBRATION)
    {
        /* set default automatic self calibration */
        res = scd30_set_automatic_self_calibration(&gs_handle, SCD30_BASIC_DEFAULT_AUTO_SELF_CALIBRATION);
        if (res != 0)
        {
            scd30_interface_debug_print("scd30: set automatic self calibration failed.\n");
            (void)scd30_deinit(&gs_handle);
            
            return 1;
        }
    }
    
    /* check mbar */
    if (mbar != 0)
    {
        res = scd30_start_measurement_with_pressure_compensation(&gs_handle, mbar);
        if (res != 0)
        {
            scd30_interface_debug_print("scd30: start measurement with pressure compensation failed.\n");
            (void)scd30_deinit(&gs_handle);
            
            return 1;
        }
    }
    else
    {
        res = scd30_start_measurement(&gs_handle);
        if (res != 0)
        {
            scd30_interface_debug_print("scd30: start measurement failed.\n");
            (void)scd30_deinit(&gs_handle);
            
            return 1;
        }
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t scd30_basic_deinit(void)
{
    uint8_t res;
    
    /* stop measurement */
    res = scd30_stop_measurement(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    /* deinit */
    res = scd30_deinit(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read
 * @param[out] *data points to a scd30_data_t structure
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t scd30_basic_read(scd30_data_t *data)
{
    uint8_t res;
    
    /* read data */
    res = scd30_read(&gs_handle, data);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example reset
 * @return status code
 *         - 0 success
 *         - 1 reset failed
 * @note   none
 */
uint8_t scd30_basic_reset(void)
{
    if (scd30_soft_reset(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example get the version
 * @param[out] *major points to a major buffer
 * @param[out] *minor points to a minor buffer
 * @return     status code
 *             - 0 success
 *             - 1 get version failed
 * @note       none
 */
uint8_t scd30_basic_get_version(uint8_t *major, uint8_t *minor)
{
    uint16_t version;
    
    if (scd30_get_firmware_version(&gs_handle, &version) != 0)
    {
        return 1;
    }
    *major = (version >> 8) & 0xFF;
    *minor = (version >> 0) & 0xFF;
    
    return 0;
}
