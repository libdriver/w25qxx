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
 * @file      driver_w25qxx_register_test.c
 * @brief     driver w25qxx register test source file
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

#include "driver_w25qxx_register_test.h"

static w25qxx_handle_t gs_handle;        /**< w25qxx handle */

/**
 * @brief     register test
 * @param[in] type is the chip type
 * @param[in] interface is the chip interface
 * @param[in] dual_quad_spi_enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t w25qxx_register_test(w25qxx_type_t type, w25qxx_interface_t interface, w25qxx_bool_t dual_quad_spi_enable)
{
    uint8_t res;
    uint8_t status;
    uint8_t manufacturer;
    uint8_t device_id[2];
    uint8_t id[8];
    w25qxx_info_t info;
    w25qxx_type_t type_check;
    w25qxx_interface_t interface_check;
    
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
    
    /* start read test */
    w25qxx_interface_debug_print("w25qxx: start register test.\n");
    
    /* w25qxx_set_type/w25qxx_get_type test */
    w25qxx_interface_debug_print("w25qxx: w25qxx_set_type/w25qxx_get_type test.\n");
    
    /* set type w25q80 */
    res = w25qxx_set_type(&gs_handle, W25Q80);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set type failed.\n");
       
        return 1;
    }
    w25qxx_interface_debug_print("w25qxx: set type W25Q80.\n");
    
    /* get type */
    res = w25qxx_get_type(&gs_handle, &type_check);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set type failed.\n");
       
        return 1;
    }
    w25qxx_interface_debug_print("w25qxx: check chip type %s.\n", type_check == W25Q80 ? "ok" : "error");
    
    /* set type w25q16 */
    res = w25qxx_set_type(&gs_handle, W25Q16);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set type failed.\n");
       
        return 1;
    }
    w25qxx_interface_debug_print("w25qxx: set type W25Q16.\n");
    
    /* get type */
    res = w25qxx_get_type(&gs_handle, &type_check);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set type failed.\n");
       
        return 1;
    }
    w25qxx_interface_debug_print("w25qxx: check chip type %s.\n", type_check == W25Q16 ? "ok" : "error");
    
    /* set type w25q32 */
    res = w25qxx_set_type(&gs_handle, W25Q32);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set type failed.\n");
       
        return 1;
    }
    w25qxx_interface_debug_print("w25qxx: set type W25Q32.\n");
    
    /* get type */
    res = w25qxx_get_type(&gs_handle, &type_check);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set type failed.\n");
       
        return 1;
    }
    w25qxx_interface_debug_print("w25qxx: check chip type %s.\n", type_check == W25Q32 ? "ok" : "error");
    
    /* set type w25q64 */
    res = w25qxx_set_type(&gs_handle, W25Q64);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set type failed.\n");
       
        return 1;
    }
    w25qxx_interface_debug_print("w25qxx: set type W25Q64.\n");
    
    /* get type */
    res = w25qxx_get_type(&gs_handle, &type_check);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set type failed.\n");
       
        return 1;
    }
    w25qxx_interface_debug_print("w25qxx: check chip type %s.\n", type_check == W25Q64 ? "ok" : "error");
    
    /* set type w25q128 */
    res = w25qxx_set_type(&gs_handle, W25Q128);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set type failed.\n");
       
        return 1;
    }
    w25qxx_interface_debug_print("w25qxx: set type W25Q128.\n");
    
    /* get type */
    res = w25qxx_get_type(&gs_handle, &type_check);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set type failed.\n");
       
        return 1;
    }
    w25qxx_interface_debug_print("w25qxx: check chip type %s.\n", type_check == W25Q128 ? "ok" : "error");

    /* set type w25q256 */
    res = w25qxx_set_type(&gs_handle, W25Q256);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set type failed.\n");
       
        return 1;
    }
    w25qxx_interface_debug_print("w25qxx: set type W25Q256.\n");
    
    /* get type */
    res = w25qxx_get_type(&gs_handle, &type_check);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set type failed.\n");
       
        return 1;
    }
    w25qxx_interface_debug_print("w25qxx: check chip type %s.\n", type_check == W25Q256 ? "ok" : "error");
    
    /* w25qxx_set_interface/w25qxx_get_interface test */
    w25qxx_interface_debug_print("w25qxx: w25qxx_set_interface/w25qxx_get_interface test.\n");
    
    /* set chip interface spi */
    res = w25qxx_set_interface(&gs_handle, W25QXX_INTERFACE_SPI);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set interface failed.\n");
       
        return 1;
    }
    w25qxx_interface_debug_print("w25qxx: set interface SPI.\n");
    
    /* get interface */
    res = w25qxx_get_interface(&gs_handle, &interface_check);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: get interface failed.\n");
       
        return 1;
    }
    w25qxx_interface_debug_print("w25qxx: check chip interface %s.\n", interface_check == W25QXX_INTERFACE_SPI ? "ok" : "error");
    
    /* set chip interface qspi */
    res = w25qxx_set_interface(&gs_handle, W25QXX_INTERFACE_QSPI);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: set interface failed.\n");
       
        return 1;
    }
    w25qxx_interface_debug_print("w25qxx: set interface QSPI.\n");
    
    /* get interface */
    res = w25qxx_get_interface(&gs_handle, &interface_check);
    if (res != 0)
    {
        w25qxx_interface_debug_print("w25qxx: get interface failed.\n");
       
        return 1;
    }
    w25qxx_interface_debug_print("w25qxx: check chip interface %s.\n", interface_check == W25QXX_INTERFACE_QSPI ? "ok" : "error");
    
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
    
    if (interface == W25QXX_INTERFACE_SPI)
    {
        /* w25qxx_get_manufacturer_device_id test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_get_manufacturer_device_id test.\n");
        
        /* get manufacturer device id */
        res = w25qxx_get_manufacturer_device_id(&gs_handle, (uint8_t *)&manufacturer, (uint8_t *)device_id);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: get manufacturer device id failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X, device id is 0x%02X.\n", manufacturer, device_id[0]);
        
        /* w25qxx_get_jedec_id test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_get_jedec_id test.\n");
        
        /* get jedec id */
        res = w25qxx_get_jedec_id(&gs_handle, (uint8_t *)&manufacturer, (uint8_t *)device_id);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: get jedec id failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X, device id is 0x%02X 0x%02X.\n", manufacturer, 
                                     device_id[0], device_id[1]);
        
        /* w25qxx_get_unique_id test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_get_unique_id test.\n");
        
        /* get unique id */
        res = w25qxx_get_unique_id(&gs_handle, (uint8_t *)id);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: get unique id failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: unique id 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\n", 
                                     id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7]);
        
        /* w25qxx_set_status1/w25qxx_get_status1 test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_set_status1/w25qxx_get_status1 test.\n");
        
        /* set status1 */
        res = w25qxx_set_status1(&gs_handle, 0x00);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: set status1 failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* get status1 */
        res = w25qxx_get_status1(&gs_handle, (uint8_t *)&status);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: get status1 failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: status1 is 0x%02X.\n", status);
        
        /* w25qxx_set_status2/w25qxx_get_status2 test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_set_status2/w25qxx_get_status2 test.\n");
        
        /* set status2 */
        res = w25qxx_set_status2(&gs_handle, 0x02);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: set status2 failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* get status2 */
        res = w25qxx_get_status2(&gs_handle, (uint8_t *)&status);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: get status2 failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: status2 is 0x%02X.\n", status);
        
        /* w25qxx_set_status3/w25qxx_get_status3 test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_set_status3/w25qxx_get_status3 test.\n");
        
        /* set status3 */
        res = w25qxx_set_status3(&gs_handle, 0x60);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: set status3 failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* get status3 */
        res = w25qxx_get_status3(&gs_handle, (uint8_t *)&status);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: get status3 failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: status3 is 0x%02X.\n", status);
        
        /* w25qxx_enable_write test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_enable_write test.\n");
        
        /* enable write */
        res = w25qxx_enable_write(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: enable write failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check enable write %s.\n", res == 0 ? "ok" : "error");
        
        /* w25qxx_disable_write test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_disable_write test.\n");
        
        /* disable write */
        res = w25qxx_disable_write(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: disable write failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check disable write %s.\n", res == 0 ? "ok" : "error");
        
        /* w25qxx_enable_write test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_enable_write test.\n");
        
        /* enable sr write */
        res = w25qxx_enable_volatile_sr_write(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: enable sr write failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check enable sr write %s.\n", res == 0 ? "ok" : "error");
        
        /* w25qxx_erase_program_suspend test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_erase_program_suspend test.\n");
        
        /* erase program suspend */
        res = w25qxx_erase_program_suspend(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: erase program suspend failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check erase program suspend %s.\n", res == 0 ? "ok" : "error");
        
        /* w25qxx_erase_program_suspend test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_erase_program_suspend test.\n");
        
        /* erase program resume */
        res = w25qxx_erase_program_resume(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: erase program resume failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check erase program resume %s.\n", res == 0 ? "ok" : "error");
        
        /* w25qxx_global_block_lock test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_global_block_lock test.\n");
        
        /* global block lock */
        res = w25qxx_global_block_lock(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: global block lock failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check global block lock %s.\n", res == 0 ? "ok" : "error");
        
        /* w25qxx_global_block_unlock test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_global_block_unlock test.\n");
        
        /* global block unlock */
        res = w25qxx_global_block_unlock(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: global block unlock failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check global block unlock %s.\n", res == 0 ? "ok" : "error");
        
        /* w25qxx_individual_block_lock test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_individual_block_lock test.\n");
        
        /* individual block lock */
        res = w25qxx_individual_block_lock(&gs_handle, 0x00000000);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: individual block lock failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check individual block lock %s.\n", res == 0 ? "ok" : "error");
        
        /* w25qxx_read_block_lock test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_read_block_lock test.\n");
        
        res = w25qxx_read_block_lock(&gs_handle, 0x00000000, (uint8_t *)&status);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: read block lock failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check read block lock %s with %d.\n", res == 0 ? "ok" : "error", status);
        
        /* w25qxx_individual_block_unlock test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_individual_block_unlock test.\n");
        
        /* individual block unlock */
        res = w25qxx_individual_block_unlock(&gs_handle, 0x00000000);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: individual block unlock failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check individual block unlock %s.\n", res == 0 ? "ok" : "error");

        /* w25qxx_set_burst_with_wrap test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_set_burst_with_wrap test.\n");
        
        /* set burst with wrap */
        res = w25qxx_set_burst_with_wrap(&gs_handle, W25QXX_BURST_WRAP_NONE);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: set burst with wrap failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check set burst with wrap %s.\n", res == 0 ? "ok" : "error");
        
        /* w25qxx_power_down test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_power_down test.\n");
        
        /* power down */
        res = w25qxx_power_down(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: power down failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_delay_ms(10);
        
        /* w25qxx_release_power_down test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_release_power_down test.\n");
        
        /* release power down */
        res = w25qxx_release_power_down(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: release power down failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_delay_ms(10);
        
        /* w25qxx_enable_reset test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_enable_reset test.\n");
        
        /* enable reset */
        res = w25qxx_enable_reset(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: enable reset failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* w25qxx_reset_device test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_reset_device test.\n");
        
        /* reset device */
        res = w25qxx_reset_device(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: reset device failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_delay_ms(10);
        
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
            
            /* w25qxx_get_unique_id test */
            w25qxx_interface_debug_print("w25qxx: w25qxx_get_unique_id test.\n");
            
            /* get unique id */
            res = w25qxx_get_unique_id(&gs_handle, (uint8_t *)id);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: get unique id failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            w25qxx_interface_debug_print("w25qxx: unique id 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\n", 
                                         id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7]);
                                         
            /* w25qxx_individual_block_lock test */
            w25qxx_interface_debug_print("w25qxx: w25qxx_individual_block_lock test.\n");
            
            /* individual block lock */
            res = w25qxx_individual_block_lock(&gs_handle, 0x00000000);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: individual block lock failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            w25qxx_interface_debug_print("w25qxx: check individual block lock %s.\n", res == 0 ? "ok" : "error");
            
            /* w25qxx_read_block_lock test */
            w25qxx_interface_debug_print("w25qxx: w25qxx_read_block_lock test.\n");
            
            res = w25qxx_read_block_lock(&gs_handle, 0x00000000, (uint8_t *)&status);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read block lock failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            w25qxx_interface_debug_print("w25qxx: check read block lock %s with %d.\n", res == 0 ? "ok" : "error", status);
            
            /* w25qxx_individual_block_unlock test */
            w25qxx_interface_debug_print("w25qxx: w25qxx_individual_block_unlock test.\n");
            
            /* individual block unlock */
            res = w25qxx_individual_block_unlock(&gs_handle, 0x00000000);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: individual block unlock failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            w25qxx_interface_debug_print("w25qxx: check individual block unlock %s.\n", res == 0 ? "ok" : "error");
            
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
        /* w25qxx_get_manufacturer_device_id test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_get_manufacturer_device_id test.\n");
        
        /* get manufacturer device id */
        res = w25qxx_get_manufacturer_device_id(&gs_handle, (uint8_t *)&manufacturer, (uint8_t *)device_id);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: get manufacturer device id failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X, device id is 0x%02X.\n", manufacturer, device_id[0]);
        
        /* w25qxx_get_jedec_id test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_get_jedec_id test.\n");
        
        /* get jedec id */
        res = w25qxx_get_jedec_id(&gs_handle, (uint8_t *)&manufacturer, (uint8_t *)device_id);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: get jedec id failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X, device id is 0x%02X 0x%02X.\n", manufacturer, 
                                     device_id[0], device_id[1]);
        
        /* w25qxx_set_status1/w25qxx_get_status1 test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_set_status1/w25qxx_get_status1 test.\n");
        
        /* set status1 */
        res = w25qxx_set_status1(&gs_handle, 0x00);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: set status1 failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* get status1 */
        res = w25qxx_get_status1(&gs_handle, (uint8_t *)&status);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: get status1 failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: status1 is 0x%02X.\n", status);
        
        /* w25qxx_set_status2/w25qxx_get_status2 test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_set_status2/w25qxx_get_status2 test.\n");
        
        /* set status2 */
        res = w25qxx_set_status2(&gs_handle, 0x02);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: set status2 failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* get status2 */
        res = w25qxx_get_status2(&gs_handle, (uint8_t *)&status);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: get status2 failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: status2 is 0x%02X.\n", status);
        
        /* w25qxx_set_status3/w25qxx_get_status3 test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_set_status3/w25qxx_get_status3 test.\n");
        
        /* set status3 */
        res = w25qxx_set_status3(&gs_handle, 0x60);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: set status3 failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* get status3 */
        res = w25qxx_get_status3(&gs_handle, (uint8_t *)&status);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: get status3 failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: status3 is 0x%02X.\n", status);
        
        /* w25qxx_enable_write test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_enable_write test.\n");
        
        /* enable write */
        res = w25qxx_enable_write(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: enable write failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check enable write %s.\n", res == 0 ? "ok" : "error");
        
        /* w25qxx_disable_write test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_disable_write test.\n");
        
        /* disable write */
        res = w25qxx_disable_write(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: disable write failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check disable write %s.\n", res == 0 ? "ok" : "error");
        
        /* w25qxx_enable_write test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_enable_write test.\n");
        
        /* enable sr write */
        res = w25qxx_enable_volatile_sr_write(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: enable sr write failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check enable sr write %s.\n", res == 0 ? "ok" : "error");
        
        /* w25qxx_erase_program_suspend test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_erase_program_suspend test.\n");
        
        /* erase program suspend */
        res = w25qxx_erase_program_suspend(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: erase program suspend failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check erase program suspend %s.\n", res == 0 ? "ok" : "error");
        
        /* w25qxx_erase_program_suspend test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_erase_program_suspend test.\n");
        
        /* erase program resume */
        res = w25qxx_erase_program_resume(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: erase program resume failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check erase program resume %s.\n", res == 0 ? "ok" : "error");
        
        /* w25qxx_global_block_lock test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_global_block_lock test.\n");
        
        /* global block lock */
        res = w25qxx_global_block_lock(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: global block lock failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check global block lock %s.\n", res == 0 ? "ok" : "error");
        
        /* w25qxx_global_block_unlock test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_global_block_unlock test.\n");
        
        /* global block unlock */
        res = w25qxx_global_block_unlock(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: global block unlock failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check global block unlock %s.\n", res == 0 ? "ok" : "error");
        
        /* w25qxx_individual_block_lock test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_individual_block_lock test.\n");
        
        /* individual block lock */
        res = w25qxx_individual_block_lock(&gs_handle, 0x00000000);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: individual block lock failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check individual block lock %s.\n", res == 0 ? "ok" : "error");
        
        /* w25qxx_read_block_lock test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_read_block_lock test.\n");
        
        res = w25qxx_read_block_lock(&gs_handle, 0x00000000, (uint8_t *)&status);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: read block lock failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check read block lock %s with %d.\n", res == 0 ? "ok" : "error", status);
        
        /* w25qxx_individual_block_unlock test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_individual_block_unlock test.\n");
        
        /* individual block unlock */
        res = w25qxx_individual_block_unlock(&gs_handle, 0x00000000);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: individual block unlock failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_debug_print("w25qxx: check individual block unlock %s.\n", res == 0 ? "ok" : "error");
        
        /* w25qxx_power_down test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_power_down test.\n");
        
        /* power down */
        res = w25qxx_power_down(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: power down failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_delay_ms(10);
        
        /* w25qxx_release_power_down test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_release_power_down test.\n");
        
        /* release power down */
        res = w25qxx_release_power_down(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: release power down failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_delay_ms(10);
        
        /* w25qxx_enable_reset test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_enable_reset test.\n");
        
        /* enable reset */
        res = w25qxx_enable_reset(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: enable reset failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        
        /* w25qxx_reset_device test */
        w25qxx_interface_debug_print("w25qxx: w25qxx_reset_device test.\n");
        
        /* reset device */
        res = w25qxx_reset_device(&gs_handle);
        if (res != 0)
        {
            w25qxx_interface_debug_print("w25qxx: reset device failed.\n");
            (void)w25qxx_deinit(&gs_handle);
           
            return 1;
        }
        w25qxx_interface_delay_ms(10);
        
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

            /* w25qxx_individual_block_lock test */
            w25qxx_interface_debug_print("w25qxx: w25qxx_individual_block_lock test.\n");
            
            /* individual block lock */
            res = w25qxx_individual_block_lock(&gs_handle, 0x00000000);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: individual block lock failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            w25qxx_interface_debug_print("w25qxx: check individual block lock %s.\n", res == 0 ? "ok" : "error");
            
            /* w25qxx_individual_block_unlock test */
            w25qxx_interface_debug_print("w25qxx: w25qxx_individual_block_unlock test.\n");
            
            /* individual block unlock */
            res = w25qxx_individual_block_unlock(&gs_handle, 0x00000000);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: individual block unlock failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            w25qxx_interface_debug_print("w25qxx: check individual block unlock %s.\n", res == 0 ? "ok" : "error");
            
            /* w25qxx_read_block_lock test */
            w25qxx_interface_debug_print("w25qxx: w25qxx_read_block_lock test.\n");
            
            res = w25qxx_read_block_lock(&gs_handle, 0x00000000, (uint8_t *)&status);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: read block lock failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            w25qxx_interface_debug_print("w25qxx: check read block lock %s with %d.\n", res == 0 ? "ok" : "error", status);
            
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
            
            /* w25qxx_get_manufacturer_device_id_dual_io test */
            w25qxx_interface_debug_print("w25qxx: w25qxx_get_manufacturer_device_id_dual_io test.\n");
            
            /* get manufacturer device id dual io */
            res = w25qxx_get_manufacturer_device_id_dual_io(&gs_handle, (uint8_t *)&manufacturer, (uint8_t *)device_id);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: get manufacturer device id dual io failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X, device id is 0x%02X.\n", manufacturer, device_id[0]);
            
            /* w25qxx_get_manufacturer_device_id_quad_io test */
            w25qxx_interface_debug_print("w25qxx: w25qxx_get_manufacturer_device_id_quad_io test.\n");
            
            /* get manufacturer device id quad io */
            res = w25qxx_get_manufacturer_device_id_quad_io(&gs_handle, (uint8_t *)&manufacturer, (uint8_t *)device_id);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: get manufacturer device id quad io failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
            w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X, device id is 0x%02X.\n", manufacturer, device_id[0]);
            
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
                
                /* w25qxx_get_manufacturer_device_id_dual_io test */
                w25qxx_interface_debug_print("w25qxx: w25qxx_get_manufacturer_device_id_dual_io test.\n");
                
                /* get manufacturer device id dual io */
                res = w25qxx_get_manufacturer_device_id_dual_io(&gs_handle, (uint8_t *)&manufacturer, (uint8_t *)device_id);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: get manufacturer device id dual io failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X, device id is 0x%02X.\n", manufacturer, device_id[0]);
                
                /* w25qxx_get_manufacturer_device_id_quad_io test */
                w25qxx_interface_debug_print("w25qxx: w25qxx_get_manufacturer_device_id_quad_io test.\n");
                
                /* get manufacturer device id quad io */
                res = w25qxx_get_manufacturer_device_id_quad_io(&gs_handle, (uint8_t *)&manufacturer, (uint8_t *)device_id);
                if (res != 0)
                {
                    w25qxx_interface_debug_print("w25qxx: get manufacturer device id quad io failed.\n");
                    (void)w25qxx_deinit(&gs_handle);
                   
                    return 1;
                }
                w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X, device id is 0x%02X.\n", manufacturer, device_id[0]);
                
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
    }
    
    /* finish read test */
    w25qxx_interface_debug_print("w25qxx: finish register test.\n");
    (void)w25qxx_deinit(&gs_handle);
    
    return 0;
}
