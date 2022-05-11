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
 * @file      qspi.h
 * @brief     qspi header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-08-07
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/08/07  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef QSPI_H
#define QSPI_H

#include "stm32f7xx_hal.h"

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @defgroup qspi qspi function
 * @brief    qspi function modules
 * @{
 */

/**
 * @brief qspi mode enumeration definition
 */
typedef enum
{
    QSPI_MODE_0 = 0x00,        /**< mode 0 */
    QSPI_MODE_3 = 0x01,        /**< mode 3 */
}qspi_mode_t;

/**
 * @brief     qspi bus init
 * @param[in] mode is the qspi mode
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 *            - 2 mode is invalid
 * @note      SCLK is PB2, CS is PB6, IO0 is PF8, IO1 is PF9, IO2 is PF7 and IO3 is PF6
 */
uint8_t qspi_init(qspi_mode_t mode);

/**
 * @brief      qspi interface write and read bytes
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[in]  instruction is the sent instruction
 * @param[in]  instruction_line is the instruction phy lines
 * @param[in]  address is the register address
 * @param[in]  address_line is the address phy lines
 * @param[in]  address_len is the address length
 * @param[in]  alternate is the register address
 * @param[in]  alternate_line is the alternate phy lines
 * @param[in]  alternate_len is the alternate length
 * @param[in]  dummy is the dummy cycle
 * @param[in]  *in_buf points to a input buffer
 * @param[in]  in_len is the input length
 * @param[out] *out_buf points to a output buffer
 * @param[in]  out_len is the output length
 * @param[in]  data_line is the data phy lines
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 *             - 2 param is invalid
 * @note       none
 */
uint8_t qspi_write_read(uint8_t instruction, uint8_t instruction_line,
                        uint32_t address, uint8_t address_line, uint8_t address_len,
                        uint32_t alternate, uint8_t alternate_line, uint8_t alternate_len,
                        uint8_t dummy, uint8_t *in_buf, uint32_t in_len,
                        uint8_t *out_buf, uint32_t out_len, uint8_t data_line);

/**
 * @brief  qspi bus deinit
 * @return status code
 *         - 0 success
 * @note   none
 */
uint8_t qspi_deinit(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
