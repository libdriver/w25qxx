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
 * @file      spi.c
 * @brief     spi source file
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

#include "spi.h"
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

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
uint8_t spi_init(char *name, int *fd, spi_mode_type_t mode, uint32_t freq)
{
    int i;
    
    /* open the spi device */
    *fd = open(name, O_RDWR);
    if ((*fd) < 0)
    {
        perror("spi: open failed.\n");
        
        return 1;
    }
    else
    {
        /* set the spi write mode */
        i = mode;
        if (ioctl(*fd, SPI_IOC_WR_MODE, &i) < 0)
        {
            perror("spi: write mode set failed.\n");
            
            return 1;
        }
        
        /* set the spi read mode */
        if (ioctl(*fd, SPI_IOC_RD_MODE, &i) < 0)
        {
            perror("spi: read mode set failed.\n");
            
            return 1;
        }
        
        /* set the spi write frequence */
        i = freq;
        if (ioctl(*fd, SPI_IOC_WR_MAX_SPEED_HZ, &i) < 0)
        {
            perror("spi: set spi write speed failed.\n");
            
            return 1;
        }
        
        /* set the spi read frequence */
        if (ioctl(*fd, SPI_IOC_RD_MAX_SPEED_HZ, &i) < 0)
        {
            perror("spi: set spi read speed failed.\n");
            
            return 1;
        }
        
        /* set the spi write MSB first */
        i = 0;
        if (ioctl(*fd, SPI_IOC_WR_LSB_FIRST, &i) < 0)
        {
            perror("spi: set spi write msb first failed.\n");
            
            return 1;
        }
        
        /* set the spi read MSB first */
        if (ioctl(*fd, SPI_IOC_RD_LSB_FIRST, &i) < 0)
        {
            perror("spi: set spi read msb first failed.\n");
            
            return 1;
        }
        
        /* set the spi write 8 bits mode */
        i = 8;
        if (ioctl(*fd, SPI_IOC_WR_BITS_PER_WORD, &i) < 0)
        {
            perror("spi: set spi wirte 8 bit failed.\n");
            
            return 1;
        }
        
        /* set the spi read 8 bits mode */
        if (ioctl(*fd, SPI_IOC_RD_BITS_PER_WORD, &i) < 0)
        {
            perror("spi: set spi read 8 bit failed.\n");
            
            return 1;
        }
        
        return 0;
    }
}

/**
 * @brief     spi bus deinit
 * @param[in] fd is the spi handle
 * @return    status code
 *            - 0 success
 *            - 1 deinit failed
 * @note      none
 */
