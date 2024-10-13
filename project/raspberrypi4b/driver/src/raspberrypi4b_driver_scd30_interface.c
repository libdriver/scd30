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
 * @file      raspberrypi4b_driver_scd30_interface.c
 * @brief     raspberrypi4b driver scd30 interface source file
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

#include "driver_scd30_interface.h"
#include "iic.h"
#include "uart.h"
#include <stdarg.h>

/**
 * @brief iic device name definition
 */
#define IIC_DEVICE_NAME "/dev/i2c-1"        /**< iic device name */

/**
 * @brief iic device handle definition
 */
static int iic_gs_fd;                       /**< iic handle */

/**
 * @brief uart device name definition
 */
#define UART_DEVICE_NAME "/dev/ttyS0"       /**< uart device name */

/**
 * @brief uart device handle definition
 */
static int uart_gs_fd;                      /**< uart handle */

/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t scd30_interface_iic_init(void)
{
    return iic_init(IIC_DEVICE_NAME, &iic_gs_fd);
}

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t scd30_interface_iic_deinit(void)
{
    return iic_deinit(iic_gs_fd);
}

/**
 * @brief      interface iic bus read
 * @param[in]  addr is the iic device write address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t scd30_interface_iic_read_cmd(uint8_t addr, uint8_t *buf, uint16_t len)
{
    return iic_read_cmd(iic_gs_fd, addr, buf, len);
}

/**
 * @brief     interface iic bus write
 * @param[in] addr is the iic device write address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t scd30_interface_iic_write_cmd(uint8_t addr, uint8_t *buf, uint16_t len)
{
    return iic_write_cmd(iic_gs_fd, addr, buf, len);
}

/**
 * @brief  interface uart init
 * @return status code
 *         - 0 success
 *         - 1 uart init failed
 * @note   none
 */
uint8_t scd30_interface_uart_init(void)
{
    return uart_init(UART_DEVICE_NAME, &uart_gs_fd, 19200, 8, 'N', 1);
}

/**
 * @brief  interface uart deinit
 * @return status code
 *         - 0 success
 *         - 1 uart deinit failed
 * @note   none
 */
uint8_t scd30_interface_uart_deinit(void)
{
    return uart_deinit(uart_gs_fd);
}

/**
 * @brief      interface uart read
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint16_t scd30_interface_uart_read(uint8_t *buf, uint16_t len)
{
    uint32_t l = len;

    if (uart_read(uart_gs_fd, buf, (uint32_t *)&l) != 0)
    {
        return 0;
    }
    else
    {
        return l;
    }
}

/**
 * @brief     interface uart write
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t scd30_interface_uart_write(uint8_t *buf, uint16_t len)
{
    uint32_t l = len;

    if (uart_write(uart_gs_fd, buf, (uint32_t)l) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  interface uart flush
 * @return status code
 *         - 0 success
 *         - 1 uart flush failed
 * @note   none
 */
uint8_t scd30_interface_uart_flush(void)
{
    return uart_flush(uart_gs_fd);
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void scd30_interface_delay_ms(uint32_t ms)
{
    usleep(1000 * ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void scd30_interface_debug_print(const char *const fmt, ...)
{
    char str[256];
    uint16_t len;
    va_list args;
    
    memset((char *)str, 0, sizeof(char) * 256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);
    
    len = strlen((char *)str);
    (void)printf((uint8_t *)str, len);
}
