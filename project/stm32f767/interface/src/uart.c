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
 * @file      uart.c
 * @brief     uart source file
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

#include "uart.h"
#include "delay.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief uart1 var definition
 */
UART_HandleTypeDef g_uart1_handle;               /**< uart1 handle */
uint8_t g_uart1_rx_buffer[UART1_MAX_LEN];        /**< uart1 rx buffer */
uint8_t g_uart1_buffer;                          /**< uart1 one buffer */
uint16_t g_uart1_point;                          /**< uart1 rx point */
uint8_t g_uart1_tx_done;                         /**< uart1 tx done flag */

/**
 * @brief uart2 var definition
 */
UART_HandleTypeDef g_uart2_handle;               /**< uart2 handle */
uint8_t g_uart2_rx_buffer[UART2_MAX_LEN];        /**< uart2 rx buffer */
uint8_t g_uart2_buffer;                          /**< uart2 one buffer */
uint16_t g_uart2_point;                          /**< uart2 rx point */
uint8_t g_uart2_tx_done;                         /**< uart2 tx done flag */

/**
 * @brief     uart1 init with 8 data bits, 1 stop bit and no parity
 * @param[in] baud rate
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      TX is PA9 and RX is PA10
 */
uint8_t uart1_init(uint32_t baud_rate)
{
    g_uart1_handle.Instance = USART1;
    g_uart1_handle.Init.BaudRate = baud_rate;
    g_uart1_handle.Init.WordLength = UART_WORDLENGTH_8B;
    g_uart1_handle.Init.StopBits = UART_STOPBITS_1;
    g_uart1_handle.Init.Parity = UART_PARITY_NONE;
    g_uart1_handle.Init.Mode = UART_MODE_TX_RX;
    g_uart1_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    g_uart1_handle.Init.OverSampling = UART_OVERSAMPLING_16;
  
    if (HAL_UART_Init(&g_uart1_handle) != HAL_OK)
    {
        return 1;
    }
    if (HAL_UART_Receive_IT(&g_uart1_handle, (uint8_t *)&g_uart1_buffer, 1))
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  uart1 deint
 * @return status code
 *         - 0 success
 * @note   TX is PA9 and RX is PA10
 */
uint8_t uart1_deinit(void)
{
    HAL_UART_DeInit(&g_uart1_handle);
    
    return 0;
}

/**
 * @brief     uart1 write data
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t uart1_write(uint8_t *buf, uint16_t len)
{
    uint16_t timeout = 1000;
    
    g_uart1_tx_done = 0;
    if (HAL_UART_Transmit_IT(&g_uart1_handle, (uint8_t *)buf, len))
    {
        return 1;
    }
    while ((g_uart1_tx_done == 0) && timeout)
    {
        delay_ms(1);
        timeout--;
    }
    if (timeout)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
 * @brief      uart1 read data
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       this function will clear all received buffer even read length is less than received length
 */
uint16_t uart1_read(uint8_t *buf, uint16_t len)
{
    uint16_t read_len;
    uint16_t g_uart1_point_old;
    
    start:
    g_uart1_point_old = g_uart1_point;
    delay_ms(1);
    if (g_uart1_point > g_uart1_point_old)
    {
        goto start;
    }
    read_len = len<g_uart1_point ? len : g_uart1_point;
    memcpy(buf, g_uart1_rx_buffer, read_len);
    g_uart1_point = 0;
    
    return read_len;
}

/**
 * @brief  uart1 flush data
 * @return status code
 *         - 0 success
 * @note   none
 */
uint16_t uart1_flush(void)
{
    g_uart1_point = 0;
    
    return 0;
}

/**
 * @brief     uart1 print format data
 * @param[in] fmt is the format data
 * @return    sent length of data
 * @note      none
 */
uint16_t uart1_print(char  *fmt, ...)
{
    char str[256];
    uint8_t len;
    va_list args;
    
    memset((char *)str, 0, sizeof(char)*256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 256, (char const *)fmt, args);
    va_end(args);
        
    len = strlen((char *)str);
    if (uart1_write((uint8_t *)str, len))
    {
        return 0;
    }
    else
    { 
        return len;
    }
}

/**
 * @brief     uart2 init with 8 data bits, 1 stop bit and no parity
 * @param[in] baud rate
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      TX is PA2 and RX is PA3
 */
uint8_t uart2_init(uint32_t baud_rate)
{
    g_uart2_handle.Instance = USART2;
    g_uart2_handle.Init.BaudRate = baud_rate;
    g_uart2_handle.Init.WordLength = UART_WORDLENGTH_8B;
    g_uart2_handle.Init.StopBits = UART_STOPBITS_1;
    g_uart2_handle.Init.Parity = UART_PARITY_NONE;
    g_uart2_handle.Init.Mode = UART_MODE_TX_RX;
    g_uart2_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    g_uart2_handle.Init.OverSampling = UART_OVERSAMPLING_16;
  
    if (HAL_UART_Init(&g_uart2_handle) != HAL_OK)
    {
        return 1;
    }
    if (HAL_UART_Receive_IT(&g_uart2_handle, (uint8_t *)&g_uart2_buffer, 1))
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  uart2 deint
 * @return status code
 *         - 0 success
 * @note   TX is PA2 and RX is PA3
 */
uint8_t uart2_deinit(void)
{
    HAL_UART_DeInit(&g_uart2_handle);
    
    return 0;
}

/**
 * @brief     uart2 write data
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t uart2_write(uint8_t *buf, uint16_t len)
{
    uint16_t timeout = 1000;
    
    g_uart2_tx_done = 0;
    if (HAL_UART_Transmit_IT(&g_uart2_handle, (uint8_t *)buf, len))
    {
        return 1;
    }
    while ((g_uart2_tx_done == 0) && timeout)
    {
        delay_ms(1);
        timeout--;
    }
    if (timeout)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
 * @brief      uart2 read data
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       this function will clear all received buffer even read length is less than received length
 */
uint16_t uart2_read(uint8_t *buf, uint16_t len)
{
    uint16_t read_len;
    uint16_t g_uart2_point_old;
    
    start:
    g_uart2_point_old = g_uart2_point;
    delay_ms(1);
    if (g_uart2_point > g_uart2_point_old)
    {
        goto start;
    }
    read_len = len<g_uart2_point ? len : g_uart2_point;
    memcpy(buf, g_uart2_rx_buffer, read_len);
    g_uart2_point = 0;
    
    return read_len;
}

/**
 * @brief  uart2 flush data
 * @return status code
 *         - 0 success
 * @note   none
 */
uint16_t uart2_flush(void)
{
    g_uart2_point = 0;
    
    return 0;
}
