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
 * @file      driver_w25qxx_read_test.h
 * @brief     driver w25qxx read test header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-07-15
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/07/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_W25QXX_READ_TEST_H
#define DRIVER_W25QXX_READ_TEST_H

#include "driver_w25qxx_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup w25qxx_test_driver
 * @{
 */

/**
 * @brief w25qxx enable erase read test definition
 */
#define W25QXX_ENABLE_ERASE_READ_TEST    1    /**< enable */

/**
 * @brief     read test
 * @param[in] type chip type
 * @param[in] interface chip interface
 * @param[in] dual_quad_spi_enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t w25qxx_read_test(w25qxx_type_t type, w25qxx_interface_t interface, w25qxx_bool_t dual_quad_spi_enable);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
