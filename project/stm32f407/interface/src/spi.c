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
 * @date      2021-2-12
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/02/12  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "spi.h"

/**
 * @brief spi var definition
 */
SPI_HandleTypeDef g_spi_handle;        /**< spi handle */

/**
  * @func   spi_cs_init(void)
  * @brief  spi cs init
  * @retval success return 0
  * @note   none
  */
static uint8_t _spi_cs_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
  
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    return 0;
}

/**
 * @brief  spi bus init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 *         - 2 mode is invalid
 * @note   SCLK is PA5, MOSI is PA7 MISO is PA6 and CS is PA4
 */
uint8_t spi_init(spi_mode_t mode)
{
    g_spi_handle.Instance = SPI1;
    g_spi_handle.Init.Mode = SPI_MODE_MASTER;
    g_spi_handle.Init.Direction = SPI_DIRECTION_2LINES;
    g_spi_handle.Init.DataSize = SPI_DATASIZE_8BIT;
    
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
    else if (mode == SPI_MODE_3)
    {
        g_spi_handle.Init.CLKPolarity = SPI_POLARITY_HIGH;
        g_spi_handle.Init.CLKPhase = SPI_PHASE_2EDGE;
    }
    else
    {
        return 2;
    }
    
    g_spi_handle.Init.NSS = SPI_NSS_SOFT;
    g_spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    g_spi_handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    g_spi_handle.Init.TIMode = SPI_TIMODE_DISABLE;
    g_spi_handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    g_spi_handle.Init.CRCPolynomial = 10;
    
    if (HAL_SPI_Init(&g_spi_handle))
    {
        return 1;
    }
    
    return _spi_cs_init();
}

/**
 * @brief  spi bus deinit
 * @return status code
 *         - 0 success
 * @note   SCLK is PA5, MOSI is PA7 MISO is PA6 and CS is PA4
 */
uint8_t spi_deinit(void)
{
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);
    HAL_SPI_DeInit(&g_spi_handle);
    
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
 * @note      SCLK is PA5, MOSI is PA7 MISO is PA6 and CS is PA4
 */
uint8_t spi_write(uint8_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t buffer;
    uint8_t res;
    
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    
    buffer = addr;
    res = HAL_SPI_Transmit(&g_spi_handle, (uint8_t *)&buffer, 1, 1000);
    if (res)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
        
        return 1;
    }
    
    res = HAL_SPI_Transmit(&g_spi_handle, buf, len, 1000);
    if (res)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
        
        return 1;
    }
    
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    
    return 0;
}

/**
 * @brief     spi bus write command
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      SCLK is PA5, MOSI is PA7 MISO is PA6 and CS is PA4
 */
uint8_t spi_write_cmd(uint8_t *buf, uint16_t len)
{
    uint8_t res;
    
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    res = HAL_SPI_Transmit(&g_spi_handle, buf, len, 1000);
    if (res)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
        
        return 1;
    }
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
 * @note       SCLK is PA5, MOSI is PA7 MISO is PA6 and CS is PA4
 */
uint8_t spi_read(uint8_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t buffer;
    uint8_t res;
    
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
   
    buffer = addr;
    res = HAL_SPI_Transmit(&g_spi_handle, (uint8_t *)&buffer, 1, 1000);
    if (res)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
       
        return 1;
    }
    res = HAL_SPI_Receive(&g_spi_handle, buf, len, 1000);
    if (res)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
       
        return 1;
    }

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
 * @note       SCLK is PA5, MOSI is PA7 MISO is PA6 and CS is PA4
 */
uint8_t spi_read_cmd(uint8_t *buf, uint16_t len)
{
    uint8_t res;
    
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    res = HAL_SPI_Receive(&g_spi_handle, buf, len, 1000);
    if (res)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
        
        return 1;
    }
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    
    return 0;
}

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
uint8_t spi_write_read(uint8_t *in_buf, uint32_t in_len, uint8_t *out_buf, uint32_t out_len)
{
    uint8_t res;
    
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
   
    if (in_len)
    {
        res = HAL_SPI_Transmit(&g_spi_handle, in_buf, in_len, 1000);
        if (res)
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
           
            return 1;
        }
    }
    if (out_len)
    {
        res = HAL_SPI_Receive(&g_spi_handle, out_buf, out_len, 1000);
        if (res)
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
           
            return 1;
        }
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

    return 0;
}
