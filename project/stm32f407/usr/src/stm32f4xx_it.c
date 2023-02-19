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
 * @file      stm32f4xx_it.c
 * @brief     stm32f4xx it source file
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

#include "stm32f4xx_it.h"
#include "uart.h"

/**
 * @brief nmi handler
 * @note  none
 */
void NMI_Handler(void)
{
    /* run nmi irq */
    HAL_RCC_NMI_IRQHandler();
}

/**
 * @brief hardfault handler
 * @note  none
 */
void HardFault_Handler(void)
{
  /* go to infinite loop when hard fault exception occurs */
  while (1)
  {
      
  }
}

/**
 * @brief memory manage handler
 * @note  none
 */
void MemManage_Handler(void)
{
  /* go to infinite loop when memory manage exception occurs */
  while (1)
  {
      
  }
}

/**
 * @brief bus fault handler
 * @note  none
 */
void BusFault_Handler(void)
{
  /* go to infinite loop when bus fault exception occurs */
  while (1)
  {
      
  }
}

/**
 * @brief usage fault handler
 * @note  none
 */
void UsageFault_Handler(void)
{
  /* go to infinite loop when usage fault exception occurs */
  while (1)
  {
      
  }
}

/**
 * @brief svc handler
 * @note  none
 */
void SVC_Handler(void)
{
    
}

/**
 * @brief debug handler
 * @note  none
 */
void DebugMon_Handler(void)
{
    
}

/**
 * @brief pendsv handler
 * @note  none
 */
void PendSV_Handler(void)
{
    
}

/**
 * @brief systick handler
 * @note  none
 */
void SysTick_Handler(void)
{
    /* run systick irq */
    HAL_SYSTICK_IRQHandler();
}

/**
 * @brief systick callback
 * @note  none
 */
void HAL_SYSTICK_Callback(void)
{
    /* inc tick */
    HAL_IncTick();
}

/**
 * @brief usart1 irq handler
 * @note  none
 */
void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(uart_get_handle());
}  

/**
 * @brief usart2 irq handler
 * @note  none
 */
void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(uart2_get_handle());
}

/**
 * @brief     uart error callback
 * @param[in] *huart points to a uart handle
 * @note      none
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    __HAL_UART_CLEAR_FEFLAG(huart);
}

/**
 * @brief     uart rx receive callback
 * @param[in] *huart points to a uart handle
 * @note      none
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{    
    if (huart->Instance == USART1)
    {
        /* run the uart irq handler */
        uart_irq_handler();
    }
    if (huart->Instance == USART2)
    {
        /* run the uart2 irq handler */
        uart2_irq_handler();
    }
}

/**
 * @brief     uart tx finished callback
 * @param[in] *huart points to a uart handle
 * @note      none
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        /* set tx done */
        uart_set_tx_done();
    }
    if (huart->Instance == USART2)
    {
        /* set tx done */
        uart2_set_tx_done();
    }
}
