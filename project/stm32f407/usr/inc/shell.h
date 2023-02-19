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
 * @file      shell.h
 * @brief     shell header file
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

#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup shell shell function
 * @brief    shell function modules
 * @{
 */

/**
 * @brief shell param definition
 */
#define SHELL_MAX_SIZE        32         /**< shell max function number */
#define SHELL_MAX_NAME        32         /**< shell max function name length */
#define SHELL_MAX_REG_SIZE    32         /**< shell max register number */
#define SHELL_MAX_BUF_SIZE    256        /**< shell max buffer size */

/**
 * @brief function struture definition
 */
typedef struct function_s
{
    char name[SHELL_MAX_NAME];                        /**< functon name */
    uint8_t (*fuc)(uint8_t argc, char **argv);        /**< function address */
} function_t;

/**
 * @brief shell struture definition
 */
typedef struct shell_s
{
    function_t fuc[SHELL_MAX_REG_SIZE];         /**< functon buffer */
    uint16_t fuc_i;                             /**< functon number */
    char *argv[SHELL_MAX_SIZE];                 /**< argv temp buffer */
    char buf_out[SHELL_MAX_BUF_SIZE];           /**< output buffer */
} shell_t;

/**
 * @brief  init shell
 * @return status code
 *         - 0 success
 * @note   none
 */
uint8_t shell_init(void);

/**
 * @brief     shell register function
 * @param[in] *name points to a name buffer
 * @param[in] *fuc points to a function address
 * @return    status code
 *            - 0 success
 *            - 1 buffer is full
 * @note      none
 */
uint8_t shell_register(char *name, uint8_t (*fuc)(uint8_t argc, char **argv));

/**
 * @brief     shell parse command
 * @param[in] *buf points to a buffer address
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 find function failed
 *            - 3 length is too big
 *            - 4 pretreat failed
 * @note      none
 */
uint8_t shell_parse(char *buf, uint16_t len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
