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
 * @file      driver_w25qxx_read_test.c
 * @brief     driver w25qxx read test source file
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

#include "driver_w25qxx_read_test.h"
#include <stdlib.h>

static w25qxx_handle_t gs_handle;            /**< w25qxx handle */
static uint8_t gs_buffer_input[600];         /**< input buffer */
static uint8_t gs_buffer_output[600];        /**< output buffer */
static const uint32_t gsc_size[] = {0x100000, 0x200000, 0x400000, 0x800000, 0x1000000, 0x2000000};        /**< flash size */

/**
 * @brief     read test
 * @param[in] type is the chip type
 * @param[in] interface is the chip interface
 * @param[in] dual_quad_spi_enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t w25qxx_read_test(w25qxx_type_t type, w25qxx_interface_t interface, w25qxx_bool_t dual_quad_spi_enable)
{
    uint8_t res;
    w25qxx_info_t info;
    
    /* link interface function */
    DRIVER_W25QXX_LINK_INIT(&gs_handle, w25qxx_handle_t);
    DRIVER_W25QXX_LINK_SPI_QSPI_INIT(&gs_handle, w25qxx_interface_spi_qspi_init);
    DRIVER_W25QXX_LINK_SPI_QSPI_DEINIT(&gs_handle, w25qxx_interface_spi_qspi_deinit);
    DRIVER_W25QXX_LINK_SPI_QSPI_WRITE_READ(&gs_handle, w25qxx_interface_spi_qspi_write_read);
    DRIVER_W25QXX_LINK_DELAY_MS(&gs_handle, w25qxx_interface_delay_ms);
    DRIVER_W25QXX_LINK_DELAY_US(&gs_handle, w25qxx_interface_delay_us);
    DRIVER_W25QXX_LINK_DEBUG_PRINT(&gs_handle, w25qxx_interface_debug_print);
    
    /* get information */
    res = w25qxx_info(&info);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        w25qxx_interface_debug_print("w25qxx: chip is %s.\n", info.chip_name);
        w25qxx_interface_debug_print("w25qxx: manufacturer is %s.\n", info.manufacturer_name);
        w25qxx_interface_debug_print("w25qxx: interface is %s.\n", info.interface);
        w25qxx_interface_debug_print("w25qxx: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        w25qxx_interface_debug_print("w25qxx: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        w25qxx_interface_debug_print("w25qxx: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        w25qxx_interface_debug_print("w25qxx: max current is %0.2fmA.\n", info.max_current_ma);
        w25qxx_interface_debug_print("w25qxx: max temperature is %0.1fC.\n", info.temperature_max);
        w25qxx_interface_debug_print("w25qxx: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* set chip type */
    res = w25qxx_set_type(&gs_handle, type);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set type failed.\n");
       
        return 1;
    }
    
    /* set chip interface */
    res = w25qxx_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set interface failed.\n");
       
        return 1;
    }
    
    /* set dual quad spi */
    res = w25qxx_set_dual_quad_spi(&gs_handle, dual_quad_spi_enable);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set dual quad spi failed.\n");
        (void)w25qxx_deinit(&gs_handle);
       
        return 1;
    }
    
    /* chip init */
    res = w25qxx_init(&gs_handle);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: init failed.\n");
       
        return 1;
    }
    
    /* start read test */
    w25qxx_interface_debug_print("w25qxx: start read test.\n");
    
    if (interface == W25QXX_INTERFACE_SPI)
    {
        uint32_t size;
        uint32_t addr, step, j;
        
        size = gsc_size[type - W25Q80];
        step = size / 16;
        
        /* w25qxx_write/w25qxx_read test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_write/w25qxx_read test.\n");
        
        for (addr = 0; addr < size; addr += step)
        {
            for (j = 0; j < 600; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            res = w25qxx_write(&gs_handle, addr, gs_buffer_input, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: write failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            res = w25qxx_read(&gs_handle, addr, gs_buffer_output, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 600; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: 0x%08X/0x%08X successful.\n", addr, size);
        }
        
        /* w25qxx_sector_erase_4k test */
        addr = (rand() % 10) * 4 * 1024;
        w25qxx_interface_debug_print("w25qxx: w25qxx_sector_erase_4k test with address 0x%X.\n", addr);
        res = w25qxx_sector_erase_4k(&gs_handle, addr);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: sector erase 4k failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            gs_buffer_input[j] = rand() %256;
        }
        
        /* w25qxx_page_program */
        res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: page program failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* w25qxx_only_spi_read */
        res = w25qxx_only_spi_read(&gs_handle, addr, gs_buffer_output, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: only spi read failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: only spi read test passed.\n");
        
        /* w25qxx_fast_read */
        res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
        
        /* w25qxx_block_erase_32k test */
        addr = (rand() % 10) * 32 * 1024;
        w25qxx_interface_debug_print("w25qxx: w25qxx_block_erase_32k test with address 0x%X.\n", addr);
        res = w25qxx_block_erase_32k(&gs_handle, addr);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: sector erase 32k failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            gs_buffer_input[j] = rand() %256;
        }
        
        /* w25qxx_page_program */
        res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: page program failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* w25qxx_only_spi_read */
        res = w25qxx_only_spi_read(&gs_handle, addr, gs_buffer_output, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: only spi read failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: only spi read test passed.\n");
        
        /* w25qxx_fast_read */
        res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
        
        /* w25qxx_block_erase_64k test */
        addr = (rand() % 10) * 64 * 1024;
        w25qxx_interface_debug_print("w25qxx: w25qxx_block_erase_64k test with address 0x%X.\n", addr);
        res = w25qxx_block_erase_64k(&gs_handle, addr);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: sector erase 64k failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            gs_buffer_input[j] = rand() %256;
        }
        
        /* w25qxx_page_program */
        res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: page program failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* w25qxx_only_spi_read */
        res = w25qxx_only_spi_read(&gs_handle, addr, gs_buffer_output, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: only spi read failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: only spi read test passed.\n");
        
        /* w25qxx_fast_read */
        res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
        
        /* get sfdp */
        w25qxx_interface_debug_print("w25qxx: get sfdp.\n");
        memset(gs_buffer_output, 0, sizeof(uint8_t) * 256);
        res = w25qxx_get_sfdp(&gs_handle, (uint8_t *)gs_buffer_output);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: get sfdp failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j += 8)
        {
            w25qxx_interface_debug_print("w25qxx: sdfp[%d-%d] is 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\n", 
                                         j , j + 7,
                                         gs_buffer_output[j + 0], gs_buffer_output[j + 1], gs_buffer_output[j + 2],
                                         gs_buffer_output[j + 3], gs_buffer_output[j + 4], gs_buffer_output[j + 5],
                                         gs_buffer_output[j + 6], gs_buffer_output[j + 7]);
        }
        
        /*  security register1 write and read test */
        w25qxx_interface_debug_print("w25qxx: security register1 write and read test.\n");
        
        /* security register1 write and read test */
        res = w25qxx_erase_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_1);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: erase security register failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            gs_buffer_input[j] = rand() %256;
        }
        res = w25qxx_program_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_1, gs_buffer_input);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: program security register failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        res = w25qxx_read_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_1, gs_buffer_output);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: read security register failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: security register1 check passed.\n");
        
        /*  security register2 write and read test */
        w25qxx_interface_debug_print("w25qxx: security register2 write and read test.\n");
        
        /* security register2 write and read test */
        res = w25qxx_erase_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_2);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: erase security register failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            gs_buffer_input[j] = rand() %256;
        }
        res = w25qxx_program_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_2, gs_buffer_input);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: program security register failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        res = w25qxx_read_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_2, gs_buffer_output);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: read security register failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: security register2 check passed.\n");

        /*  security register3 write and read test */
        w25qxx_interface_debug_print("w25qxx: security register3 write and read test.\n");
        
        /* security register3 write and read test */
        res = w25qxx_erase_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_3);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: erase security register failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            gs_buffer_input[j] = rand() %256;
        }
        res = w25qxx_program_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_3, gs_buffer_input);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: program security register failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        res = w25qxx_read_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_3, gs_buffer_output);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: read security register failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: security register3 check passed.\n");
        
        #if (W25QXX_ENABLE_ERASE_READ_TEST == 1)
            /* start chip erasing */
            w25qxx_interface_debug_print("w25qxx: start chip erasing.\n");
            
            /* chip erase */
            w25qxx_interface_debug_print("w25qxx: w25qxx_chip_erase test.\n");
            res = w25qxx_chip_erase(&gs_handle);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: chip erase failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            w25qxx_interface_debug_print("w25qxx: chip erase successful.\n");
        #endif
        
        if (type >= W25Q256)
        {
            /* set address mode 4 byte */
            w25qxx_interface_debug_print("w25qxx: set address mode 4 byte.\n");
            
            /* set address mode 4 byte */
            res = w25qxx_set_address_mode(&gs_handle, W25QXX_ADDRESS_MODE_4_BYTE);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: set address mode failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_write/w25qxx_read test */
            w25qxx_interface_debug_print("w25qxx: w25qxx_write/w25qxx_read test.\n");
            
            for (addr = 0; addr < size; addr += step)
            {
                for (j = 0; j < 600; j++)
                {
                    gs_buffer_input[j] = rand() %256;
                }
                res = w25qxx_write(&gs_handle, addr, gs_buffer_input, 600);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: write failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                res = w25qxx_read(&gs_handle, addr, gs_buffer_output, 600);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: read failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                for (j = 0; j < 600; j++)
                {
                    if (gs_buffer_input[j] != gs_buffer_output[j])
                    {
                        w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                        (void)w25qxx_deinit(&gs_handle);
                        
                        return 1;
                    }
                }
                w25qxx_interface_debug_print("w25qxx: %d/%d successful.\n", addr, size);
            }
            
            /* w25qxx_sector_erase_4k test */
            addr = (rand() % 10) * 4 * 1024;
            w25qxx_interface_debug_print("w25qxx: w25qxx_sector_erase_4k test with address 0x%X.\n", addr);
            res = w25qxx_sector_erase_4k(&gs_handle, addr);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: sector erase 4k failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            
            /* w25qxx_page_program */
            res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: page program failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_only_spi_read */
            res = w25qxx_only_spi_read(&gs_handle, addr, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: only spi read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: only spi read test passed.\n");
            
            /* w25qxx_fast_read */
            res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
            
            /* w25qxx_block_erase_32k test */
            addr = (rand() % 10) * 32 * 1024;
            w25qxx_interface_debug_print("w25qxx: w25qxx_block_erase_32k test with address 0x%X.\n", addr);
            res = w25qxx_block_erase_32k(&gs_handle, addr);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: sector erase 32k failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            
            /* w25qxx_page_program */
            res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: page program failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_only_spi_read */
            res = w25qxx_only_spi_read(&gs_handle, addr, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: only spi read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: only spi read test passed.\n");
            
            /* w25qxx_fast_read */
            res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
            
            /* w25qxx_block_erase_64k test */
            addr = (rand() % 10) * 64 * 1024;
            w25qxx_interface_debug_print("w25qxx: w25qxx_block_erase_64k test with address 0x%X.\n", addr);
            res = w25qxx_block_erase_64k(&gs_handle, addr);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: sector erase 64k failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            
            /* w25qxx_page_program */
            res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: page program failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_only_spi_read */
            res = w25qxx_only_spi_read(&gs_handle, addr, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: only spi read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: only spi read test passed.\n");
            
            /* w25qxx_fast_read */
            res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
            
            /* get sfdp */
            w25qxx_interface_debug_print("w25qxx: get sfdp.\n");
            memset(gs_buffer_output, 0, sizeof(uint8_t) * 256);
            res = w25qxx_get_sfdp(&gs_handle, (uint8_t *)gs_buffer_output);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: get sfdp failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j += 8)
            {
                w25qxx_interface_debug_print("w25qxx: sdfp[%d-%d] is 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\n", 
                                             j , j + 7,
                                             gs_buffer_output[j + 0], gs_buffer_output[j + 1], gs_buffer_output[j + 2],
                                             gs_buffer_output[j + 3], gs_buffer_output[j + 4], gs_buffer_output[j + 5],
                                             gs_buffer_output[j + 6], gs_buffer_output[j + 7]);
            }
            
            /*  security register1 write and read test */
            w25qxx_interface_debug_print("w25qxx: security register1 write and read test.\n");
            
            /* security register1 write and read test */
            res = w25qxx_erase_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_1);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: erase security register failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            res = w25qxx_program_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_1, gs_buffer_input);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: program security register failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            res = w25qxx_read_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_1, gs_buffer_output);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read security register failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: security register1 check passed.\n");
            
            /*  security register2 write and read test */
            w25qxx_interface_debug_print("w25qxx: security register2 write and read test.\n");
            
            /* security register2 write and read test */
            res = w25qxx_erase_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_2);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: erase security register failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            res = w25qxx_program_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_2, gs_buffer_input);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: program security register failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            res = w25qxx_read_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_2, gs_buffer_output);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read security register failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: security register2 check passed.\n");

            /*  security register3 write and read test */
            w25qxx_interface_debug_print("w25qxx: security register3 write and read test.\n");
            
            /* security register3 write and read test */
            res = w25qxx_erase_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_3);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: erase security register failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            res = w25qxx_program_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_3, gs_buffer_input);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: program security register failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            res = w25qxx_read_security_register(&gs_handle, W25QXX_SECURITY_REGISTER_3, gs_buffer_output);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read security register failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: security register3 check passed.\n");
            
            /* set address mode 3 byte */
            w25qxx_interface_debug_print("w25qxx: set address mode 3 byte.\n");
            
            /* set address mode 3 byte */
            res = w25qxx_set_address_mode(&gs_handle, W25QXX_ADDRESS_MODE_3_BYTE);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: set address mode failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
        }
    }
    else
    {
        uint32_t size;
        uint32_t addr, step, j;
        
        size = gsc_size[type - W25Q80];
        step = size / 16;
        
        /* w25qxx_write/w25qxx_read test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_write/w25qxx_read test.\n");
        
        for (addr = 0; addr < size; addr += step)
        {
            for (j = 0; j < 600; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            res = w25qxx_write(&gs_handle, addr, gs_buffer_input, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: write failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            res = w25qxx_read(&gs_handle, addr, gs_buffer_output, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 600; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: 0x%08X/0x%08X successful.\n", addr, size);
        }
        
        /* w25qxx_sector_erase_4k test */
        addr = (rand() % 10) * 4 * 1024;
        w25qxx_interface_debug_print("w25qxx: w25qxx_sector_erase_4k test with address 0x%X.\n", addr);
        res = w25qxx_sector_erase_4k(&gs_handle, addr);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: sector erase 4k failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            gs_buffer_input[j] = rand() %256;
        }
        
        /* w25qxx_page_program */
        res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: page program failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* w25qxx_fast_read */
        res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
        
        /* w25qxx_block_erase_32k test */
        addr = (rand() % 10) * 32 * 1024;
        w25qxx_interface_debug_print("w25qxx: w25qxx_block_erase_32k test with address 0x%X.\n", addr);
        res = w25qxx_block_erase_32k(&gs_handle, addr);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: sector erase 32k failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            gs_buffer_input[j] = rand() %256;
        }
        
        /* w25qxx_page_program */
        res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: page program failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* w25qxx_fast_read */
        res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
        
        /* w25qxx_block_erase_64k test */
        addr = (rand() % 10) * 64 * 1024;
        w25qxx_interface_debug_print("w25qxx: w25qxx_block_erase_64k test with address 0x%X.\n", addr);
        res = w25qxx_block_erase_64k(&gs_handle, addr);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: sector erase 64k failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            gs_buffer_input[j] = rand() %256;
        }
        
        /* w25qxx_page_program */
        res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: page program failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* w25qxx_fast_read */
        res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        for (j = 0; j < 256; j++)
        {
            if (gs_buffer_input[j] != gs_buffer_output[j])
            {
                w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                (void)w25qxx_deinit(&gs_handle);
                
                return 1;
            }
        }
        w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
        
        #if (W25QXX_ENABLE_ERASE_READ_TEST == 1)
            /* start chip erasing */
            w25qxx_interface_debug_print("w25qxx: start chip erasing.\n");
            
            /* chip erase */
            w25qxx_interface_debug_print("w25qxx: w25qxx_chip_erase test.\n");
            res = w25qxx_chip_erase(&gs_handle);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: chip erase failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            w25qxx_interface_debug_print("w25qxx: chip erase successful.\n");
        #endif
        
        if (type >= W25Q256)
        {
            /* set address mode 4 byte */
            w25qxx_interface_debug_print("w25qxx: set address mode 4 byte.\n");
            
            /* set address mode 4 byte */
            res = w25qxx_set_address_mode(&gs_handle, W25QXX_ADDRESS_MODE_4_BYTE);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: set address mode failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_write/w25qxx_read test */
            w25qxx_interface_debug_print("w25qxx: w25qxx_write/w25qxx_read test.\n");
            
            for (addr = 0; addr < size; addr += step)
            {
                for (j = 0; j < 600; j++)
                {
                    gs_buffer_input[j] = rand() %256;
                }
                res = w25qxx_write(&gs_handle, addr, gs_buffer_input, 600);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: write failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                res = w25qxx_read(&gs_handle, addr, gs_buffer_output, 600);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: read failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                for (j = 0; j < 600; j++)
                {
                    if (gs_buffer_input[j] != gs_buffer_output[j])
                    {
                        w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                        (void)w25qxx_deinit(&gs_handle);
                        
                        return 1;
                    }
                }
                w25qxx_interface_debug_print("w25qxx: 0x%08X/0x%08X successful.\n", addr, size);
            }
            
            /* w25qxx_sector_erase_4k test */
            addr = (rand() % 10) * 4 * 1024;
            w25qxx_interface_debug_print("w25qxx: w25qxx_sector_erase_4k test with address 0x%X.\n", addr);
            res = w25qxx_sector_erase_4k(&gs_handle, addr);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: sector erase 4k failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            
            /* w25qxx_page_program */
            res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: page program failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_fast_read */
            res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
            
            /* w25qxx_block_erase_32k test */
            addr = (rand() % 10) * 32 * 1024;
            w25qxx_interface_debug_print("w25qxx: w25qxx_block_erase_32k test with address 0x%X.\n", addr);
            res = w25qxx_block_erase_32k(&gs_handle, addr);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: sector erase 32k failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            
            /* w25qxx_page_program */
            res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: page program failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_fast_read */
            res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
            
            /* w25qxx_block_erase_64k test */
            addr = (rand() % 10) * 64 * 1024;
            w25qxx_interface_debug_print("w25qxx: w25qxx_block_erase_64k test with address 0x%X.\n", addr);
            res = w25qxx_block_erase_64k(&gs_handle, addr);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: sector erase 64k failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            
            /* w25qxx_page_program */
            res = w25qxx_page_program(&gs_handle, addr, gs_buffer_input, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: page program failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_fast_read */
            res = w25qxx_fast_read(&gs_handle, addr, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: fast read test passed.\n");
            
            /* set address mode 3 byte */
            w25qxx_interface_debug_print("w25qxx: set address mode 3 byte.\n");
            
            /* set address mode 3 byte */
            res = w25qxx_set_address_mode(&gs_handle, W25QXX_ADDRESS_MODE_3_BYTE);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: set address mode failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
        }
        
        if (dual_quad_spi_enable != 0)
        {
            /* enter to spi mode */
            w25qxx_interface_debug_print("w25qxx: enter to spi mode.\n");
            
            /* exit qspi */
            res = w25qxx_exit_qspi_mode(&gs_handle);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: exit qspi mode failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* set chip interface spi */
            res = w25qxx_set_interface(&gs_handle, W25QXX_INTERFACE_SPI);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: set interface failed.\n");
               
                return 1;
            }
            
            /* random data */
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            
            /* w25qxx_sector_erase_4k */
            res = w25qxx_sector_erase_4k(&gs_handle, 0x00000000);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: sector erase 4k failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_page_program */
            res = w25qxx_page_program(&gs_handle, 0x00000000, gs_buffer_input, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: page program failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* fast read dual output */
            res = w25qxx_fast_read_dual_output(&gs_handle, 0x00000000, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read dual output failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: fast_read_dual_output check passed.\n");
            
            /* fast read quad output */
            res = w25qxx_fast_read_quad_output(&gs_handle, 0x00000000, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read quad output failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: fast_read_quad_output check passed.\n");
            
            /* fast read dual io */
            res = w25qxx_fast_read_dual_io(&gs_handle, 0x00000000, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read dual io failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: w25qxx_fast_read_dual_io check passed.\n");
            
            /* fast read quad io */
            res = w25qxx_fast_read_quad_io(&gs_handle, 0x00000000, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read quad io failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: w25qxx_fast_read_quad_io check passed.\n");
            
            /* word read quad io */
            res = w25qxx_word_read_quad_io(&gs_handle, 0x00000000, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: word read quad io failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: w25qxx_word_read_quad_io check passed.\n");
            
            /* octal read quad io */
            res = w25qxx_octal_word_read_quad_io(&gs_handle, 0x00000000, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: octal read quad io failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: w25qxx_octal_word_read_quad_io check passed.\n");
            
            /* random data */
            for (j = 0; j < 256; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            
            /* w25qxx_sector_erase_4k */
            res = w25qxx_sector_erase_4k(&gs_handle, 0x00000000);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: sector erase 4k failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* w25qxx_page_program_quad_input */
            res = w25qxx_page_program_quad_input(&gs_handle, 0x00000000, gs_buffer_input, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: page program quad input failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* fast read */
            res = w25qxx_fast_read(&gs_handle, 0x00000000, gs_buffer_output, 256);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 256; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: w25qxx_page_program_quad_input check passed.\n");
            
            if (type >= W25Q256)
            {
                /* set address mode 4 byte */
                w25qxx_interface_debug_print("w25qxx: set address mode 4 byte.\n");
                
                /* set address mode 4 byte */
                res = w25qxx_set_address_mode(&gs_handle, W25QXX_ADDRESS_MODE_4_BYTE);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: set address mode failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                
                /* random data */
                for (j = 0; j < 256; j++)
                {
                    gs_buffer_input[j] = rand() %256;
                }
                
                /* w25qxx_sector_erase_4k */
                res = w25qxx_sector_erase_4k(&gs_handle, 0x00000000);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: sector erase 4k failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                
                /* w25qxx_page_program */
                res = w25qxx_page_program(&gs_handle, 0x00000000, gs_buffer_input, 256);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: page program failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                
                /* fast read dual output */
                res = w25qxx_fast_read_dual_output(&gs_handle, 0x00000000, gs_buffer_output, 256);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: fast read dual output failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                for (j = 0; j < 256; j++)
                {
                    if (gs_buffer_input[j] != gs_buffer_output[j])
                    {
                        w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                        (void)w25qxx_deinit(&gs_handle);
                        
                        return 1;
                    }
                }
                w25qxx_interface_debug_print("w25qxx: fast_read_dual_output check passed.\n");
                
                /* fast read quad output */
                res = w25qxx_fast_read_quad_output(&gs_handle, 0x00000000, gs_buffer_output, 256);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: fast read quad output failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                for (j = 0; j < 256; j++)
                {
                    if (gs_buffer_input[j] != gs_buffer_output[j])
                    {
                        w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                        (void)w25qxx_deinit(&gs_handle);
                        
                        return 1;
                    }
                }
                w25qxx_interface_debug_print("w25qxx: fast_read_quad_output check passed.\n");
                
                /* fast read dual io */
                res = w25qxx_fast_read_dual_io(&gs_handle, 0x00000000, gs_buffer_output, 256);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: fast read dual io failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                for (j = 0; j < 256; j++)
                {
                    if (gs_buffer_input[j] != gs_buffer_output[j])
                    {
                        w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                        (void)w25qxx_deinit(&gs_handle);
                        
                        return 1;
                    }
                }
                w25qxx_interface_debug_print("w25qxx: w25qxx_fast_read_dual_io check passed.\n");
                
                /* fast read quad io */
                res = w25qxx_fast_read_quad_io(&gs_handle, 0x00000000, gs_buffer_output, 256);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: fast read quad io failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                for (j = 0; j < 256; j++)
                {
                    if (gs_buffer_input[j] != gs_buffer_output[j])
                    {
                        w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                        (void)w25qxx_deinit(&gs_handle);
                        
                        return 1;
                    }
                }
                w25qxx_interface_debug_print("w25qxx: w25qxx_fast_read_quad_io check passed.\n");
                
                /* word read quad io */
                res = w25qxx_word_read_quad_io(&gs_handle, 0x00000000, gs_buffer_output, 256);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: word read quad io failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                for (j = 0; j < 256; j++)
                {
                    if (gs_buffer_input[j] != gs_buffer_output[j])
                    {
                        w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                        (void)w25qxx_deinit(&gs_handle);
                        
                        return 1;
                    }
                }
                w25qxx_interface_debug_print("w25qxx: w25qxx_word_read_quad_io check passed.\n");
                
                /* octal read quad io */
                res = w25qxx_octal_word_read_quad_io(&gs_handle, 0x00000000, gs_buffer_output, 256);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: octal read quad io failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                for (j = 0; j < 256; j++)
                {
                    if (gs_buffer_input[j] != gs_buffer_output[j])
                    {
                        w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                        (void)w25qxx_deinit(&gs_handle);
                        
                        return 1;
                    }
                }
                w25qxx_interface_debug_print("w25qxx: w25qxx_octal_word_read_quad_io check passed.\n");
                
                /* random data */
                for (j = 0; j < 256; j++)
                {
                    gs_buffer_input[j] = rand() %256;
                }
                
                /* w25qxx_sector_erase_4k */
                res = w25qxx_sector_erase_4k(&gs_handle, 0x00000000);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: sector erase 4k failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                
                /* w25qxx_page_program_quad_input */
                res = w25qxx_page_program_quad_input(&gs_handle, 0x00000000, gs_buffer_input, 256);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: page program quad input failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                
                /* fast read */
                res = w25qxx_fast_read(&gs_handle, 0x00000000, gs_buffer_output, 256);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: fast read failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                for (j = 0; j < 256; j++)
                {
                    if (gs_buffer_input[j] != gs_buffer_output[j])
                    {
                        w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                        (void)w25qxx_deinit(&gs_handle);
                        
                        return 1;
                    }
                }
                w25qxx_interface_debug_print("w25qxx: w25qxx_page_program_quad_input check passed.\n");
                
                /* set address mode 3 byte */
                w25qxx_interface_debug_print("w25qxx: set address mode 3 byte.\n");
                
                /* set address mode 3 byte */
                res = w25qxx_set_address_mode(&gs_handle, W25QXX_ADDRESS_MODE_3_BYTE);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: set address mode failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
            }
            
            /* enter to qspi mode */
            w25qxx_interface_debug_print("w25qxx: enter to qspi mode.\n");
            
            /* enter qspi */
            res = w25qxx_enter_qspi_mode(&gs_handle);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: enter qspi mode failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            
            /* set chip interface spi */
            res = w25qxx_set_interface(&gs_handle, W25QXX_INTERFACE_QSPI);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: set interface failed.\n");
               
                return 1;
            }
        }
        
        /* w25qxx_set_read_parameters test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_set_read_parameters test.\n");
        
        /* 8 dummy max 80MHz test */
        w25qxx_interface_debug_print("w25qxx: set 8 dummy max 80MHz test.\n");
        
        res = w25qxx_set_read_parameters(&gs_handle, W25QXX_QSPI_READ_DUMMY_8_80MHZ, W25QXX_QSPI_READ_WRAP_LENGTH_8_BYTE);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: set read parameters.\n");
           
            return 1;
        }
        
        for (addr = 0; addr < size; addr += step)
        {
            for (j = 0; j < 600; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            res = w25qxx_write(&gs_handle, addr, gs_buffer_input, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: write failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            res = w25qxx_read(&gs_handle, addr, gs_buffer_output, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 600; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: 0x%08X/0x%08X successful.\n", addr, size);
        }
        
        /* 6 dummy max 80MHz test */
        w25qxx_interface_debug_print("w25qxx: set 6 dummy max 80MHz test.\n");
        
        res = w25qxx_set_read_parameters(&gs_handle, W25QXX_QSPI_READ_DUMMY_6_80MHZ, W25QXX_QSPI_READ_WRAP_LENGTH_8_BYTE);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: set read parameters.\n");
           
            return 1;
        }
        
        for (addr = 0; addr < size; addr += step)
        {
            for (j = 0; j < 600; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            res = w25qxx_write(&gs_handle, addr, gs_buffer_input, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: write failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            res = w25qxx_read(&gs_handle, addr, gs_buffer_output, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 600; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: 0x%08X/0x%08X successful.\n", addr, size);
        }
        
        /* 4 dummy max 55MHz test */
        w25qxx_interface_debug_print("w25qxx: set 4 dummy max 55MHz test.\n");
        
        res = w25qxx_set_read_parameters(&gs_handle, W25QXX_QSPI_READ_DUMMY_4_55MHZ, W25QXX_QSPI_READ_WRAP_LENGTH_8_BYTE);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: set read parameters.\n");
           
            return 1;
        }
        
        for (addr = 0; addr < size; addr += step)
        {
            for (j = 0; j < 600; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            res = w25qxx_write(&gs_handle, addr, gs_buffer_input, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: write failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            res = w25qxx_read(&gs_handle, addr, gs_buffer_output, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 600; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: 0x%08X/0x%08X successful.\n", addr, size);
        }
        
        /* 2 dummy max 33MHz test */
        w25qxx_interface_debug_print("w25qxx: set 2 dummy max 33MHz test.\n");
        
        res = w25qxx_set_read_parameters(&gs_handle, W25QXX_QSPI_READ_DUMMY_2_33MHZ, W25QXX_QSPI_READ_WRAP_LENGTH_8_BYTE);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: set read parameters.\n");
           
            return 1;
        }
        
        for (addr = 0; addr < size; addr += step)
        {
            for (j = 0; j < 600; j++)
            {
                gs_buffer_input[j] = rand() %256;
            }
            res = w25qxx_write(&gs_handle, addr, gs_buffer_input, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: write failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            res = w25qxx_read(&gs_handle, addr, gs_buffer_output, 600);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            for (j = 0; j < 600; j++)
            {
                if (gs_buffer_input[j] != gs_buffer_output[j])
                {
                    w25qxx_interface_debug_print("w25qxx: write read check failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                    
                    return 1;
                }
            }
            w25qxx_interface_debug_print("w25qxx: 0x%08X/0x%08X successful.\n", addr, size);
        }
    }
    
    /* finish read test */
    w25qxx_interface_debug_print("w25qxx: finish read test.\n");
    (void)w25qxx_deinit(&gs_handle);
    
    return 0;
}
