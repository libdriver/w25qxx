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
 * @file      stm32f7xx_hal_msp.c
 * @brief     stm32f7xx hal msp source file
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

#include "stm32f7xx_hal.h"

/** 
 * @defgroup msp HAL_MSP_Private_Functions
 * @{
 */

/**
 * @brief  initializes the global msp.
 * @note   none
 */
void HAL_MspInit(void)
{
    
}

/**
 * @brief  deinitializes the global msp.
 * @note   none
 */
void HAL_MspDeInit(void)
{
    
}

/**
 * @brief     uart hal init
 * @param[in] *huart points to a uart handle
 * @note      none
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    if (huart->Instance == USART1)
    {
        /* enable uart gpio clock */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        
        /* enable usart1 clock */
        __HAL_RCC_USART1_CLK_ENABLE();
        
        /**
         * PA9  ------> USART1_TX
         * PA10 ------> USART1_RX 
         */
        GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        /* enable nvic */
        HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
    if (huart->Instance == USART2)
    {
        /* enable uart gpio clock */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        
        /* enable usart2 clock */
        __HAL_RCC_USART2_CLK_ENABLE();
    
        /**
         * PA2 ------> USART2_TX
         * PA3 ------> USART2_RX 
         */
        GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        /* enable nvic */
        HAL_NVIC_SetPriority(USART2_IRQn, 2, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
}

/**
 * @brief     uart hal deinit
 * @param[in] *huart points to a uart handle
 * @note      none
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
    if (huart->Instance == USART1)
    {
        /* disable usart1 clock */
        __HAL_RCC_USART1_CLK_DISABLE();
        
        /* uart gpio deinit */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
        
        /* disable nvic */
        HAL_NVIC_DisableIRQ(USART1_IRQn);
    }
    if (huart->Instance == USART2)
    {
        /* disable usart2 clock */
        __HAL_RCC_USART2_CLK_DISABLE();
        
        /* uart gpio deinit */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);
        
        /* disable nvic */
        HAL_NVIC_DisableIRQ(USART2_IRQn);
    }
}

/**
 * @brief     qspi hal init
 * @param[in] *hqspi points to a qspi handle
 * @note      none
 */
void HAL_QSPI_MspInit(QSPI_HandleTypeDef *hqspi)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    /* enable qspi clock */
    __HAL_RCC_QSPI_CLK_ENABLE();
    
    /* enable qspi gpio clock */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    
    /**
     * PB6 ------> QSPI_CS
     */
    GPIO_Initure.Pin = GPIO_PIN_6;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    GPIO_Initure.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);
    
    /**
     * PF8 ------> QSPI_IO0
     * PF9 ------> QSPI_IO1
     */
    GPIO_Initure.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_Initure.Pull = GPIO_NOPULL;
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOF, &GPIO_Initure);
    
    /**
     * PB2 ------> QSPI_SCK
     */
    GPIO_Initure.Pin = GPIO_PIN_2;
    GPIO_Initure.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);
    
    /**
     * PF6 ------> QSPI_IO3
     * PF7 ------> QSPI_IO2
     */
    GPIO_Initure.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOF, &GPIO_Initure);
}

/**
 * @brief     qspi hal deinit
 * @param[in] *hqspi points to a qspi handle
 * @note      none
 */
void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef *hqspi)
{
    /* disable qspi clock */
    __HAL_RCC_QSPI_CLK_DISABLE();
    
    /* qspi gpio deinit */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6 | GPIO_PIN_2);
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_6 | GPIO_PIN_7);
}
