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
 * @file      uart.h
 * @brief     uart header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-11-11
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/11/11  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef UART_H
#define UART_H

#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup uart uart function
 * @brief    uart function modules
 * @{
 */

/**
 * @brief uart max rx buffer length definition
 */
#define UART_MAX_LEN        256        /**< uart max len */
#define UART2_MAX_LEN       512        /**< uart2 max len */

/**
 * @brief     uart init with 8 data bits, 1 stop bit and no parity
 * @param[in] baud is the baud rate
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      TX is PA9 and RX is PA10
 */
uint8_t uart_init(uint32_t baud_rate);

/**
 * @brief  uart deint
 * @return status code
 *         - 0 success
 *         - 1 deinit
 * @note   none
 */
uint8_t uart_deinit(void);

/**
 * @brief     uart write data
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t uart_write(uint8_t *buf, uint16_t len);

/**
 * @brief      uart read data
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     length of the read data
 * @note       this function will clear all received buffer even read length is less than received length
 */
uint16_t uart_read(uint8_t *buf, uint16_t len);

/**
 * @brief  uart flush data
 * @return status code
 *         - 0 success
 * @note   none
 */
uint16_t uart_flush(void);

/**
 * @brief     uart print format data
 * @param[in] fmt is the format data
 * @return    length of the sent data
 * @note      none
 */
uint16_t uart_print(const char *const fmt, ...);

/**
 * @brief  uart get the handle
 * @return points to a uart handle
 * @note   none
 */
UART_HandleTypeDef* uart_get_handle(void);

/**
 * @brief uart set tx done
 * @note  none
 */
void uart_set_tx_done(void);

/**
 * @brief uart irq handler
 * @note  none
 */
void uart_irq_handler(void);

/**
 * @brief     uart2 init with 8 data bits, 1 stop bit and no parity
 * @param[in] baud is the baud rate
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      TX is PA2 and RX is PA3
 */
uint8_t uart2_init(uint32_t baud);

/**
 * @brief  uart2 deint
 * @return status code
 *         - 0 success
 *         - 1 deinit
 * @note   none
 */
uint8_t uart2_deinit(void);

/**
 * @brief     uart2 write data
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t uart2_write(uint8_t *buf, uint16_t len);

/**
 * @brief      uart2 read data
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     length of the read data
 * @note       this function will clear all received buffer even read length is less than received length
 */
uint16_t uart2_read(uint8_t *buf, uint16_t len);

/**
 * @brief  uart2 flush data
 * @return status code
 *         - 0 success
 * @note   none
 */
uint16_t uart2_flush(void);

/**
 * @brief  uart2 get the handle
 * @return points to a uart handle
 * @note   none
 */
UART_HandleTypeDef* uart2_get_handle(void);

/**
 * @brief uart2 set tx done
 * @note  none
 */
void uart2_set_tx_done(void);

/**
 * @brief uart2 irq handler
 * @note  none
 */
void uart2_irq_handler(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
