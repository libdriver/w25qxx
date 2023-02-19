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

/**
 * @brief spi var definition
 */
SPI_HandleTypeDef g_spi_handle;        /**< spi handle */

/**
 * @brief  spi cs init
 * @return status code
 *         - 0 success
 * @note   none
 */
static uint8_t a_spi_cs_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* enable cs gpio clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    /* gpio init */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    return 0;
}

/**
 * @brief     spi bus init
 * @param[in] mode is the spi mode
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      SCLK is PA5, MOSI is PA7 MISO is PA6 and CS is PA4
 */
uint8_t spi_init(spi_mode_t mode)
{
    g_spi_handle.Instance = SPI1;
    g_spi_handle.Init.Mode = SPI_MODE_MASTER;
    g_spi_handle.Init.Direction = SPI_DIRECTION_2LINES;
    g_spi_handle.Init.DataSize = SPI_DATASIZE_8BIT;
    
    /* set the mode */
    if (mode == SPI_MODE_0)
    {
        g_spi_handle.Init.CLKPolarity = SPI_POLARITY_LOW;
        g_spi_handle.Init.CLKPhase = SPI_PHASE_1EDGE;
    }
    else if (mode == SPI_MODE_1)
    {
        g_spi_handle.Init.CLKPolarity = SPI_POLARITY_LOW;
        g_spi_handle.Init.CLKPhase = SPI_PHASE_2EDGE;
    }
    else if (mode == SPI_MODE_2)
    {
        g_spi_handle.Init.CLKPolarity = SPI_POLARITY_HIGH;
        g_spi_handle.Init.CLKPhase = SPI_PHASE_1EDGE;
    }
    else
    {
        g_spi_handle.Init.CLKPolarity = SPI_POLARITY_HIGH;
        g_spi_handle.Init.CLKPhase = SPI_PHASE_2EDGE;
    }
    g_spi_handle.Init.NSS = SPI_NSS_SOFT;
    g_spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    g_spi_handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    g_spi_handle.Init.TIMode = SPI_TIMODE_DISABLE;
    g_spi_handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    g_spi_handle.Init.CRCPolynomial = 10;
    
    /* spi init */
    if (HAL_SPI_Init(&g_spi_handle) != HAL_OK)
    {
        return 1;
    }
    
    return a_spi_cs_init();
}

/**
 * @brief  spi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t spi_deinit(void)
{
    /* cs deinit */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);
    
    /* spi deinit */
    if (HAL_SPI_DeInit(&g_spi_handle) != HAL_OK)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     spi bus write command
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t spi_write_cmd(uint8_t *buf, uint16_t len)
{
    uint8_t res;
    
    /* set cs low */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    
    /* if len > 0 */
    if (len > 0)
    {
        /* transmit the buffer */
        res = HAL_SPI_Transmit(&g_spi_handle, buf, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
            
            return 1;
        }
    }
    
    /* set cs high */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    
    return 0;
}

/**
 * @brief     spi bus write
 * @param[in] addr is the spi register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t spi_write(uint8_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t buffer;
    uint8_t res;
    
    /* set cs low */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    
    /* transmit the addr */
    buffer = addr;
    res = HAL_SPI_Transmit(&g_spi_handle, (uint8_t *)&buffer, 1, 1000);
    if (res != HAL_OK)
    {
        /* set cs high */
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
        
        return 1;
    }
    
    /* if len > 0 */
    if (len > 0)
    {
        /* transmit the buffer */
        res = HAL_SPI_Transmit(&g_spi_handle, buf, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
            
            return 1;
        }
    }
    
    /* set cs high */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    
    return 0;
}

