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
 * @file      stm32f4xx_it.h
 * @brief     stm32f4xx it header file
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

#ifndef STM32F4XX_IT_H
#define STM32F4XX_IT_H

#include "stm32f4xx_hal.h"

#ifdef __cplusplus
 extern "C" {
#endif 

/** 
 * @defgroup IT HAL_IT_Private_Functions
 * @{
 */

/**
 * @brief nmi handler
 * @note  none
 */
void NMI_Handler(void);

/**
 * @brief hardfault handler
 * @note  none
 */
void HardFault_Handler(void);

/**
 * @brief memory manage handler
 * @note  none
 */
void MemManage_Handler(void);

/**
 * @brief bus fault handler
 * @note  none
 */
void BusFault_Handler(void);

/**
 * @brief usage fault handler
 * @note  none
 */
void UsageFault_Handler(void);

/**
 * @brief svc handler
 * @note  none
 */
void SVC_Handler(void);

/**
 * @brief debug handler
 * @note  none
 */
void DebugMon_Handler(void);

/**
 * @brief pendsv handler
 * @note  none
 */
void PendSV_Handler(void);

/**
 * @brief systick handler
 * @note  none
 */
void SysTick_Handler(void);

/**
 * @brief usart1 irq handler
 * @note  none
 */
void USART1_IRQHandler(void);

/**
 * @brief usart2 irq handler
 * @note  none
 */
void USART2_IRQHandler(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif 
