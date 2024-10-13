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
 * @file      driver_scd30.c
 * @brief     driver scd30 source file
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

#include "driver_scd30.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Sensirion SCD30"        /**< chip name */
#define MANUFACTURER_NAME         "Sensirion"              /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        3.3f                     /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                     /**< chip max supply voltage */
#define MAX_CURRENT               75.0f                    /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                   /**< chip min operating temperature */
#define TEMPERATURE_MAX           70.0f                    /**< chip max operating temperature */
#define DRIVER_VERSION            1000                     /**< driver version */

/**
 * @brief chip address definition
 */
#define SCD30_ADDRESS             0xC2              /**< chip iic address */

/**
 * @brief chip command definition
 */
#define SCD30_IIC_COMMAND_START_PERIODIC_MEASUREMENT        0x0010U        /**< start periodic measurement */
#define SCD30_IIC_COMMAND_STOP_PERIODIC_MEASUREMENT         0x0104U        /**< stop periodic measurement */
#define SCD30_IIC_COMMAND_SET_MEASUREMENT_INTERVAL          0x4600U        /**< set measurement interval */
#define SCD30_IIC_COMMAND_GET_DATA_READY_STATUS             0x0202U        /**< get data ready status */
#define SCD30_IIC_COMMAND_READ_MEASUREMENT                  0x0300U        /**< read measurement */
#define SCD30_IIC_COMMAND_AUTO_SELF_CALIBRATION             0x5306U        /**< auto self calibration */
#define SCD30_IIC_COMMAND_SET_FORCED_RECALIBRATION          0x5204U        /**< set forced recalibration */
#define SCD30_IIC_COMMAND_SET_TEMPERATURE_OFFSET            0x5403U        /**< set temperature offset */
#define SCD30_IIC_COMMAND_SET_ALTITUDE                      0x5102U        /**< set altitude */
#define SCD30_IIC_COMMAND_READ_FIRMWARE_VERSION             0xD100U        /**< read firmware version */
#define SCD30_IIC_COMMAND_SOFT_RESET                        0xD304U        /**< soft reset */
#define SCD30_UART_ADDRESS_START_PERIODIC_MEASUREMENT       0x0036U        /**< start periodic measurement */
#define SCD30_UART_ADDRESS_STOP_PERIODIC_MEASUREMENT        0x0037U        /**< stop periodic measurement */
#define SCD30_UART_ADDRESS_SET_MEASUREMENT_INTERVAL         0x0025U        /**< set measurement interval */
#define SCD30_UART_ADDRESS_GET_DATA_READY_STATUS            0x0027U        /**< get data ready status */
#define SCD30_UART_ADDRESS_READ_MEASUREMENT                 0x0028U        /**< read measurement */
#define SCD30_UART_ADDRESS_AUTO_SELF_CALIBRATION            0x003AU        /**< auto self calibration */
#define SCD30_UART_ADDRESS_SET_FORCED_RECALIBRATION         0x0039U        /**< set forced recalibration */
#define SCD30_UART_ADDRESS_SET_TEMPERATURE_OFFSET           0x003BU        /**< set temperature offset */
#define SCD30_UART_ADDRESS_SET_ALTITUDE                     0x0038U        /**< set altitude */
#define SCD30_UART_ADDRESS_READ_FIRMWARE_VERSION            0x0020U        /**< read firmware version */
#define SCD30_UART_ADDRESS_SOFT_RESET                       0x0034U        /**< soft reset */

/**
 * @brief crc16 table high definition
 */
static const uint8_t gs_crc_table_hi[] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};

/**
 * @brief crc16 table low definition
 */
static const uint8_t gs_crc_table_lo[] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5,
    0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B,
    0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE,
    0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6,
    0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8,
    0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C,
    0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21,
    0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A,
    0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7,
    0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91, 0x51,
    0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98,
    0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D,
    0x4C, 0x8C, 0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};

/**
 * @brief     generate the crc16
 * @param[in] *data points to a data buffer
 * @param[in] count is the data length
 * @return    crc
 * @note      none
 */
static uint16_t a_scd30_generate_crc16(uint8_t *data, uint16_t count) 
{
    uint8_t crc_hi = 0xFF;
    uint8_t crc_lo = 0xFF;
    uint8_t i;

    while (count--)                                  /* count-- */
    {
        i = crc_lo ^ *data++;                        /* get index */
        crc_lo = crc_hi ^ gs_crc_table_hi[i];        /* get crc lo */
        crc_hi = gs_crc_table_lo[i];                 /* get crc hi */
    }

    return ((uint16_t)crc_hi << 8 | crc_lo);         /* return crc16 */
}

/**
 * @brief     generate the crc8
 * @param[in] *data points to a data buffer
 * @param[in] count is the data length
 * @return    crc
 * @note      none
 */
static uint8_t a_scd30_generate_crc8(uint8_t *data, uint8_t count) 
{
    uint8_t current_byte;
    uint8_t crc = 0xFF;
    uint8_t crc_bit;

    for (current_byte = 0; current_byte < count; ++current_byte)        /* calculate crc */
    {
        crc ^= (data[current_byte]);                                    /* xor data */
        for (crc_bit = 8; crc_bit > 0; --crc_bit)                       /* 8 bit */
        {
            if ((crc & 0x80) != 0)                                      /* if 7th bit is 1 */
            {
                crc = (crc << 1) ^ 0x31;                                /* xor */
            }
            else
            {
                crc = crc << 1;                                         /* left shift 1 */
            }
        }
    }
    
    return crc;                                                         /* return crc */
}

/**
 * @brief      read bytes
 * @param[in]  *handle points to an scd30 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @param[in]  delay_ms is the delay time in ms
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_scd30_iic_read(scd30_handle_t *handle, uint16_t reg, uint8_t *data, uint16_t len, uint16_t delay_ms)
{
    uint8_t buf[2];
    
    buf[0] = (reg >> 8) & 0xFF;                                                /* set msb */
    buf[1] = (reg >> 0) & 0xFF;                                                /* set lsb */
    if (handle->iic_write_cmd(SCD30_ADDRESS, (uint8_t *)buf, 2) != 0)          /* write data */
    {
        return 1;                                                              /* return error */
    }
    handle->delay_ms(delay_ms);                                                /* delay ms */
    if (handle->iic_read_cmd(SCD30_ADDRESS, (uint8_t *)data, len) != 0)        /* read data */
    {
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief     write bytes
 * @param[in] *handle points to an scd30 handle structure
 * @param[in] reg is the iic register address
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @param[in] delay_ms is the delay time in ms
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_scd30_iic_write(scd30_handle_t *handle, uint16_t reg, uint8_t *data, uint16_t len, uint16_t delay_ms)
{
    uint8_t buf[16];
    
    if (len > 14)                                                                  /* check length */
    {
        return 1;                                                                  /* return error */
    }
    buf[0] = (reg >> 8) & 0xFF;                                                    /* set msb */
    buf[1] = (reg >> 0) & 0xFF;                                                    /* set lsb */
    memcpy((uint8_t *)&buf[2], data, len);                                         /* copy data */
    if (handle->iic_write_cmd(SCD30_ADDRESS, (uint8_t *)buf, len + 2) != 0)        /* write data */
    {
        return 1;                                                                  /* return error */
    }
    handle->delay_ms(delay_ms);                                                    /* delay ms */
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief      write read bytes
 * @param[in]  *handle points to an scd30 handle structure
 * @param[in]  *input points to an input buffer
 * @param[in]  in_len is the input length
 * @param[in]  delay_ms is the delay time in ms
 * @param[out] *output points to an output buffer
 * @param[in]  out_len is the output length
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       none
 */
static uint8_t a_scd30_uart_write_read(scd30_handle_t *handle, uint8_t *input, uint16_t in_len,
                                       uint16_t delay_ms, uint8_t *output, uint16_t out_len)
{
    uint16_t len;
    
    if (handle->uart_flush() != 0)                     /* uart flush */
    {
        return 1;                                      /* return error */
    }
    if (handle->uart_write(input, in_len) != 0)        /* write data */
    {
        return 1;                                      /* return error */
    }
    handle->delay_ms(delay_ms);                        /* delay ms */
    len = handle->uart_read(output, out_len);          /* read data */
    if (len != out_len)                                /* check the result */
    {
        return 1;                                      /* return error */
    }
    
    return 0;                                          /* success return 0 */
}

/**
 * @brief      get data ready status
 * @param[in]  *handle points to an scd30 handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get data ready status failed
 * @note       none
 */
static uint8_t a_scd30_get_data_ready_status(scd30_handle_t *handle, uint16_t *status)
{
    uint8_t res;
    
    if (handle->iic_uart != 0)                                                                                      /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[7];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                                        /* set slave address */
        input_buf[1] = 0x03;                                                                                        /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_GET_DATA_READY_STATUS >> 8) & 0xFF;                                      /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_GET_DATA_READY_STATUS >> 0) & 0xFF;                                      /* set addr lsb */
        input_buf[4] = 0x00;                                                                                        /* set 0x00 */
        input_buf[5] = 0x01;                                                                                        /* set 0x01 */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                               /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                                         /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                                         /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 7);                                                                    /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 7);                  /* write read frame */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                                     /* write read failed */
           
            return 1;                                                                                               /* return error */
        }
        crc16 = ((uint16_t)out_buf[6] << 8 | (uint16_t)out_buf[5]);                                                 /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 5))                                                            /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                                       /* crc check error */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[0] != 0x61)                                                                                     /* check header */
        {
            handle->debug_print("scd30: header is invalid.\n");                                                     /* header is invalid */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[1] != 0x03)                                                                                     /* check code */
        {
            handle->debug_print("scd30: code is invalid.\n");                                                       /* code is invalid */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[2] != 0x02)                                                                                     /* check number */
        {
            handle->debug_print("scd30: number is invalid.\n");                                                     /* number is invalid */
           
            return 1;                                                                                               /* return error */
        }
        *status = ((uint16_t)out_buf[3] << 8) | out_buf[4];                                                         /* get status */
    }
    else                                                                                                            /* iic */
    {
        uint8_t buf[3];
        
        res = a_scd30_iic_read(handle, SCD30_IIC_COMMAND_GET_DATA_READY_STATUS, (uint8_t *)buf, 3, 20);             /* get data ready status command */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: get data ready status failed.\n");                                          /* get data ready status failed */
           
            return 1;                                                                                               /* return error */
        }
        if (buf[2] != a_scd30_generate_crc8((uint8_t *)buf, 2))                                                     /* check crc */
        {
            handle->debug_print("scd30: crc check failed.\n");                                                      /* crc check failed */
           
            return 1;                                                                                               /* return error */
        }
        *status = ((uint16_t)buf[0] << 8) | buf[1];                                                                 /* get status */
    }
    
    return 0;                                                                                                       /* success return 0 */
}

