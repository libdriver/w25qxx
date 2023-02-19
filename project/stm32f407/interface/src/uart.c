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
 * @date      2022-11-11
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/11/11  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "uart.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief uart1 var definition
 */
UART_HandleTypeDef g_uart_handle;              /**< uart handle */
uint8_t g_uart_rx_buffer[UART_MAX_LEN];        /**< uart rx buffer */
uint8_t g_uart_buffer;                         /**< uart one buffer */
volatile uint16_t g_uart_point;                /**< uart rx point */
volatile uint8_t g_uart_tx_done;               /**< uart tx done flag */

/**
 * @brief uart2 var definition
 */
UART_HandleTypeDef g_uart2_handle;               /**< uart2 handle */
uint8_t g_uart2_rx_buffer[UART2_MAX_LEN];        /**< uart2 rx buffer */
uint8_t g_uart2_buffer;                          /**< uart2 one buffer */
volatile uint16_t g_uart2_point;                 /**< uart2 rx point */
volatile uint8_t g_uart2_tx_done;                /**< uart2 tx done flag */

/**
 * @brief     uart init with 8 data bits, 1 stop bit and no parity
 * @param[in] baud is the baud rate
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      TX is PA9 and RX is PA10
 */
uint8_t uart_init(uint32_t baud)
{
    g_uart_handle.Instance = USART1;
    g_uart_handle.Init.BaudRate = baud;
    g_uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
    g_uart_handle.Init.StopBits = UART_STOPBITS_1;
    g_uart_handle.Init.Parity = UART_PARITY_NONE;
    g_uart_handle.Init.Mode = UART_MODE_TX_RX;
    g_uart_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    g_uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;
    
    /* uart init */
    if (HAL_UART_Init(&g_uart_handle) != HAL_OK)
    {
        return 1;
    }
    
    /* receive one byte */
    if (HAL_UART_Receive_IT(&g_uart_handle, (uint8_t *)&g_uart_buffer, 1) != HAL_OK)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  uart deint
 * @return status code
 *         - 0 success
 *         - 1 deinit
 * @note   none
 */
uint8_t uart_deinit(void)
{
    /* uart deinit */
    if (HAL_UART_DeInit(&g_uart_handle) != HAL_OK)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     uart write data
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t uart_write(uint8_t *buf, uint16_t len)
{
    uint16_t timeout = 1000;
    
    /* set tx done 0 */
    g_uart_tx_done = 0;
    
    /* transmit */
    if (HAL_UART_Transmit_IT(&g_uart_handle, (uint8_t *)buf, len) != HAL_OK)
    {
        return 1;
    }
    
    /* wait for events */
    while ((g_uart_tx_done == 0) && (timeout != 0))
    {
        HAL_Delay(1);
        timeout--;
    }
    
    /* check the timeout */
    if (timeout != 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
 * @brief      uart read data
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     length of the read data
 * @note       this function will clear all received buffer even read length is less than received length
 */
uint16_t uart_read(uint8_t *buf, uint16_t len)
{
    uint16_t read_len;
    uint16_t g_uart_point_old;
    
    /* check receiving */
    start:
    g_uart_point_old = g_uart_point;
    HAL_Delay(1);
    if (g_uart_point > g_uart_point_old)
    {
        goto start;
    }
    
    /* copy the data */
    read_len = (len < g_uart_point) ? len : g_uart_point;
    memcpy(buf, g_uart_rx_buffer, read_len);
    
    /* clear the buffer */
    g_uart_point = 0;
    
    return read_len;
}

/**
 * @brief  uart flush data
 * @return status code
 *         - 0 success
 * @note   none
 */
uint16_t uart_flush(void)
{
    /* clear the buffer */
    g_uart_point = 0;
    
    return 0;
}

/**
 * @brief     uart print format data
 * @param[in] fmt is the format data
 * @return    length of the sent data
 * @note      none
 */
uint16_t uart_print(const char *const fmt, ...)
{
    char str[256];
    uint16_t len;
    va_list args;
    
    /* print to the buffer */
    memset((char *)str, 0, sizeof(char) * 256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);
    
    /* send the data */
    len = strlen((char *)str);
    if (uart_write((uint8_t *)str, len) != 0)
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
 * @param[in] baud is the baud rate
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      TX is PA2 and RX is PA3
 */
uint8_t uart2_init(uint32_t baud)
{
    g_uart2_handle.Instance = USART2;
    g_uart2_handle.Init.BaudRate = baud;
    g_uart2_handle.Init.WordLength = UART_WORDLENGTH_8B;
    g_uart2_handle.Init.StopBits = UART_STOPBITS_1;
    g_uart2_handle.Init.Parity = UART_PARITY_NONE;
    g_uart2_handle.Init.Mode = UART_MODE_TX_RX;
    g_uart2_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    g_uart2_handle.Init.OverSampling = UART_OVERSAMPLING_16;
    
    /* uart init */
    if (HAL_UART_Init(&g_uart2_handle) != HAL_OK)
    {
        return 1;
    }
    
    /* receive one byte */
    if (HAL_UART_Receive_IT(&g_uart2_handle, (uint8_t *)&g_uart2_buffer, 1) != HAL_OK)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  uart2 deint
 * @return status code
 *         - 0 success
 *         - 1 deinit
 * @note   none
 */
uint8_t uart2_deinit(void)
{
    /* uart deinit */
    if (HAL_UART_DeInit(&g_uart2_handle) != HAL_OK)
    {
        return 1;
    }
    
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
    
    /* set tx done 0 */
    g_uart2_tx_done = 0;
    
    /* transmit */
    if (HAL_UART_Transmit_IT(&g_uart2_handle, (uint8_t *)buf, len) != HAL_OK)
    {
        return 1;
    }
    
    /* wait for events */
    while ((g_uart2_tx_done == 0) && (timeout != 0))
    {
        HAL_Delay(1);
        timeout--;
    }
    
    /* check the timeout */
    if (timeout != 0)
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
 * @return     length of the read data
 * @note       this function will clear all received buffer even read length is less than received length
 */
uint16_t uart2_read(uint8_t *buf, uint16_t len)
{
    uint16_t read_len;
    uint16_t g_uart_point_old;
    
    /* check receiving */
    start:
    g_uart_point_old = g_uart2_point;
    HAL_Delay(1);
    if (g_uart2_point > g_uart_point_old)
    {
        goto start;
    }
    
    /* copy the data */
    read_len = (len < g_uart2_point) ? len : g_uart2_point;
    memcpy(buf, g_uart2_rx_buffer, read_len);
    
    /* clear the buffer */
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
    /* clear the buffer */
    g_uart2_point = 0;
    
    return 0;
}

/**
 * @brief  uart get the handle
 * @return points to a uart handle
 * @note   none
 */
UART_HandleTypeDef* uart_get_handle(void)
{
    return &g_uart_handle;
}

/**
 * @brief  uart2 get the handle
 * @return points to a uart handle
 * @note   none
 */
UART_HandleTypeDef* uart2_get_handle(void)
{
    return &g_uart2_handle;
}

/**
 * @brief uart set tx done
 * @note  none
 */
void uart_set_tx_done(void)
{
    g_uart_tx_done = 1;
}

/**
 * @brief uart2 set tx done
 * @note  none
 */
void uart2_set_tx_done(void)
{
    g_uart2_tx_done = 1;
}

/**
 * @brief uart irq handler
 * @note  none
 */
void uart_irq_handler(void)
{
    /* save one byte */
    g_uart_rx_buffer[g_uart_point] = g_uart_buffer;
    g_uart_point++;
    if (g_uart_point > (UART_MAX_LEN - 1))
    {
        g_uart_point = 0;
    }
    
    /* receive one byte */
    (void)HAL_UART_Receive_IT(&g_uart_handle, (uint8_t *)&g_uart_buffer, 1);
}

/**
 * @brief uart2 irq handler
 * @note  none
 */
void uart2_irq_handler(void)
{
    /* save one byte */
    g_uart2_rx_buffer[g_uart2_point] = g_uart2_buffer;
    g_uart2_point++;
    if (g_uart2_point > (UART2_MAX_LEN - 1))
    {
        g_uart2_point = 0;
    }
    
    /* receive one byte */
    (void)HAL_UART_Receive_IT(&g_uart2_handle, (uint8_t *)&g_uart2_buffer, 1);
}
