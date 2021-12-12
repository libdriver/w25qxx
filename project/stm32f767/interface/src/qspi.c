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
 * @file      qspi.c
 * @brief     qspi source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-08-07
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/08/07  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "qspi.h"

/**
 * @brief qspi var definition
 */
QSPI_HandleTypeDef g_qspi_handle;        /**< qspi handle */

/**
 * @brief     qspi bus init
 * @param[in] mode is the qspi mode
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 *            - 2 mode is invalid
 * @note      SCLK is PB2, CS is PB6, IO0 is PF8, IO1 is PF9, IO2 is PF7 and IO3 is PF6
 */
uint8_t qspi_init(qspi_mode_t mode)
{
    g_qspi_handle.Instance = QUADSPI;
    g_qspi_handle.Init.ClockPrescaler = 128;
    g_qspi_handle.Init.FifoThreshold = 4;
    g_qspi_handle.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
    g_qspi_handle.Init.FlashSize = POSITION_VAL(0X2000000) - 1;
    g_qspi_handle.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_4_CYCLE;
    if (mode == QSPI_MODE_0)
    {
        g_qspi_handle.Init.ClockMode = QSPI_CLOCK_MODE_0;
    }
    else if (mode == QSPI_CLOCK_MODE_3)
    {
        g_qspi_handle.Init.ClockMode = QSPI_CLOCK_MODE_3;
    }
    else
    {
        return 2;
    }
    g_qspi_handle.Init.FlashID = QSPI_FLASH_ID_1;
    g_qspi_handle.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
    
    if (HAL_QSPI_Init(&g_qspi_handle) != HAL_OK)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      qspi interface write and read bytes
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[in]  instruction is the sent instruction
 * @param[in]  instruction_line is the instruction phy lines
 * @param[in]  address is the register address
 * @param[in]  address_line is the address phy lines
 * @param[in]  address_len is the address length
 * @param[in]  alternate is the register address
 * @param[in]  alternate_line is the alternate phy lines
 * @param[in]  alternate_len is the alternate length
 * @param[in]  dummy is the dummy cycle
 * @param[in]  *in_buf points to a input buffer
 * @param[in]  in_len is the input length
 * @param[out] *out_buf points to a output buffer
 * @param[in]  out_len is the output length
 * @param[in]  data_line is the data phy lines
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 *             - 2 param is invalid
 * @note       none
 */
uint8_t qspi_write_read(uint8_t instruction, uint8_t instruction_line,
                        uint32_t address, uint8_t address_line, uint8_t address_len,
                        uint32_t alternate, uint8_t alternate_line, uint8_t alternate_len,
                        uint8_t dummy, uint8_t *in_buf, uint32_t in_len,
                        uint8_t *out_buf, uint32_t out_len, uint8_t data_line)
{
    QSPI_CommandTypeDef cmd_handler;
    
    if (in_len && out_len)
    {
        return 2;
    }
    
    cmd_handler.Instruction = instruction;
    if (instruction_line == 0)
    {
        cmd_handler.InstructionMode = QSPI_INSTRUCTION_NONE;
    }
    else if (instruction_line == 1)
    {
        cmd_handler.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    }
    else if (instruction_line == 2)
    {
        cmd_handler.InstructionMode = QSPI_INSTRUCTION_2_LINES;
    }
    else if (instruction_line == 4)
    {
        cmd_handler.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    }
    else
    {
        return 2;
    }
    
    if (address_line == 0)
    {
        cmd_handler.AddressMode = QSPI_ADDRESS_NONE;
    }
    else if (address_line == 1)
    {
        cmd_handler.AddressMode = QSPI_ADDRESS_1_LINE;
    }
    else if (address_line == 2)
    {
        cmd_handler.AddressMode = QSPI_ADDRESS_2_LINES;
    }
    else if (address_line == 4)
    {
        cmd_handler.AddressMode = QSPI_ADDRESS_4_LINES;
    }
    else
    {
        return 2;
    }
    if (address_len == 0)
    {
        cmd_handler.Address = address & 0xFF;
        cmd_handler.AddressSize = QSPI_ADDRESS_8_BITS;
    }
    else if (address_len == 1)
    {
        cmd_handler.Address = address & 0xFF;
        cmd_handler.AddressSize = QSPI_ADDRESS_8_BITS;
    }
    else if (address_len == 2)
    {
        cmd_handler.Address = address & 0xFFFF;
        cmd_handler.AddressSize = QSPI_ADDRESS_16_BITS;
    }
    else if (address_len == 3)
    {
        cmd_handler.Address = address & 0xFFFFFF;
        cmd_handler.AddressSize = QSPI_ADDRESS_24_BITS;
    }
    else if (address_len == 4)
    {
        cmd_handler.Address = address & 0xFFFFFFFF;
        cmd_handler.AddressSize = QSPI_ADDRESS_32_BITS;
    }
    else
    {
        return 2;
    }
    
    cmd_handler.AlternateBytes = alternate;
    if (alternate_line == 0)
    {
        cmd_handler.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    }
    else if (alternate_line == 1)
    {
        cmd_handler.AlternateByteMode = QSPI_ALTERNATE_BYTES_1_LINE;
    }
    else if (alternate_line == 2)
    {
        cmd_handler.AlternateByteMode = QSPI_ALTERNATE_BYTES_2_LINES;
    }
    else if (alternate_line == 4)
    {
        cmd_handler.AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES;
    }
    else
    {
        return 2;
    }
    if (alternate_len == 0)
    {
        cmd_handler.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
    }
    else if (alternate_len == 1)
    {
        cmd_handler.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
    }
    else if (alternate_len == 2)
    {
        cmd_handler.AlternateBytesSize = QSPI_ALTERNATE_BYTES_16_BITS;
    }
    else if (alternate_len == 3)
    {
        cmd_handler.AlternateBytesSize = QSPI_ALTERNATE_BYTES_24_BITS;
    }
    else if (alternate_len == 4)
    {
        cmd_handler.AlternateBytesSize = QSPI_ALTERNATE_BYTES_32_BITS;
    }
    else
    {
        return 2;
    }
    
    cmd_handler.DummyCycles = dummy;
    
    if (data_line == 0)
    {
        cmd_handler.DataMode = QSPI_DATA_NONE;
    }
    else if (data_line == 1)
    {
        cmd_handler.DataMode = QSPI_DATA_1_LINE;
    }
    else if (data_line == 2)
    {
        cmd_handler.DataMode = QSPI_DATA_2_LINES;
    }
    else if (data_line == 4)
    {
        cmd_handler.DataMode = QSPI_DATA_4_LINES;
    }
    else
    {
        return 2;
    }
    
    cmd_handler.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    cmd_handler.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd_handler.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    
    if (HAL_QSPI_Command(&g_qspi_handle, &cmd_handler, 1000))
    {
        return 1;
    }
    
    if (in_len)
    {
        g_qspi_handle.Instance->DLR = in_len - 1;
        if (HAL_QSPI_Transmit(&g_qspi_handle, in_buf, 1000))
        {
            return 1;
        }
    }
    if (out_len)
    {
        g_qspi_handle.Instance->DLR = out_len - 1;
        if (HAL_QSPI_Receive(&g_qspi_handle, out_buf, 1000))
        {
            return 1;
        }
    }
    
    return 0;
}

/**
 * @brief  qspi bus deinit
 * @return status code
 *         - 0 success
 * @note   none
 */
uint8_t qspi_deinit(void)
{
    HAL_QSPI_DeInit(&g_qspi_handle);
    
    return 0;
}
