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
 * @file      shell.c
 * @brief     shell source file
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

#include "shell.h"

static shell_t gs_shell;        /**< shell handle */

/**
 * @brief      shell pretreatment
 * @param[in]  *buf pointer to an in data buffer
 * @param[in]  in_len length of in data
 * @param[out] *buf_out pointer to an out data buffer
 * @param[out] *out_len pointer to the length of a out data buffer
 * @param[out] **argv pointer to a argv buffer
 * @param[out] *argc pointer to a argc buffer
 * @return     status code
 *             - 0 success
 *             - 1 pretreatment failed
 * @note       none
 */
static uint8_t a_shell_pretreatment(char *buf, uint16_t in_len, char *buf_out, uint16_t *out_len, char **argv, uint8_t *argc)
{
    uint16_t i;
    uint8_t flag = 0;
  
    *out_len = 0;
    *argc = 0;
    flag = 1;
    for (i = 0; i < in_len; i++)
    {
        if (buf[i] != ' ')
        {
            buf_out[(*out_len)] = buf[i];
            (*out_len)++;
            if (flag == 1)
            {
                argv[(*argc)] = &buf_out[(*out_len-1)];
                (*argc)++; 
                if((*argc) >= SHELL_MAX_SIZE)
                {
                    return 1;
                }
            }
            flag = 0;
        }
        else
        {
            if (flag == 0)
            {
                buf_out[(*out_len)] = NULL;
                (*out_len)++;
                flag = 1;
            }
        }
    }
    
    return 0;
}

/**
 * @brief     shell find function
 * @param[in] *handle pointer to a shell structure
 * @param[in] *name pointer to a name buffer
 * @return    status code
 *            - 0 success
 *            - 1 find failed
 * @note      none
 */
static uint8_t a_shell_find(shell_t *handle, char *name)
{
    uint16_t i;
    
    for (i = 0; i < SHELL_MAX_REG_SIZE; i++)
    {
       if (strcmp(handle->fuc[i].name, name) == 0)
       {
           return 0;
       }
    }
    
    return 1;
}

/**
 * @brief     shell run function
 * @param[in] *handle pointer to a shell structure
 * @param[in] argc number of param
 * @param[in] **argv param
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 find function failed
 * @note      none
 */
static uint8_t a_shell_run(shell_t *handle, uint8_t argc, char **argv)
{
    uint16_t i;
    
    for (i = 0; i < SHELL_MAX_REG_SIZE; i++)
    {
        if (strcmp(handle->fuc[i].name, argv[0]) == 0)
        {
            if (handle->fuc[i].fuc != NULL)
            {
                return handle->fuc[i].fuc(argc, argv);
            }
        }
    }
    
    return 2;
}

/**
 * @brief     shell register function
 * @param[in] *name pointer to a name buffer
 * @param[in] *fuc pointer to a function address
 * @return    status code
 *            - 0 success
 *            - 1 buffer is full
 * @note      none
 */
uint8_t shell_register(char *name, uint8_t (*fuc)(uint8_t argc, char **argv))
{
    if (gs_shell.fuc_i >= SHELL_MAX_REG_SIZE)
    {
        return 1;
    }
    
    strncpy(gs_shell.fuc[gs_shell.fuc_i].name, name, SHELL_MAX_NAME);
    gs_shell.fuc[gs_shell.fuc_i].fuc = fuc;
    gs_shell.fuc_i++;
    
    return 0;
}

/**
 * @brief  init shell
 * @return status code
 *         - 0 success
 * @note   none
 */
uint8_t shell_init(void)
{
    memset(&gs_shell, 0 , sizeof(shell_t));
    
    return 0;
}

/**
 * @brief     shell parse command
 * @param[in] *buf pointer to a buffer address
 * @param[in] len buffer length
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 find function failed
 *            - 3 length is too big
 *            - 4 pretreat failed
 * @note      none
 */
uint8_t shell_parse(char *buf, uint16_t len)
{
    uint8_t argc;
    uint16_t out_len;
    
    if (len > SHELL_MAX_BUF_SIZE)
    {
        return 3;
    }
    
    memset(&gs_shell.buf_out, 0 , sizeof(uint8_t) * SHELL_MAX_BUF_SIZE);
    if (a_shell_pretreatment(buf, len, gs_shell.buf_out, (uint16_t *)&out_len, gs_shell.argv, (uint8_t *)(&argc)) != 0)
    {
        return 4;
    }
    if (a_shell_find(&gs_shell, (char *)gs_shell.argv[0]) != 0)
    {
        return 2;
    } 
    
    return a_shell_run(&gs_shell, argc, (char **)gs_shell.argv);
}