uint8_t spi_deinit(int fd)
{
    /* close the spi */
    if (close(fd) < 0)
    {
        perror("spi: close failed.\n");
        
        return 1;
    }
    else
    {
        return 0;
    }
}

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
uint8_t spi_read_cmd(int fd, uint8_t *buf, uint16_t len)
{
    struct spi_ioc_transfer k;
    int l;
    
    /* clear ioc transfer */
    memset(&k, 0, sizeof(struct spi_ioc_transfer));
    
    /* set the param */
    k.rx_buf = (unsigned long)buf;
    k.len = len;
    k.cs_change = 0;
    
    /* transmit */
    l = ioctl(fd, SPI_IOC_MESSAGE(1), &k);
    if (l != len)
    {
        perror("spi: length check error.\n");
        
        return 1;
    }
    
    return 0;
}

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
uint8_t spi_read(int fd, uint8_t reg, uint8_t *buf, uint16_t len)
{
    struct spi_ioc_transfer k;
    uint8_t command[len + 1];
    uint8_t recv[len + 1];
    int l;
    
    /* set the command */
    command[0] = reg;
    memset(&command[1], 0x00, len);
    
    /* clear ioc transfer */
    memset(&k, 0, sizeof(struct spi_ioc_transfer));
    
    /* set the param */
    k.tx_buf = (unsigned long)command;
    k.rx_buf = (unsigned long)recv;
    k.len = len + 1;
    k.cs_change = 0;
    
    /* transmit */
    l = ioctl(fd, SPI_IOC_MESSAGE(1), &k);
    if (l != k.len)
    {
        perror("spi: length check error.\n");
        
        return 1;
    }
    
    /* copy the data */
    memcpy(buf, recv + 1, len);
    
    return 0;
}

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
uint8_t spi_read_address16(int fd, uint16_t reg, uint8_t *buf, uint16_t len)
{
    struct spi_ioc_transfer k;
    uint8_t command[len + 2];
    uint8_t recv[len + 2];
    int l;
    
    /* set the command */
    command[0] = (reg >> 8) & 0xFF;
    command[1] = reg & 0xFF;
    memset(&command[2], 0x00, len);
    
    /* clear ioc transfer */
    memset(&k, 0, sizeof(struct spi_ioc_transfer));
    
    /* set the param */
    k.tx_buf = (unsigned long)command;
    k.rx_buf = (unsigned long)recv;
    k.len = len + 2;
    k.cs_change = 0;
    
    /* transmit */
    l = ioctl(fd, SPI_IOC_MESSAGE(1), &k);
    if (l != k.len)
    {
        perror("spi: length check error.\n");
        
        return 1;
    }
    
    /* copy the data */
    memcpy(buf, recv + 2, len);
    
    return 0;
}

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
uint8_t spi_write_cmd(int fd, uint8_t *buf, uint16_t len)
{
    struct spi_ioc_transfer k;
    int l;
    
    /* clear ioc transfer */
    memset(&k, 0, sizeof(k));
    
    /* set the param */
    k.tx_buf = (unsigned long)buf;
    k.len = len;
    k.cs_change = 0;
    
    /* transmit */
    l = ioctl(fd, SPI_IOC_MESSAGE(1), &k);
    if (l != len)
    {
        perror("spi: length check error.\n");
        
        return 1;
    }
    
    return 0;
}

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
uint8_t spi_write(int fd, uint8_t reg, uint8_t *buf, uint16_t len)
{
    struct spi_ioc_transfer k;
    uint8_t command[len + 1];
    int l;
    
    /* set the command */
    command[0] = reg;
    memcpy(&command[1], buf, len);
    
    /* clear ioc transfer */
    memset(&k, 0, sizeof(struct spi_ioc_transfer));
    
    /* set the param */
    k.tx_buf = (unsigned long)command;
    k.len = len + 1;
    k.cs_change = 0;
    
    /* transmit */
    l = ioctl(fd, SPI_IOC_MESSAGE(1), &k);
    if (l != k.len)
    {
        perror("spi: length check error.\n");
        
        return 1;
    }
    
    return 0;
}

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
uint8_t spi_write_address16(int fd, uint16_t reg, uint8_t *buf, uint16_t len)
{
    struct spi_ioc_transfer k;
    uint8_t command[len + 2];
    int l;
    
    /* set the command */
    command[0] = (reg >> 8) & 0xFF;
    command[1] = reg & 0xFF;
    memcpy(&command[2], buf, len);
    
    /* clear ioc transfer */
    memset(&k, 0, sizeof(struct spi_ioc_transfer));
    
    /* set the param */
    k.tx_buf = (unsigned long)command;
    k.len = len + 2;
    k.cs_change = 0;
    
    /* transmit */
    l = ioctl(fd, SPI_IOC_MESSAGE(1), &k);
    if (l != k.len)
    {
        perror("spi: length check error.\n");
        
        return 1;
    }
    
    return 0;
}

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
uint8_t spi_write_read(int fd, uint8_t *in_buf, uint32_t in_len, uint8_t *out_buf, uint32_t out_len)
{
    if ((in_len > 0) && (out_len > 0))
    {
        struct spi_ioc_transfer k[2];
        int l;
        
        /* clear ioc transfer */
        memset(k, 0, sizeof(struct spi_ioc_transfer) * 2);
        
        /* set the param */
        k[0].tx_buf = (unsigned long)in_buf;
        k[0].len = in_len;
        k[0].cs_change = 0;
        k[1].rx_buf = (unsigned long)out_buf;
        k[1].len = out_len;
        k[1].cs_change = 0;
        
        /* transmit */
        l = ioctl(fd, SPI_IOC_MESSAGE(2), &k);
        if (l != (k[0].len + k[1].len))
        {
            perror("spi: length check error.\n");
            
            return 1;
        }
        
        return 0;
    }
    else if (in_len > 0)
    {
        struct spi_ioc_transfer k;
        int l;
        
        /* clear ioc transfer */
        memset(&k, 0, sizeof(struct spi_ioc_transfer));
        
        /* set the param */
        k.tx_buf = (unsigned long)in_buf;
        k.len = in_len;
        k.cs_change = 0;
        
        /* transmit */
        l = ioctl(fd, SPI_IOC_MESSAGE(1), &k);
        if (l != k.len)
        {
            perror("spi: length check error.\n");
            
            return 1;
        }
        
        return 0;
    }
    else
    {
        return 1;
    }
}

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
uint8_t spi_transmit(int fd, uint8_t *tx, uint8_t *rx, uint16_t len)
{
    int l;
    struct spi_ioc_transfer k;
    
    /* clear ioc transfer */
    memset(&k, 0, sizeof(struct spi_ioc_transfer));
    
    /* set the param */
    k.tx_buf = (unsigned long)tx;
    k.rx_buf = (unsigned long)rx;
    k.len = len;
    k.cs_change = 0;
    
    /* transmit */
    l = ioctl(fd, SPI_IOC_MESSAGE(1), &k);
    if (l != k.len)
    {
        perror("spi: length check error.\n");
        
        return 1;
    }
    
    return 0;
}