/**
 * @brief     set the chip interface
 * @param[in] *handle points to an scd30 handle structure
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t scd30_set_interface(scd30_handle_t *handle, scd30_interface_t interface)
{
    if (handle == NULL)                           /* check handle */
    {
        return 2;                                 /* return error */
    }
    
    handle->iic_uart = (uint8_t)interface;        /* set interface */
    
    return 0;                                     /* success return 0 */
}

/**
 * @brief      get the chip interface
 * @param[in]  *handle points to an scd30 handle structure
 * @param[out] *interface points to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t scd30_get_interface(scd30_handle_t *handle, scd30_interface_t *interface)
{
    if (handle == NULL)                                        /* check handle */
    {
        return 2;                                              /* return error */
    }
    
    *interface = (scd30_interface_t)(handle->iic_uart);        /* get interface */
    
    return 0;                                                  /* success return 0 */
}

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
uint8_t scd30_start_measurement_with_pressure_compensation(scd30_handle_t *handle, uint16_t mbar)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                             /* check handle */
    {
        return 2;                                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                                        /* check handle initialization */
    {
        return 3;                                                                                                   /* return error */
    }
    if ((mbar < 700) || (mbar > 1400))                                                                              /* check mbar range */
    {
        handle->debug_print("scd30: mbar is invalid.\n");                                                           /* mbar is invalid */
       
        return 4;                                                                                                   /* return error */
    }

    if (handle->iic_uart != 0)                                                                                      /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[6 + 2];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                                        /* set slave address */
        input_buf[1] = 0x06;                                                                                        /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_START_PERIODIC_MEASUREMENT >> 8) & 0xFF;                                 /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_START_PERIODIC_MEASUREMENT >> 0) & 0xFF;                                 /* set addr lsb */
        input_buf[4] = (mbar >> 8) & 0xFF;                                                                          /* set mbar msb */
        input_buf[5] = (mbar >> 0) & 0xFF;                                                                          /* set mbar lsb */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                               /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                                         /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                                         /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 8);                                                                    /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 8);                  /* write read frame */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                                     /* write read failed */
           
            return 1;                                                                                               /* return error */
        }
        crc16 = ((uint16_t)out_buf[7] << 8 | (uint16_t)out_buf[6]);                                                 /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 6))                                                            /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                                       /* crc check error */
           
            return 1;                                                                                               /* return error */
        }
        if (memcmp(input_buf, out_buf, 8) != 0)                                                                     /* check the response */
        {
            handle->debug_print("scd30: response check error.\n");                                                  /* response check error */
           
            return 1;                                                                                               /* return error */
        }
    }
    else                                                                                                            /* iic */
    {
        uint8_t buf[3];
        
        buf[0] = (mbar >> 8) & 0xFF;                                                                                /* set mbar msb */
        buf[1] = (mbar >> 0) & 0xFF;                                                                                /* set mbar lsb */
        buf[2] = a_scd30_generate_crc8((uint8_t *)buf, 2);                                                          /* generate crc */
        res = a_scd30_iic_write(handle, SCD30_IIC_COMMAND_START_PERIODIC_MEASUREMENT, (uint8_t *)buf, 3, 20);       /* start measurement command */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: start measurement failed.\n");                                              /* start measurement failed */
           
            return 1;                                                                                               /* return error */
        }
    }
    
    return 0;                                                                                                       /* success return 0 */
}

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
uint8_t scd30_start_measurement(scd30_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                             /* check handle */
    {
        return 2;                                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                                        /* check handle initialization */
    {
        return 3;                                                                                                   /* return error */
    }

    if (handle->iic_uart != 0)                                                                                      /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[6 + 2];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                                        /* set slave address */
        input_buf[1] = 0x06;                                                                                        /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_START_PERIODIC_MEASUREMENT >> 8) & 0xFF;                                 /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_START_PERIODIC_MEASUREMENT >> 0) & 0xFF;                                 /* set addr lsb */
        input_buf[4] = 0x00;                                                                                        /* set 0x00 */
        input_buf[5] = 0x00;                                                                                        /* set 0x00 */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                               /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                                         /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                                         /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 8);                                                                    /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 8);                  /* write read frame */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                                     /* write read failed */
           
            return 1;                                                                                               /* return error */
        }
        crc16 = ((uint16_t)out_buf[7] << 8 | (uint16_t)out_buf[6]);                                                 /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 6))                                                            /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                                       /* crc check error */
           
            return 1;                                                                                               /* return error */
        }
        if (memcmp(input_buf, out_buf, 8) != 0)                                                                     /* check the response */
        {
            handle->debug_print("scd30: response check error.\n");                                                  /* response check error */
           
            return 1;                                                                                               /* return error */
        }
    }
    else                                                                                                            /* iic */
    {
        uint8_t buf[3];
        
        buf[0] = 0x00;                                                                                              /* set 0x00 */
        buf[1] = 0x00;                                                                                              /* set 0x00 */
        buf[2] = a_scd30_generate_crc8((uint8_t *)buf, 2);                                                          /* generate crc */
        res = a_scd30_iic_write(handle, SCD30_IIC_COMMAND_START_PERIODIC_MEASUREMENT, (uint8_t *)buf, 3, 20);       /* start measurement command */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: start measurement failed.\n");                                              /* start measurement failed */
           
            return 1;                                                                                               /* return error */
        }
    }
    
    return 0;                                                                                                       /* success return 0 */
}

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
uint8_t scd30_stop_measurement(scd30_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }
    if (handle->inited != 1)                                                                              /* check handle initialization */
    {
        return 3;                                                                                         /* return error */
    }

    if (handle->iic_uart != 0)                                                                            /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[6 + 2];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                              /* set slave address */
        input_buf[1] = 0x06;                                                                              /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_STOP_PERIODIC_MEASUREMENT >> 8) & 0xFF;                        /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_STOP_PERIODIC_MEASUREMENT >> 0) & 0xFF;                        /* set addr lsb */
        input_buf[4] = 0x00;                                                                              /* set 0x00 */
        input_buf[5] = 0x01;                                                                              /* set 0x01 */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                     /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                               /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                               /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 8);                                                          /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 8);        /* write read frame */
        if (res != 0)                                                                                     /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                           /* write read failed */
           
            return 1;                                                                                     /* return error */
        }
        crc16 = ((uint16_t)out_buf[7] << 8 | (uint16_t)out_buf[6]);                                       /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 6))                                                  /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                             /* crc check error */
           
            return 1;                                                                                     /* return error */
        }
        if (memcmp(input_buf, out_buf, 8) != 0)                                                           /* check the response */
        {
            handle->debug_print("scd30: response check error.\n");                                        /* response check error */
           
            return 1;                                                                                     /* return error */
        }
    }
    else                                                                                                  /* iic */
    {
        res = a_scd30_iic_write(handle, SCD30_IIC_COMMAND_STOP_PERIODIC_MEASUREMENT, NULL, 0, 20);        /* stop measurement command */
        if (res != 0)                                                                                     /* check result */
        {
            handle->debug_print("scd30: stop measurement failed.\n");                                     /* stop measurement failed */
           
            return 1;                                                                                     /* return error */
        }
    }
    
    return 0;                                                                                             /* success return 0 */
}

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
uint8_t scd30_set_measurement_interval(scd30_handle_t *handle, uint16_t second)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                             /* check handle */
    {
        return 2;                                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                                        /* check handle initialization */
    {
        return 3;                                                                                                   /* return error */
    }
    if ((second < 2) || (second > 1800))                                                                            /* check second range */
    {
        handle->debug_print("scd30: second is invalid.\n");                                                         /* second is invalid */
       
        return 4;                                                                                                   /* return error */
    }

    if (handle->iic_uart != 0)                                                                                      /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[6 + 2];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                                        /* set slave address */
        input_buf[1] = 0x06;                                                                                        /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_SET_MEASUREMENT_INTERVAL >> 8) & 0xFF;                                   /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_SET_MEASUREMENT_INTERVAL >> 0) & 0xFF;                                   /* set addr lsb */
        input_buf[4] = (second >> 8) & 0xFF;                                                                        /* set second msb */
        input_buf[5] = (second >> 0) & 0xFF;                                                                        /* set second lsb */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                               /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                                         /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                                         /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 8);                                                                    /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 8);                  /* write read frame */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                                     /* write read failed */
           
            return 1;                                                                                               /* return error */
        }
        crc16 = ((uint16_t)out_buf[7] << 8 | (uint16_t)out_buf[6]);                                                 /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 6))                                                            /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                                       /* crc check error */
           
            return 1;                                                                                               /* return error */
        }
        if (memcmp(input_buf, out_buf, 8) != 0)                                                                     /* check the response */
        {
            handle->debug_print("scd30: response check error.\n");                                                  /* response check error */
           
            return 1;                                                                                               /* return error */
        }
    }
    else                                                                                                            /* iic */
    {
        uint8_t buf[3];
        
        buf[0] = (second >> 8) & 0xFF;                                                                              /* set second msb */
        buf[1] = (second >> 0) & 0xFF;                                                                              /* set second lsb */
        buf[2] = a_scd30_generate_crc8((uint8_t *)buf, 2);                                                          /* generate crc */
        res = a_scd30_iic_write(handle, SCD30_IIC_COMMAND_SET_MEASUREMENT_INTERVAL, (uint8_t *)buf, 3, 20);         /* set measurement interval command */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: set measurement interval failed.\n");                                       /* set measurement interval failed */
           
            return 1;                                                                                               /* return error */
        }
    }
    
    return 0;                                                                                                       /* success return 0 */
}

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
uint8_t scd30_get_measurement_interval(scd30_handle_t *handle, uint16_t *second)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                             /* check handle */
    {
        return 2;                                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                                        /* check handle initialization */
    {
        return 3;                                                                                                   /* return error */
    }

    if (handle->iic_uart != 0)                                                                                      /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[7];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                                        /* set slave address */
        input_buf[1] = 0x03;                                                                                        /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_SET_MEASUREMENT_INTERVAL >> 8) & 0xFF;                                   /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_SET_MEASUREMENT_INTERVAL >> 0) & 0xFF;                                   /* set addr lsb */
        input_buf[4] = 0x00;                                                                                        /* set 0x00 */
        input_buf[5] = 0x01;                                                                                        /* set 0x01 */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                               /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                                         /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                                         /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 7);                                                                    /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 7);                  /* write read frame */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                                     /* write read failed */
           
            return 1;                                                                                               /* return error */
        }
        crc16 = ((uint16_t)out_buf[6] << 8 | (uint16_t)out_buf[5]);                                                 /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 5))                                                            /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                                       /* crc check error */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[0] != 0x61)                                                                                     /* check header */
        {
            handle->debug_print("scd30: header is invalid.\n");                                                     /* header is invalid */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[1] != 0x03)                                                                                     /* check code */
        {
            handle->debug_print("scd30: code is invalid.\n");                                                       /* code is invalid */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[2] != 0x02)                                                                                     /* check number */
        {
            handle->debug_print("scd30: number is invalid.\n");                                                     /* number is invalid */
           
            return 1;                                                                                               /* return error */
        }
        *second = ((uint16_t)out_buf[3] << 8) | out_buf[4];                                                         /* get second */
    }
    else                                                                                                            /* iic */
    {
        uint8_t buf[3];
        
        res = a_scd30_iic_read(handle, SCD30_IIC_COMMAND_SET_MEASUREMENT_INTERVAL, (uint8_t *)buf, 3, 20);          /* get measurement interval command */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: get measurement interval failed.\n");                                       /* get measurement interval failed */
           
            return 1;                                                                                               /* return error */
        }
        if (buf[2] != a_scd30_generate_crc8((uint8_t *)buf, 2))                                                     /* check crc */
        {
            handle->debug_print("scd30: crc check failed.\n");                                                      /* crc check failed */
           
            return 1;                                                                                               /* return error */
        }
        *second = ((uint16_t)buf[0] << 8) | buf[1];                                                                 /* get second */
    }
    
    return 0;                                                                                                       /* success return 0 */
}

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
uint8_t scd30_get_data_ready_status(scd30_handle_t *handle, uint16_t *status)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                             /* check handle */
    {
        return 2;                                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                                        /* check handle initialization */
    {
        return 3;                                                                                                   /* return error */
    }

    if (handle->iic_uart != 0)                                                                                      /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[7];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                                        /* set slave address */
        input_buf[1] = 0x03;                                                                                        /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_GET_DATA_READY_STATUS >> 8) & 0xFF;                                      /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_GET_DATA_READY_STATUS >> 0) & 0xFF;                                      /* set addr lsb */
        input_buf[4] = 0x00;                                                                                        /* set 0x00 */
        input_buf[5] = 0x01;                                                                                        /* set 0x01 */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                               /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                                         /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                                         /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 7);                                                                    /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 7);                  /* write read frame */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                                     /* write read failed */
           
            return 1;                                                                                               /* return error */
        }
        crc16 = ((uint16_t)out_buf[6] << 8 | (uint16_t)out_buf[5]);                                                 /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 5))                                                            /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                                       /* crc check error */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[0] != 0x61)                                                                                     /* check header */
        {
            handle->debug_print("scd30: header is invalid.\n");                                                     /* header is invalid */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[1] != 0x03)                                                                                     /* check code */
        {
            handle->debug_print("scd30: code is invalid.\n");                                                       /* code is invalid */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[2] != 0x02)                                                                                     /* check number */
        {
            handle->debug_print("scd30: number is invalid.\n");                                                     /* number is invalid */
           
            return 1;                                                                                               /* return error */
        }
        *status = ((uint16_t)out_buf[3] << 8) | out_buf[4];                                                         /* get status */
    }
    else                                                                                                            /* iic */
    {
        uint8_t buf[3];
        
        res = a_scd30_iic_read(handle, SCD30_IIC_COMMAND_GET_DATA_READY_STATUS, (uint8_t *)buf, 3, 20);             /* get data ready status command */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: get data ready status failed.\n");                                          /* get data ready status failed */
           
            return 1;                                                                                               /* return error */
        }
        if (buf[2] != a_scd30_generate_crc8((uint8_t *)buf, 2))                                                     /* check crc */
        {
            handle->debug_print("scd30: crc check failed.\n");                                                      /* crc check failed */
           
            return 1;                                                                                               /* return error */
        }
        *status = ((uint16_t)buf[0] << 8) | buf[1];                                                                 /* get status */
    }
    
    return 0;                                                                                                       /* success return 0 */
}

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
uint8_t scd30_set_automatic_self_calibration(scd30_handle_t *handle, scd30_bool_t enable)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                             /* check handle */
    {
        return 2;                                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                                        /* check handle initialization */
    {
        return 3;                                                                                                   /* return error */
    }

    if (handle->iic_uart != 0)                                                                                      /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[6 + 2];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                                        /* set slave address */
        input_buf[1] = 0x06;                                                                                        /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_AUTO_SELF_CALIBRATION >> 8) & 0xFF;                                      /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_AUTO_SELF_CALIBRATION >> 0) & 0xFF;                                      /* set addr lsb */
        input_buf[4] = ((uint16_t)enable >> 8) & 0xFF;                                                              /* set bool msb */
        input_buf[5] = ((uint16_t)enable >> 0) & 0xFF;                                                              /* set bool lsb */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                               /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                                         /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                                         /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 8);                                                                    /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 8);                  /* write read frame */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                                     /* write read failed */
           
            return 1;                                                                                               /* return error */
        }
        crc16 = ((uint16_t)out_buf[7] << 8 | (uint16_t)out_buf[6]);                                                 /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 6))                                                            /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                                       /* crc check error */
           
            return 1;                                                                                               /* return error */
        }
        if (memcmp(input_buf, out_buf, 8) != 0)                                                                     /* check the response */
        {
            handle->debug_print("scd30: response check error.\n");                                                  /* response check error */
           
            return 1;                                                                                               /* return error */
        }
    }
    else                                                                                                            /* iic */
    {
        uint8_t buf[3];
        
        buf[0] = ((uint16_t)enable >> 8) & 0xFF;                                                                    /* set bool msb */
        buf[1] = ((uint16_t)enable >> 0) & 0xFF;                                                                    /* set bool lsb */
        buf[2] = a_scd30_generate_crc8((uint8_t *)buf, 2);                                                          /* generate crc */
        res = a_scd30_iic_write(handle, SCD30_IIC_COMMAND_AUTO_SELF_CALIBRATION, (uint8_t *)buf, 3, 20);            /* set auto self calibration command */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: set auto self calibration failed.\n");                                      /* set auto self calibration failed */
           
            return 1;                                                                                               /* return error */
        }
    }
    
    return 0;                                                                                                       /* success return 0 */
}

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
uint8_t scd30_get_automatic_self_calibration(scd30_handle_t *handle, scd30_bool_t *enable)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                             /* check handle */
    {
        return 2;                                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                                        /* check handle initialization */
    {
        return 3;                                                                                                   /* return error */
    }

    if (handle->iic_uart != 0)                                                                                      /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[7];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                                        /* set slave address */
        input_buf[1] = 0x03;                                                                                        /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_AUTO_SELF_CALIBRATION >> 8) & 0xFF;                                      /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_AUTO_SELF_CALIBRATION >> 0) & 0xFF;                                      /* set addr lsb */
        input_buf[4] = 0x00;                                                                                        /* set 0x00 */
        input_buf[5] = 0x01;                                                                                        /* set 0x01 */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                               /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                                         /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                                         /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 7);                                                                    /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 7);                  /* write read frame */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                                     /* write read failed */
           
            return 1;                                                                                               /* return error */
        }
        crc16 = ((uint16_t)out_buf[6] << 8 | (uint16_t)out_buf[5]);                                                 /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 5))                                                            /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                                       /* crc check error */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[0] != 0x61)                                                                                     /* check header */
        {
            handle->debug_print("scd30: header is invalid.\n");                                                     /* header is invalid */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[1] != 0x03)                                                                                     /* check code */
        {
            handle->debug_print("scd30: code is invalid.\n");                                                       /* code is invalid */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[2] != 0x02)                                                                                     /* check number */
        {
            handle->debug_print("scd30: number is invalid.\n");                                                     /* number is invalid */
           
            return 1;                                                                                               /* return error */
        }
        *enable = (scd30_bool_t)(((uint16_t)out_buf[3] << 8) | out_buf[4]);                                         /* get bool */
    }
    else                                                                                                            /* iic */
    {
        uint8_t buf[3];
        
        res = a_scd30_iic_read(handle, SCD30_IIC_COMMAND_AUTO_SELF_CALIBRATION, (uint8_t *)buf, 3, 20);             /* get auto self calibration command */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: get auto self calibration failed.\n");                                      /* get auto self calibration failed */
           
            return 1;                                                                                               /* return error */
        }
        if (buf[2] != a_scd30_generate_crc8((uint8_t *)buf, 2))                                                     /* check crc */
        {
            handle->debug_print("scd30: crc check failed.\n");                                                      /* crc check failed */
           
            return 1;                                                                                               /* return error */
        }
        *enable = (scd30_bool_t)(((uint16_t)buf[0] << 8) | buf[1]);                                                 /* get bool */
    }
    
    return 0;                                                                                                       /* success return 0 */
}

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
uint8_t scd30_set_forced_recalibration(scd30_handle_t *handle, uint16_t co2_ppm)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                             /* check handle */
    {
        return 2;                                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                                        /* check handle initialization */
    {
        return 3;                                                                                                   /* return error */
    }
    if ((co2_ppm < 400) || (co2_ppm > 2000))                                                                        /* check co2_ppm range */
    {
        handle->debug_print("scd30: co2_ppm is invalid.\n");                                                        /* co2_ppm is invalid */
       
        return 4;                                                                                                   /* return error */
    }
    
    if (handle->iic_uart != 0)                                                                                      /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[6 + 2];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                                        /* set slave address */
        input_buf[1] = 0x06;                                                                                        /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_SET_FORCED_RECALIBRATION >> 8) & 0xFF;                                   /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_SET_FORCED_RECALIBRATION >> 0) & 0xFF;                                   /* set addr lsb */
        input_buf[4] = (co2_ppm >> 8) & 0xFF;                                                                       /* set ppm msb */
        input_buf[5] = (co2_ppm >> 0) & 0xFF;                                                                       /* set ppm lsb */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                               /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                                         /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                                         /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 8);                                                                    /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 8);                  /* write read frame */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                                     /* write read failed */
           
            return 1;                                                                                               /* return error */
        }
        crc16 = ((uint16_t)out_buf[7] << 8 | (uint16_t)out_buf[6]);                                                 /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 6))                                                            /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                                       /* crc check error */
           
            return 1;                                                                                               /* return error */
        }
        if (memcmp(input_buf, out_buf, 8) != 0)                                                                     /* check the response */
        {
            handle->debug_print("scd30: response check error.\n");                                                  /* response check error */
           
            return 1;                                                                                               /* return error */
        }
    }
    else                                                                                                            /* iic */
    {
        uint8_t buf[3];
        
        buf[0] = (co2_ppm >> 8) & 0xFF;                                                                             /* set ppm msb */
        buf[1] = (co2_ppm >> 0) & 0xFF;                                                                             /* set ppm lsb */
        buf[2] = a_scd30_generate_crc8((uint8_t *)buf, 2);                                                          /* generate crc */
        res = a_scd30_iic_write(handle, SCD30_IIC_COMMAND_SET_FORCED_RECALIBRATION, (uint8_t *)buf, 3, 20);         /* set forced recalibration command */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: set forced recalibration failed.\n");                                       /* set forced recalibration failed */
           
            return 1;                                                                                               /* return error */
        }
    }
    
    return 0;                                                                                                       /* success return 0 */
}

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
uint8_t scd30_get_forced_recalibration(scd30_handle_t *handle, uint16_t *co2_ppm)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                             /* check handle */
    {
        return 2;                                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                                        /* check handle initialization */
    {
        return 3;                                                                                                   /* return error */
    }

    if (handle->iic_uart != 0)                                                                                      /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[7];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                                        /* set slave address */
        input_buf[1] = 0x03;                                                                                        /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_SET_FORCED_RECALIBRATION >> 8) & 0xFF;                                   /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_SET_FORCED_RECALIBRATION >> 0) & 0xFF;                                   /* set addr lsb */
        input_buf[4] = 0x00;                                                                                        /* set 0x00 */
        input_buf[5] = 0x01;                                                                                        /* set 0x01 */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                               /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                                         /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                                         /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 7);                                                                    /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 7);                  /* write read frame */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                                     /* write read failed */
           
            return 1;                                                                                               /* return error */
        }
        crc16 = ((uint16_t)out_buf[6] << 8 | (uint16_t)out_buf[5]);                                                 /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 5))                                                            /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                                       /* crc check error */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[0] != 0x61)                                                                                     /* check header */
        {
            handle->debug_print("scd30: header is invalid.\n");                                                     /* header is invalid */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[1] != 0x03)                                                                                     /* check code */
        {
            handle->debug_print("scd30: code is invalid.\n");                                                       /* code is invalid */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[2] != 0x02)                                                                                     /* check number */
        {
            handle->debug_print("scd30: number is invalid.\n");                                                     /* number is invalid */
           
            return 1;                                                                                               /* return error */
        }
        *co2_ppm = (uint16_t)(((uint16_t)out_buf[3] << 8) | out_buf[4]);                                            /* get co2_ppm */
    }
    else                                                                                                            /* iic */
    {
        uint8_t buf[3];
        
        res = a_scd30_iic_read(handle, SCD30_IIC_COMMAND_SET_FORCED_RECALIBRATION, (uint8_t *)buf, 3, 20);          /* set forced recalibration command */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: set forced recalibration failed.\n");                                       /* set forced recalibration failed */
           
            return 1;                                                                                               /* return error */
        }
        if (buf[2] != a_scd30_generate_crc8((uint8_t *)buf, 2))                                                     /* check crc */
        {
            handle->debug_print("scd30: crc check failed.\n");                                                      /* crc check failed */
           
            return 1;                                                                                               /* return error */
        }
        *co2_ppm = (uint16_t)(((uint16_t)buf[0] << 8) | buf[1]);                                                    /* get co2_ppm */
    }
    
    return 0;                                                                                                       /* success return 0 */
}

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
uint8_t scd30_set_temperature_offset(scd30_handle_t *handle, uint16_t deg)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                             /* check handle */
    {
        return 2;                                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                                        /* check handle initialization */
    {
        return 3;                                                                                                   /* return error */
    }
    
    if (handle->iic_uart != 0)                                                                                      /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[6 + 2];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                                        /* set slave address */
        input_buf[1] = 0x06;                                                                                        /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_SET_TEMPERATURE_OFFSET >> 8) & 0xFF;                                     /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_SET_TEMPERATURE_OFFSET >> 0) & 0xFF;                                     /* set addr lsb */
        input_buf[4] = (deg >> 8) & 0xFF;                                                                           /* set deg msb */
        input_buf[5] = (deg >> 0) & 0xFF;                                                                           /* set deg lsb */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                               /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                                         /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                                         /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 8);                                                                    /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 8);                  /* write read frame */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                                     /* write read failed */
           
            return 1;                                                                                               /* return error */
        }
        crc16 = ((uint16_t)out_buf[7] << 8 | (uint16_t)out_buf[6]);                                                 /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 6))                                                            /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                                       /* crc check error */
           
            return 1;                                                                                               /* return error */
        }
        if (memcmp(input_buf, out_buf, 8) != 0)                                                                     /* check the response */
        {
            handle->debug_print("scd30: response check error.\n");                                                  /* response check error */
           
            return 1;                                                                                               /* return error */
        }
    }
    else                                                                                                            /* iic */
    {
        uint8_t buf[3];
        
        buf[0] = (deg >> 8) & 0xFF;                                                                                 /* set deg msb */
        buf[1] = (deg >> 0) & 0xFF;                                                                                 /* set deg lsb */
        buf[2] = a_scd30_generate_crc8((uint8_t *)buf, 2);                                                          /* generate crc */
        res = a_scd30_iic_write(handle, SCD30_IIC_COMMAND_SET_TEMPERATURE_OFFSET, (uint8_t *)buf, 3, 20);           /* set temperature offset command */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: set temperature offset failed.\n");                                         /* set temperature offset failed */
           
            return 1;                                                                                               /* return error */
        }
    }
    
    return 0;                                                                                                       /* success return 0 */
}

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
uint8_t scd30_get_temperature_offset(scd30_handle_t *handle, uint16_t *deg)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                             /* check handle */
    {
        return 2;                                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                                        /* check handle initialization */
    {
        return 3;                                                                                                   /* return error */
    }

    if (handle->iic_uart != 0)                                                                                      /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[7];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                                        /* set slave address */
        input_buf[1] = 0x03;                                                                                        /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_SET_TEMPERATURE_OFFSET >> 8) & 0xFF;                                     /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_SET_TEMPERATURE_OFFSET >> 0) & 0xFF;                                     /* set addr lsb */
        input_buf[4] = 0x00;                                                                                        /* set 0x00 */
        input_buf[5] = 0x01;                                                                                        /* set 0x01 */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                               /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                                         /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                                         /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 7);                                                                    /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 7);                  /* write read frame */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                                     /* write read failed */
           
            return 1;                                                                                               /* return error */
        }
        crc16 = ((uint16_t)out_buf[6] << 8 | (uint16_t)out_buf[5]);                                                 /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 5))                                                            /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                                       /* crc check error */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[0] != 0x61)                                                                                     /* check header */
        {
            handle->debug_print("scd30: header is invalid.\n");                                                     /* header is invalid */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[1] != 0x03)                                                                                     /* check code */
        {
            handle->debug_print("scd30: code is invalid.\n");                                                       /* code is invalid */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[2] != 0x02)                                                                                     /* check number */
        {
            handle->debug_print("scd30: number is invalid.\n");                                                     /* number is invalid */
           
            return 1;                                                                                               /* return error */
        }
        *deg = (uint16_t)(((uint16_t)out_buf[3] << 8) | out_buf[4]);                                                /* get deg */
    }
    else                                                                                                            /* iic */
    {
        uint8_t buf[3];
        
        res = a_scd30_iic_read(handle, SCD30_IIC_COMMAND_SET_TEMPERATURE_OFFSET, (uint8_t *)buf, 3, 20);            /* set temperature offset command */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: set temperature offset failed.\n");                                         /* set temperature offset failed */
           
            return 1;                                                                                               /* return error */
        }
        if (buf[2] != a_scd30_generate_crc8((uint8_t *)buf, 2))                                                     /* check crc */
        {
            handle->debug_print("scd30: crc check failed.\n");                                                      /* crc check failed */
           
            return 1;                                                                                               /* return error */
        }
        *deg = (uint16_t)(((uint16_t)buf[0] << 8) | buf[1]);                                                        /* get deg */
    }
    
    return 0;                                                                                                       /* success return 0 */
}

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
uint8_t scd30_temperature_offset_convert_to_register(scd30_handle_t *handle, float deg, uint16_t *reg)
{
    if (handle == NULL)                   /* check handle */
    {
        return 2;                         /* return error */
    }
    if (handle->inited != 1)              /* check handle initialization */
    {
        return 3;                         /* return error */
    }
    
    *reg = (uint8_t)(deg * 100.0f);       /* convert real data to register data */
    
    return 0;                             /* success return 0 */
}

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
uint8_t scd30_temperature_offset_convert_to_data(scd30_handle_t *handle, uint16_t reg, float *deg)
{
    if (handle == NULL)                 /* check handle */
    {
        return 2;                       /* return error */
    }
    if (handle->inited != 1)            /* check handle initialization */
    {
        return 3;                       /* return error */
    }
    
    *deg = (float)(reg) / 100.0f;       /* convert raw data to real data */
    
    return 0;                           /* success return 0 */
}

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
uint8_t scd30_set_altitude_compensation(scd30_handle_t *handle, uint16_t m)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                             /* check handle */
    {
        return 2;                                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                                        /* check handle initialization */
    {
        return 3;                                                                                                   /* return error */
    }
    
    if (handle->iic_uart != 0)                                                                                      /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[6 + 2];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                                        /* set slave address */
        input_buf[1] = 0x06;                                                                                        /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_SET_ALTITUDE >> 8) & 0xFF;                                               /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_SET_ALTITUDE >> 0) & 0xFF;                                               /* set addr lsb */
        input_buf[4] = (m >> 8) & 0xFF;                                                                             /* set m msb */
        input_buf[5] = (m >> 0) & 0xFF;                                                                             /* set m lsb */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                               /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                                         /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                                         /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 8);                                                                    /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 8);                  /* write read frame */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                                     /* write read failed */
           
            return 1;                                                                                               /* return error */
        }
        crc16 = ((uint16_t)out_buf[7] << 8 | (uint16_t)out_buf[6]);                                                 /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 6))                                                            /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                                       /* crc check error */
           
            return 1;                                                                                               /* return error */
        }
        if (memcmp(input_buf, out_buf, 8) != 0)                                                                     /* check the response */
        {
            handle->debug_print("scd30: response check error.\n");                                                  /* response check error */
           
            return 1;                                                                                               /* return error */
        }
    }
    else                                                                                                            /* iic */
    {
        uint8_t buf[3];
        
        buf[0] = (m >> 8) & 0xFF;                                                                                   /* set m msb */
        buf[1] = (m >> 0) & 0xFF;                                                                                   /* set m lsb */
        buf[2] = a_scd30_generate_crc8((uint8_t *)buf, 2);                                                          /* generate crc */
        res = a_scd30_iic_write(handle, SCD30_IIC_COMMAND_SET_ALTITUDE, (uint8_t *)buf, 3, 20);                     /* set altitude command */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: set altitude failed.\n");                                                   /* set altitude failed */
           
            return 1;                                                                                               /* return error */
        }
    }
    
    return 0;                                                                                                       /* success return 0 */
}

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
uint8_t scd30_get_altitude_compensation(scd30_handle_t *handle, uint16_t *m)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                             /* check handle */
    {
        return 2;                                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                                        /* check handle initialization */
    {
        return 3;                                                                                                   /* return error */
    }

    if (handle->iic_uart != 0)                                                                                      /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[7];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                                        /* set slave address */
        input_buf[1] = 0x03;                                                                                        /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_SET_ALTITUDE >> 8) & 0xFF;                                               /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_SET_ALTITUDE >> 0) & 0xFF;                                               /* set addr lsb */
        input_buf[4] = 0x00;                                                                                        /* set 0x00 */
        input_buf[5] = 0x01;                                                                                        /* set 0x01 */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                               /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                                         /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                                         /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 7);                                                                    /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 7);                  /* write read frame */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                                     /* write read failed */
           
            return 1;                                                                                               /* return error */
        }
        crc16 = ((uint16_t)out_buf[6] << 8 | (uint16_t)out_buf[5]);                                                 /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 5))                                                            /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                                       /* crc check error */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[0] != 0x61)                                                                                     /* check header */
        {
            handle->debug_print("scd30: header is invalid.\n");                                                     /* header is invalid */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[1] != 0x03)                                                                                     /* check code */
        {
            handle->debug_print("scd30: code is invalid.\n");                                                       /* code is invalid */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[2] != 0x02)                                                                                     /* check number */
        {
            handle->debug_print("scd30: number is invalid.\n");                                                     /* number is invalid */
           
            return 1;                                                                                               /* return error */
        }
        *m = (uint16_t)(((uint16_t)out_buf[3] << 8) | out_buf[4]);                                                  /* get m */
    }
    else                                                                                                            /* iic */
    {
        uint8_t buf[3];
        
        res = a_scd30_iic_read(handle, SCD30_IIC_COMMAND_SET_ALTITUDE, (uint8_t *)buf, 3, 20);                      /* set altitude command */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: set altitude failed.\n");                                                   /* set altitude failed */
           
            return 1;                                                                                               /* return error */
        }
        if (buf[2] != a_scd30_generate_crc8((uint8_t *)buf, 2))                                                     /* check crc */
        {
            handle->debug_print("scd30: crc check failed.\n");                                                      /* crc check failed */
           
            return 1;                                                                                               /* return error */
        }
        *m = (uint16_t)(((uint16_t)buf[0] << 8) | buf[1]);                                                          /* get m */
    }
    
    return 0;                                                                                                       /* success return 0 */
}

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
uint8_t scd30_get_firmware_version(scd30_handle_t *handle, uint16_t *version)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                             /* check handle */
    {
        return 2;                                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                                        /* check handle initialization */
    {
        return 3;                                                                                                   /* return error */
    }

    if (handle->iic_uart != 0)                                                                                      /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[7];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                                        /* set slave address */
        input_buf[1] = 0x03;                                                                                        /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_READ_FIRMWARE_VERSION >> 8) & 0xFF;                                      /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_READ_FIRMWARE_VERSION >> 0) & 0xFF;                                      /* set addr lsb */
        input_buf[4] = 0x00;                                                                                        /* set 0x00 */
        input_buf[5] = 0x01;                                                                                        /* set 0x01 */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                               /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                                         /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                                         /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 7);                                                                    /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 7);                  /* write read frame */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                                     /* write read failed */
           
            return 1;                                                                                               /* return error */
        }
        crc16 = ((uint16_t)out_buf[6] << 8 | (uint16_t)out_buf[5]);                                                 /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 5))                                                            /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                                       /* crc check error */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[0] != 0x61)                                                                                     /* check header */
        {
            handle->debug_print("scd30: header is invalid.\n");                                                     /* header is invalid */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[1] != 0x03)                                                                                     /* check code */
        {
            handle->debug_print("scd30: code is invalid.\n");                                                       /* code is invalid */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[2] != 0x02)                                                                                     /* check number */
        {
            handle->debug_print("scd30: number is invalid.\n");                                                     /* number is invalid */
           
            return 1;                                                                                               /* return error */
        }
        *version = (uint16_t)(((uint16_t)out_buf[3] << 8) | out_buf[4]);                                            /* get version */
    }
    else                                                                                                            /* iic */
    {
        uint8_t buf[3];
        
        res = a_scd30_iic_read(handle, SCD30_IIC_COMMAND_READ_FIRMWARE_VERSION, (uint8_t *)buf, 3, 20);             /* read firmware version command */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: read firmware version failed.\n");                                          /* read firmware version failed */
           
            return 1;                                                                                               /* return error */
        }
        if (buf[2] != a_scd30_generate_crc8((uint8_t *)buf, 2))                                                     /* check crc */
        {
            handle->debug_print("scd30: crc check failed.\n");                                                      /* crc check failed */
           
            return 1;                                                                                               /* return error */
        }
        *version = (uint16_t)(((uint16_t)buf[0] << 8) | buf[1]);                                                    /* get version */
    }
    
    return 0;                                                                                                       /* success return 0 */
}

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
uint8_t scd30_soft_reset(scd30_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                             /* check handle */
    {
        return 2;                                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                                        /* check handle initialization */
    {
        return 3;                                                                                                   /* return error */
    }
    
    if (handle->iic_uart != 0)                                                                                      /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[6 + 2];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                                        /* set slave address */
        input_buf[1] = 0x06;                                                                                        /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_SOFT_RESET >> 8) & 0xFF;                                                 /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_SOFT_RESET >> 0) & 0xFF;                                                 /* set addr lsb */
        input_buf[4] = 0x00;                                                                                        /* set 0x00 */
        input_buf[5] = 0x01;                                                                                        /* set 0x01 */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                               /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                                         /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                                         /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 8);                                                                    /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 8);                  /* write read frame */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                                     /* write read failed */
           
            return 1;                                                                                               /* return error */
        }
        crc16 = ((uint16_t)out_buf[7] << 8 | (uint16_t)out_buf[6]);                                                 /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 6))                                                            /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                                       /* crc check error */
           
            return 1;                                                                                               /* return error */
        }
        if (memcmp(input_buf, out_buf, 8) != 0)                                                                     /* check the response */
        {
            handle->debug_print("scd30: response check error.\n");                                                  /* response check error */
           
            return 1;                                                                                               /* return error */
        }
    }
    else                                                                                                            /* iic */
    {
        res = a_scd30_iic_write(handle, SCD30_IIC_COMMAND_SOFT_RESET, (uint8_t *)NULL, 0, 20);                      /* soft reset command */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: soft reset failed.\n");                                                     /* soft reset failed */
           
            return 1;                                                                                               /* return error */
        }
    }
    handle->delay_ms(100);                                                                                          /* delay 100ms */
    
    return 0;                                                                                                       /* success return 0 */
}

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
uint8_t scd30_read(scd30_handle_t *handle, scd30_data_t *data)
{
    union float_u
    {
        float f;
        uint32_t i;
    };
    uint8_t res;
    uint16_t status;
    union float_u prev;
    uint32_t timeout = 5000;
    
    if ((handle == NULL) || (data == NULL))                                                                         /* check handle */
    {
        return 2;                                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                                        /* check handle initialization */
    {
        return 3;                                                                                                   /* return error */
    }

    while (timeout != 0)                                                                                            /* check timeout */
    {
        res = a_scd30_get_data_ready_status(handle, &status);                                                       /* get data ready status */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: get data ready status failed.\n");                                          /* get data ready status failed */
           
            return 1;                                                                                               /* return error */
        }
        if (status == 1)                                                                                            /* check status */
        {
            break;                                                                                                  /* break */
        }
        timeout--;                                                                                                  /* timeout-- */
        handle->delay_ms(1);                                                                                        /* delay 1ms */
    }
    if (timeout == 0)                                                                                               /* check timeout */
    {
        handle->debug_print("scd30: read timeout.\n");                                                              /* read timeout */
       
        return 4;                                                                                                   /* return error */
    }
    
    if (handle->iic_uart != 0)                                                                                      /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[17];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                                        /* set slave address */
        input_buf[1] = 0x03;                                                                                        /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_READ_MEASUREMENT >> 8) & 0xFF;                                           /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_READ_MEASUREMENT >> 0) & 0xFF;                                           /* set addr lsb */
        input_buf[4] = 0x00;                                                                                        /* set 0x00 */
        input_buf[5] = 0x06;                                                                                        /* set 0x01 */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                               /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                                         /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                                         /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 17);                                                                   /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 17);                 /* write read frame */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                                     /* write read failed */
           
            return 1;                                                                                               /* return error */
        }
        crc16 = ((uint16_t)out_buf[16] << 8 | (uint16_t)out_buf[15]);                                               /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 15))                                                           /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                                       /* crc check error */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[0] != 0x61)                                                                                     /* check header */
        {
            handle->debug_print("scd30: header is invalid.\n");                                                     /* header is invalid */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[1] != 0x03)                                                                                     /* check code */
        {
            handle->debug_print("scd30: code is invalid.\n");                                                       /* code is invalid */
           
            return 1;                                                                                               /* return error */
        }
        if (out_buf[2] != 0x0C)                                                                                     /* check number */
        {
            handle->debug_print("scd30: number is invalid.\n");                                                     /* number is invalid */
           
            return 1;                                                                                               /* return error */
        }
        prev.i = ((uint32_t)out_buf[3] << 24) | ((uint32_t)out_buf[4] << 16) |
                 ((uint32_t)out_buf[5] << 8) | ((uint32_t)out_buf[6] << 0);                                         /* integer to float */
        data->co2_raw = prev.i;                                                                                     /* save raw */
        data->co2_ppm = prev.f;                                                                                     /* save float */
        prev.i = ((uint32_t)out_buf[7] << 24) | ((uint32_t)out_buf[8] << 16) |
                 ((uint32_t)out_buf[9] << 8) | ((uint32_t)out_buf[10] << 0);                                        /* integer to float */
        data->temperature_raw = prev.i;                                                                             /* save raw */
        data->temperature_deg = prev.f;                                                                             /* save float */
        prev.i = ((uint32_t)out_buf[11] << 24) | ((uint32_t)out_buf[12] << 16) |
                 ((uint32_t)out_buf[13] << 8) | ((uint32_t)out_buf[14] << 0);                                       /* integer to float */
        data->humidity_raw = prev.i;                                                                                /* save raw */
        data->humidity_percent = prev.f;                                                                            /* save float */
    }
    else                                                                                                            /* iic */
    {
        uint8_t buf[18];
        
        res = a_scd30_iic_read(handle, SCD30_IIC_COMMAND_READ_MEASUREMENT, (uint8_t *)buf, 18, 20);                 /* read measurement command */
        if (res != 0)                                                                                               /* check result */
        {
            handle->debug_print("scd30: read measurement failed.\n");                                               /* read measurement failed */
           
            return 1;                                                                                               /* return error */
        }
        if (buf[2] != a_scd30_generate_crc8((uint8_t *)buf + 0, 2))                                                 /* check crc */
        {
            handle->debug_print("scd30: crc check failed.\n");                                                      /* crc check failed */
           
            return 1;                                                                                               /* return error */
        }
        if (buf[5] != a_scd30_generate_crc8((uint8_t *)buf + 3, 2))                                                 /* check crc */
        {
            handle->debug_print("scd30: crc check failed.\n");                                                      /* crc check failed */
           
            return 1;                                                                                               /* return error */
        }
        if (buf[8] != a_scd30_generate_crc8((uint8_t *)buf + 6, 2))                                                 /* check crc */
        {
            handle->debug_print("scd30: crc check failed.\n");                                                      /* crc check failed */
           
            return 1;                                                                                               /* return error */
        }
        if (buf[11] != a_scd30_generate_crc8((uint8_t *)buf + 9, 2))                                                /* check crc */
        {
            handle->debug_print("scd30: crc check failed.\n");                                                      /* crc check failed */
           
            return 1;                                                                                               /* return error */
        }
        if (buf[14] != a_scd30_generate_crc8((uint8_t *)buf + 12, 2))                                               /* check crc */
        {
            handle->debug_print("scd30: crc check failed.\n");                                                      /* crc check failed */
           
            return 1;                                                                                               /* return error */
        }
        if (buf[17] != a_scd30_generate_crc8((uint8_t *)buf + 15, 2))                                               /* check crc */
        {
            handle->debug_print("scd30: crc check failed.\n");                                                      /* crc check failed */
           
            return 1;                                                                                               /* return error */
        }
        prev.i = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                 ((uint32_t)buf[3] << 8) | ((uint32_t)buf[4] << 0);                                                 /* integer to float */
        data->co2_raw = prev.i;                                                                                     /* save raw */
        data->co2_ppm = prev.f;                                                                                     /* save float */
        prev.i = ((uint32_t)buf[6] << 24) | ((uint32_t)buf[7] << 16) |
                 ((uint32_t)buf[9] << 8) | ((uint32_t)buf[10] << 0);                                                /* integer to float */
        data->temperature_raw = prev.i;                                                                             /* save raw */
        data->temperature_deg = prev.f;                                                                             /* save float */
        prev.i = ((uint32_t)buf[12] << 24) | ((uint32_t)buf[13] << 16) |
                 ((uint32_t)buf[15] << 8) | ((uint32_t)buf[16] << 0);                                               /* integer to float */
        data->humidity_raw = prev.i;                                                                                /* save raw */
        data->humidity_percent = prev.f;                                                                            /* save float */
    }
    
    return 0;                                                                                                       /* success return 0 */
}

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
uint8_t scd30_init(scd30_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }
    if (handle->debug_print == NULL)                                                                      /* check debug_print */
    {
        return 3;                                                                                         /* return error */
    }
    if (handle->iic_init == NULL)                                                                         /* check iic_init */
    {
        handle->debug_print("scd30: iic_init is null.\n");                                                /* iic_init is null */
    
        return 3;                                                                                         /* return error */
    }
    if (handle->iic_deinit == NULL)                                                                       /* check iic_deinit */
    {
        handle->debug_print("scd30: iic_deinit is null.\n");                                              /* iic_deinit is null */
    
        return 3;                                                                                         /* return error */
    }
    if (handle->iic_write_cmd == NULL)                                                                    /* check iic_write_cmd */
    {
        handle->debug_print("scd30: iic_write_cmd is null.\n");                                           /* iic_write_cmd is null */
    
        return 3;                                                                                         /* return error */
    }
    if (handle->iic_read_cmd == NULL)                                                                     /* check iic_read_cmd */
    {
        handle->debug_print("scd30: iic_read_cmd is null.\n");                                            /* iic_read_cmd is null */
    
        return 3;                                                                                         /* return error */
    }
    if (handle->uart_init == NULL)                                                                        /* check uart_init */
    {
        handle->debug_print("scd30: uart_init is null.\n");                                               /* uart_init is null */
    
        return 3;                                                                                         /* return error */
    }
    if (handle->uart_deinit == NULL)                                                                      /* check uart_deinit */
    {
        handle->debug_print("scd30: uart_deinit is null.\n");                                             /* uart_deinit is null */
    
        return 3;                                                                                         /* return error */
    }
    if (handle->uart_read == NULL)                                                                        /* check uart_read */
    {
        handle->debug_print("scd30: uart_read is null.\n");                                               /* uart_read is null */
    
        return 3;                                                                                         /* return error */
    }
    if (handle->uart_write == NULL)                                                                       /* check uart_write */
    {
        handle->debug_print("scd30: uart_write is null.\n");                                              /* uart_write is null */
    
        return 3;                                                                                         /* return error */
    }
    if (handle->uart_flush == NULL)                                                                       /* check uart_flush */
    {
        handle->debug_print("scd30: uart_flush is null.\n");                                              /* uart_flush is null */
    
        return 3;                                                                                         /* return error */
    }
    if (handle->delay_ms == NULL)                                                                         /* check delay_ms */
    {
        handle->debug_print("scd30: delay_ms is null.\n");                                                /* delay_ms is null */
    
        return 3;                                                                                         /* return error */
    }
    
    if (handle->iic_uart != 0)                                                                            /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[6 + 2];
        uint16_t crc16;
        
        if (handle->uart_init() != 0)                                                                     /* uart init */
        {
            handle->debug_print("scd30: uart init failed.\n");                                            /* uart init failed */
        
            return 1;                                                                                     /* return error */
        }
        
        input_buf[0] = 0x61;                                                                              /* set slave address */
        input_buf[1] = 0x06;                                                                              /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_SOFT_RESET >> 8) & 0xFF;                                       /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_SOFT_RESET >> 0) & 0xFF;                                       /* set addr lsb */
        input_buf[4] = 0x00;                                                                              /* set 0x00 */
        input_buf[5] = 0x01;                                                                              /* set 0x01 */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                     /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                               /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                               /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 8);                                                          /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 8);        /* write read frame */
        if (res != 0)                                                                                     /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                           /* write read failed */
           
            return 4;                                                                                     /* return error */
        }
        crc16 = ((uint16_t)out_buf[7] << 8 | (uint16_t)out_buf[6]);                                       /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 6))                                                  /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                             /* crc check error */
           
            return 4;                                                                                     /* return error */
        }
        if (memcmp(input_buf, out_buf, 8) != 0)                                                           /* check the response */
        {
            handle->debug_print("scd30: response check error.\n");                                        /* response check error */
           
            return 4;                                                                                     /* return error */
        }
    }
    else                                                                                                  /* iic */
    {
        if (handle->iic_init() != 0)                                                                      /* iic init */
        {
            handle->debug_print("scd30: iic init failed.\n");                                             /* iic init failed */
        
            return 1;                                                                                     /* return error */
        }
        res = a_scd30_iic_write(handle, SCD30_IIC_COMMAND_SOFT_RESET, (uint8_t *)NULL, 0, 20);            /* soft reset command */
        if (res != 0)                                                                                     /* check result */
        {
            handle->debug_print("scd30: soft reset failed.\n");                                           /* soft reset failed */
           
            return 4;                                                                                     /* return error */
        }
    }
    handle->delay_ms(100);                                                                                /* delay 100ms */
    handle->inited = 1;                                                                                   /* flag finish initialization */
  
    return 0;                                                                                             /* success return 0 */
}

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
uint8_t scd30_deinit(scd30_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }
    if (handle->inited != 1)                                                                              /* check handle initialization */
    {
        return 3;                                                                                         /* return error */
    }    
    
    if (handle->iic_uart != 0)
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[6 + 2];
        uint16_t crc16;
        
        input_buf[0] = 0x61;                                                                              /* set slave address */
        input_buf[1] = 0x06;                                                                              /* set function code */
        input_buf[2] = (SCD30_UART_ADDRESS_SOFT_RESET >> 8) & 0xFF;                                       /* set addr msb */
        input_buf[3] = (SCD30_UART_ADDRESS_SOFT_RESET >> 0) & 0xFF;                                       /* set addr lsb */
        input_buf[4] = 0x00;                                                                              /* set 0x00 */
        input_buf[5] = 0x01;                                                                              /* set 0x01 */
        crc16 = a_scd30_generate_crc16(input_buf, 6);                                                     /* get crc16 */
        input_buf[6] = (crc16 >> 0) & 0xFF;                                                               /* set crc lsb */
        input_buf[7] = (crc16 >> 8) & 0xFF;                                                               /* set crc mss */
        memset(out_buf, 0, sizeof(uint8_t) * 8);                                                          /* clear the buffer */
        res = a_scd30_uart_write_read(handle, (uint8_t *)input_buf, 8, 50, (uint8_t *)out_buf, 8);        /* write read frame */
        if (res != 0)                                                                                     /* check result */
        {
            handle->debug_print("scd30: write read failed.\n");                                           /* write read failed */
           
            return 4;                                                                                     /* return error */
        }
        crc16 = ((uint16_t)out_buf[7] << 8 | (uint16_t)out_buf[6]);                                       /* get crc16 */
        if (crc16 != a_scd30_generate_crc16(out_buf, 6))                                                  /* check crc16 */
        {
            handle->debug_print("scd30: crc check error.\n");                                             /* crc check error */
           
            return 4;                                                                                     /* return error */
        }
        if (memcmp(input_buf, out_buf, 8) != 0)                                                           /* check the response */
        {
            handle->debug_print("scd30: response check error.\n");                                        /* response check error */
           
            return 4;                                                                                     /* return error */
        }
        if (handle->uart_deinit() != 0)                                                                   /* uart deinit */
        {
            handle->debug_print("scd30: uart deinit failed.\n");                                          /* uart deinit failed */
        
            return 1;                                                                                     /* return error */
        }
    }
    else
    {
        res = a_scd30_iic_write(handle, SCD30_IIC_COMMAND_SOFT_RESET, (uint8_t *)NULL, 0, 20);            /* soft reset command */
        if (res != 0)                                                                                     /* check result */
        {
            handle->debug_print("scd30: soft reset failed.\n");                                           /* soft reset failed */
           
            return 4;                                                                                     /* return error */
        }
        res = handle->iic_deinit();                                                                       /* iic deinit */
        if (res != 0)                                                                                     /* check result */
        {
            handle->debug_print("scd30: iic deinit failed.\n");                                           /* iic deinit */
           
            return 1;                                                                                     /* return error */
        }
    }
    handle->delay_ms(100);                                                                                /* delay 100ms */
    handle->inited = 0;                                                                                   /* flag close initialization */
  
    return 0;                                                                                             /* success return 0 */
}

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
uint8_t scd30_set_get_reg_uart(scd30_handle_t *handle, uint8_t *input, uint16_t in_len, uint8_t *output, uint16_t out_len)
{
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }

    if (handle->iic_uart != 0)
    {
        return a_scd30_uart_write_read(handle, input, in_len, 50, output, out_len);       /* write and read with the uart interface */
    }
    else
    {
        handle->debug_print("scd30: iic interface is invalid.\n");                        /* iic interface is invalid */
       
        return 1;                                                                         /* return error */
    }
}

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
uint8_t scd30_set_reg_iic(scd30_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }

    if (handle->iic_uart != 0)
    {
        handle->debug_print("scd30: uart interface is invalid.\n");        /* uart interface is invalid */
       
        return 1;                                                          /* return error */
    }
    else
    {
        return a_scd30_iic_write(handle, reg, buf, len, 20);               /* write the data */
    }
}

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
uint8_t scd30_get_reg_iic(scd30_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }

    if (handle->iic_uart != 0)
    {
        handle->debug_print("scd30: uart interface is invalid.\n");        /* uart interface is invalid */
       
        return 1;
    }
    else
    {
        return a_scd30_iic_read(handle, reg, buf, len, 20);                /* read the data */
    }
}

/**
 * @brief      get chip information
 * @param[out] *info points to an scd30 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t scd30_info(scd30_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(scd30_info_t));                          /* initialize scd30 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "UART IIC", 16);                       /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