/**
 * @brief     spi bus write address 16
 * @param[in] addr is the spi register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t spi_write_address16(uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t buffer[2];
    uint8_t res;
    
    /* set cs low */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    
    /* transmit the addr  */
    buffer[0] = (addr >> 8) & 0xFF;
    buffer[1] = addr & 0xFF;
    res = HAL_SPI_Transmit(&g_spi_handle, (uint8_t *)buffer, 2, 1000);
    if (res != HAL_OK)
    {
        /* set cs high */
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
       
        return 1;
    }
    
    /* if len > 0 */
    if (len > 0)
    {
        /* transmit the buffer */
        res = HAL_SPI_Transmit(&g_spi_handle, buf, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
           
            return 1;
        }
    }
    
    /* set cs high */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    
    return 0;
}

/**
 * @brief      spi bus read command
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t spi_read_cmd(uint8_t *buf, uint16_t len)
{
    uint8_t res;
    
    /* set cs low */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    
    /* if len > 0 */
    if (len > 0)
    {
        /* receive to the buffer */
        res = HAL_SPI_Receive(&g_spi_handle, buf, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
            
            return 1;
        }
    }
    
    /* set cs high */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    
    return 0;
}

/**
 * @brief      spi bus read
 * @param[in]  addr is the spi register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t spi_read(uint8_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t buffer;
    uint8_t res;
    
    /* set cs low */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    
    /* transmit the addr */
    buffer = addr;
    res = HAL_SPI_Transmit(&g_spi_handle, (uint8_t *)&buffer, 1, 1000);
    if (res != HAL_OK)
    {
        /* set cs high */
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
       
        return 1;
    }
    
    /* if len > 0 */
    if (len > 0)
    {
        /* receive to the buffer */
        res = HAL_SPI_Receive(&g_spi_handle, buf, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
           
            return 1;
        }
    }
    
    /* set cs high */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    
    return 0;
}

/**
 * @brief      spi bus read address 16
 * @param[in]  addr is the spi register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t spi_read_address16(uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t buffer[2];
    uint8_t res;
    
    /* set cs low */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    
    /* transmit the addr  */
    buffer[0] = (addr >> 8) & 0xFF;
    buffer[1] = addr & 0xFF;
    res = HAL_SPI_Transmit(&g_spi_handle, (uint8_t *)buffer, 2, 1000);
    if (res != HAL_OK)
    {
        /* set cs high */
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
       
        return 1;
    }
    
    /* if len > 0 */
    if (len > 0)
    {
        /* receive to the buffer */
        res = HAL_SPI_Receive(&g_spi_handle, buf, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
           
            return 1;
        }
    }
    
    /* set cs high */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    
    return 0;
}

/**
 * @brief      spi transmit
 * @param[in]  *tx points to a tx buffer
 * @param[out] *rx points to a rx buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 transmit failed
 * @note       none
 */
uint8_t spi_transmit(uint8_t *tx, uint8_t *rx, uint16_t len)
{
    uint8_t res;
    
    /* set cs low */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    
    /* if len > 0 */
    if (len > 0)
    {
        /* transmit */
        res = HAL_SPI_TransmitReceive(&g_spi_handle, tx, rx, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
            
            return 1;
        }
    }
    
    /* set cs high */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    
    return 0;
}

/**
 * @brief      spi bus write read
 * @param[in]  *in_buf points to an input buffer
 * @param[in]  in_len is the input length
 * @param[out] *out_buf points to an output buffer
 * @param[in]  out_len is the output length
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       none
 */
uint8_t spi_write_read(uint8_t *in_buf, uint32_t in_len, uint8_t *out_buf, uint32_t out_len)
{
    uint8_t res;
    
    /* set cs low */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    
    /* if in_len > 0 */
    if (in_len > 0)
    {
        /* transmit the input buffer */
        res = HAL_SPI_Transmit(&g_spi_handle, in_buf, in_len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
           
            return 1;
        }
    }
    
    /* if out_len > 0 */
    if (out_len > 0)
    {
        /* transmit to the output buffer */
        res = HAL_SPI_Receive(&g_spi_handle, out_buf, out_len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
           
            return 1;
        }
    }
    
    /* set cs high */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    
    return 0;
}
