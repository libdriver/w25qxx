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
 * @file      delay.h
 * @brief     delay header file
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

#ifndef DELAY_H
#define DELAY_H

#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup delay delay function
 * @brief    delay function modules
 * @{
 */

/**
 * @brief  delay clock init
 * @return status code
 *         - 0 success
 * @note   none
 */
uint8_t delay_init(void);

/**
 * @brief     delay us
 * @param[in] us time
 * @note      none
 */
void delay_us(uint32_t us);

/**
 * @brief     delay ms
 * @param[in] ms time
 * @note      none
 */
void delay_ms(uint32_t ms);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif 
