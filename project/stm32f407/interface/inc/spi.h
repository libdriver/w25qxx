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
 * @file      spi.h
 * @brief     spi header file
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

#ifndef SPI_H
#define SPI_H

#include "stm32f4xx_hal.h"

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @defgroup spi spi function
 * @brief    spi function modules
 * @{
 */

/**
 * @brief spi mode enumeration definition
 */
typedef enum
{
    SPI_MODE_0 = 0x00,        /**< mode 0 */
    SPI_MODE_1 = 0x01,        /**< mode 1 */
    SPI_MODE_2 = 0x02,        /**< mode 2 */
    SPI_MODE_3 = 0x03,        /**< mode 3 */
}spi_mode_t;

/**
 * @brief  spi bus init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 *         - 2 mode is invalid
 * @note   SCLK is PA5, MOSI is PA7 MISO is PA6 and CS is PA4
 */
uint8_t spi_init(spi_mode_t mode);

/**
 * @brief  spi bus deinit
 * @return status code
 *         - 0 success
 * @note   SCLK is PA5, MOSI is PA7 MISO is PA6 and CS is PA4
 */
uint8_t spi_deinit(void);

/**
 * @brief      spi bus read
 * @param[in]  addr is the spi register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       SCLK is PA5, MOSI is PA7 MISO is PA6 and CS is PA4
 */
uint8_t spi_read(uint8_t addr, uint8_t *buf, uint16_t len);

/**
 * @brief      spi bus read command
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       SCLK is PA5, MOSI is PA7 MISO is PA6 and CS is PA4
 */
uint8_t spi_read_cmd(uint8_t *buf, uint16_t len);

/**
 * @brief     spi bus write
 * @param[in] addr is the spi register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      SCLK is PA5, MOSI is PA7 MISO is PA6 and CS is PA4
 */
uint8_t spi_write(uint8_t addr, uint8_t *buf, uint16_t len);

/**
 * @brief     spi bus write command
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      SCLK is PA5, MOSI is PA7 MISO is PA6 and CS is PA4
 */
uint8_t spi_write_cmd(uint8_t *buf, uint16_t len);

/**
 * @brief      spi bus write read
 * @param[in]  *in_buf points to a input buffer
 * @param[in]  in_len is the input length
 * @param[out] *out_buf points to a output buffer
 * @param[in]  out_len is the output length
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       SCLK is PA5, MOSI is PA7 MISO is PA6 and CS is PA4
 */
uint8_t spi_write_read(uint8_t *in_buf, uint32_t in_len, uint8_t *out_buf, uint32_t out_len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
