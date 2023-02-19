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
 * @date      2022-11-11
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/11/11  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef SPI_H
#define SPI_H

#include <linux/spi/spi.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup spi spi function
 * @brief    spi function modules
 * @{
 */

/**
 * @brief spi mode type enumeration definition
 */
typedef enum  
{
    SPI_MODE_TYPE_0 = SPI_MODE_0,        /**< mode 0 */
    SPI_MODE_TYPE_1 = SPI_MODE_1,        /**< mode 1 */
    SPI_MODE_TYPE_2 = SPI_MODE_2,        /**< mode 2 */
    SPI_MODE_TYPE_3 = SPI_MODE_3,        /**< mode 3 */
} spi_mode_type_t;

/**
 * @brief      spi bus init
 * @param[in]  *name points to a spi device name buffer
 * @param[out] *fd points to a spi device handle buffer
 * @param[in]  mode is the spi mode.
 * @param[in]  freq is the spi running frequence
 * @return     status code
 *             - 0 success
 *             - 1 init failed
 * @note       none
 */
uint8_t spi_init(char *name, int *fd, spi_mode_type_t mode, uint32_t freq);

/**
 * @brief     spi bus deinit
 * @param[in] fd is the spi handle
 * @return    status code
 *            - 0 success
 *            - 1 deinit failed
 * @note      none
 */
uint8_t spi_deinit(int fd);

/**
 * @brief      spi bus read command
 * @param[in]  fd is the spi handle
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t spi_read_cmd(int fd, uint8_t *buf, uint16_t len);

/**
 * @brief      spi bus read
 * @param[in]  fd is the spi handle
 * @param[in]  reg is the spi register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t spi_read(int fd, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      spi bus read address 16
 * @param[in]  fd is the spi handle
 * @param[in]  reg is the spi register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t spi_read_address16(int fd, uint16_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief     spi bus write command
 * @param[in] fd is the spi handle
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t spi_write_cmd(int fd, uint8_t *buf, uint16_t len);

/**
 * @brief     spi bus write
 * @param[in] fd is the spi handle
 * @param[in] reg is the spi register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t spi_write(int fd, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief     spi bus write address 16
 * @param[in] fd is the spi handle
 * @param[in] reg is the spi register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t spi_write_address16(int fd, uint16_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      spi bus write read
 * @param[in]  fd is the spi handle
 * @param[in]  *in_buf points to an input buffer
 * @param[in]  in_len is the input length
 * @param[out] *out_buf points to an output buffer
 * @param[in]  out_len is the output length
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       none
 */
uint8_t spi_write_read(int fd, uint8_t *in_buf, uint32_t in_len, uint8_t *out_buf, uint32_t out_len);

/**
 * @brief      spi transmit
 * @param[in]  fd is the spi handle
 * @param[in]  *tx points to a tx buffer
 * @param[out] *rx points to a rx buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 transmit failed
 * @note       none
 */
uint8_t spi_transmit(int fd, uint8_t *tx, uint8_t *rx, uint16_t len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
