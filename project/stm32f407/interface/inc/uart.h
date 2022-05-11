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
 * @date      2021-2-12
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/02/12  <td>1.0      <td>Shifeng Li  <td>first upload
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
#define UART1_MAX_LEN 256        /**< uart1 max len */
#define UART2_MAX_LEN 256        /**< uart2 max len */

/**
 * @brief uart1 var definition
 */
extern UART_HandleTypeDef g_uart1_handle;               /**< uart1 handle */
extern uint8_t g_uart1_rx_buffer[UART1_MAX_LEN];        /**< uart1 rx buffer */
extern uint8_t g_uart1_buffer;                          /**< uart1 one buffer */
extern uint16_t g_uart1_point;                          /**< uart1 rx point */
extern uint8_t g_uart1_tx_done;                         /**< uart1 tx done flag */

/**
 * @brief uart2 var definition
 */
extern UART_HandleTypeDef g_uart2_handle;               /**< uart2 handle */
extern uint8_t g_uart2_rx_buffer[UART2_MAX_LEN];        /**< uart2 rx buffer */
extern uint8_t g_uart2_buffer;                          /**< uart2 one buffer */
extern uint16_t g_uart2_point;                          /**< uart2 rx point */
extern uint8_t g_uart2_tx_done;                         /**< uart2 tx done flag */

/**
 * @brief     uart1 init with 8 data bits, 1 stop bit and no parity
 * @param[in] baud rate
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      TX is PA9 and RX is PA10
 */
uint8_t uart1_init(uint32_t baud_rate);

/**
 * @brief  uart1 deint
 * @return status code
 *         - 0 success
 * @note   TX is PA9 and RX is PA10
 */
uint8_t uart1_deinit(void);

/**
 * @brief     uart1 write data
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t uart1_write(uint8_t *buf, uint16_t len);

/**
 * @brief      uart1 read data
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       this function will clear all received buffer even read length is less than received length
 */
uint16_t uart1_read(uint8_t *buf, uint16_t len);

/**
 * @brief  uart1 flush data
 * @return status code
 *         - 0 success
 * @note   none
 */
uint16_t uart1_flush(void);

/**
 * @brief     uart1 print format data
 * @param[in] fmt is the format data
 * @return    sent length of data
 * @note      none
 */
uint16_t uart1_print(char  *fmt, ...);

/**
 * @brief     uart2 init with 8 data bits, 1 stop bit and no parity
 * @param[in] baud rate
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      TX is PA2 and RX is PA3
 */
uint8_t uart2_init(uint32_t baud_rate);

/**
 * @brief  uart2 deint
 * @return status code
 *         - 0 success
 * @note   TX is PA2 and RX is PA3
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
 * @return     status code
 *             - 0 success
 *             - 1 read failed
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
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
