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
 * @file      driver_w25qxx_advance.c
 * @brief     driver w25qxx advance source file
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

#include "driver_w25qxx_advance.h"

static w25qxx_handle_t gs_handle;        /**< w25qxx handle */

/**
 * @brief     advance example init
 * @param[in] type is the chip type
 * @param[in] interface is the chip interface
 * @param[in] dual_quad_spi_enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t w25qxx_advance_init(w25qxx_type_t type, w25qxx_interface_t interface, w25qxx_bool_t dual_quad_spi_enable)
{
    uint8_t res;
    
    /* link interface function */
    DRIVER_W25QXX_LINK_INIT(&gs_handle, w25qxx_handle_t);
    DRIVER_W25QXX_LINK_SPI_QSPI_INIT(&gs_handle, w25qxx_interface_spi_qspi_init);
    DRIVER_W25QXX_LINK_SPI_QSPI_DEINIT(&gs_handle, w25qxx_interface_spi_qspi_deinit);
    DRIVER_W25QXX_LINK_SPI_QSPI_WRITE_READ(&gs_handle, w25qxx_interface_spi_qspi_write_read);
    DRIVER_W25QXX_LINK_DELAY_MS(&gs_handle, w25qxx_interface_delay_ms);
    DRIVER_W25QXX_LINK_DELAY_US(&gs_handle, w25qxx_interface_delay_us);
    DRIVER_W25QXX_LINK_DEBUG_PRINT(&gs_handle, w25qxx_interface_debug_print);
    
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
    else
    {
        if (type >= W25Q256)
        {
            res = w25qxx_set_address_mode(&gs_handle, W25QXX_ADDRESS_MODE_4_BYTE);
            if (res != 0)
            {
                w25qxx_interface_debug_print("w25qxx: set address mode failed.\n");
                (void)w25qxx_deinit(&gs_handle);
               
                return 1;
            }
        }
        
        return 0;
    }
}

/**
 * @brief  advance example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t w25qxx_advance_deinit(void)
{
    if (w25qxx_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example write
 * @param[in] addr is the write address
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t w25qxx_advance_write(uint32_t addr, uint8_t *data, uint32_t len)
{
    if (w25qxx_write(&gs_handle, addr, data, len) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example read
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t w25qxx_advance_read(uint32_t addr, uint8_t *data, uint32_t len)
{
    if (w25qxx_read(&gs_handle, addr, data, len) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  advance example power down
 * @return status code
 *         - 0 success
 *         - 1 power down failed
 * @note   none
 */
uint8_t w25qxx_advance_power_down(void)
{
    if (w25qxx_power_down(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  advance example wake up
 * @return status code
 *         - 0 success
 *         - 1 wake up failed
 * @note   none
 */
uint8_t w25qxx_advance_wake_up(void)
{
    if (w25qxx_release_power_down(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  advance example chip erase
 * @return status code
 *         - 0 success
 *         - 1 chip erase failed
 * @note   none
 */
uint8_t w25qxx_advance_chip_erase(void)
{
    if (w25qxx_chip_erase(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example get the manufacturer && device id information
 * @param[out] *manufacturer points to a manufacturer buffer
 * @param[out] *device_id points to a device id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get manufacturer device id failed
 * @note       none
 */
uint8_t w25qxx_advance_get_id(uint8_t *manufacturer, uint8_t *device_id)
{
    if (w25qxx_get_manufacturer_device_id(&gs_handle, manufacturer, device_id) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example page program
 * @param[in] addr is the programming address
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 page program failed
 * @note      len <= 256
 */
uint8_t w25qxx_advance_page_program(uint32_t addr, uint8_t *data, uint16_t len)
{
    if (w25qxx_page_program(&gs_handle, addr, data, len) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example erase the 4k sector
 * @param[in] addr is the erase address
 * @return    status code
 *            - 0 success
 *            - 1 sector erase 4k failed
 * @note      none
 */
uint8_t w25qxx_advance_sector_erase_4k(uint32_t addr)
{
    if (w25qxx_sector_erase_4k(&gs_handle, addr) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example erase the 32k block
 * @param[in] addr is the erase address
 * @return    status code
 *            - 0 success
 *            - 1 block erase 32k failed
 * @note      none
 */
uint8_t w25qxx_advance_block_erase_32k(uint32_t addr)
{
    if (w25qxx_block_erase_32k(&gs_handle, addr) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example erase the 64k block
 * @param[in] addr is the erase address
 * @return    status code
 *            - 0 success
 *            - 1 block erase 64k failed
 * @note      none
 */
uint8_t w25qxx_advance_block_erase_64k(uint32_t addr)
{
    if (w25qxx_block_erase_64k(&gs_handle, addr) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example read in the fast mode
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 fast read failed
 * @note       none
 */
uint8_t w25qxx_advance_fast_read(uint32_t addr, uint8_t *data, uint32_t len)
{
    if (w25qxx_fast_read(&gs_handle, addr, data, len) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example read only in the spi interface
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 only spi read failed
 * @note       none
 */
uint8_t w25qxx_advance_only_spi_read(uint32_t addr, uint8_t *data, uint32_t len)
{
    if (w25qxx_only_spi_read(&gs_handle, addr, data, len) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example fast read only in the spi dual output interface
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t w25qxx_advance_only_spi_fast_read_dual_output(uint32_t addr, uint8_t *data, uint32_t len)
{
    if (w25qxx_fast_read_dual_output(&gs_handle, addr, data, len) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example fast read only in the spi quad output interface
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t w25qxx_advance_only_spi_fast_read_quad_output(uint32_t addr, uint8_t *data, uint32_t len)
{
    if (w25qxx_fast_read_quad_output(&gs_handle, addr, data, len) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example fast read only in the spi dual io interface
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t w25qxx_advance_only_spi_fast_read_dual_io(uint32_t addr, uint8_t *data, uint32_t len)
{
    if (w25qxx_fast_read_dual_io(&gs_handle, addr, data, len) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example fast read only in the spi quad io interface
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t w25qxx_advance_only_spi_fast_read_quad_io(uint32_t addr, uint8_t *data, uint32_t len)
{
    if (w25qxx_fast_read_quad_io(&gs_handle, addr, data, len) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example word read only in the spi quad io interface
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t w25qxx_advance_only_spi_word_read_quad_io(uint32_t addr, uint8_t *data, uint32_t len)
{
    if (w25qxx_word_read_quad_io(&gs_handle, addr, data, len) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
 
/**
 * @brief      advance example octal word read only in the spi quad io interface
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t w25qxx_advance_only_spi_octal_word_read_quad_io(uint32_t addr, uint8_t *data, uint32_t len)
{
    if (w25qxx_octal_word_read_quad_io(&gs_handle, addr, data, len) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example quad page program only in the spi quad input interface
 * @param[in] addr is the programming address
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 quad page program failed
 * @note      len <= 256
 */
uint8_t w25qxx_advance_only_spi_page_program_quad_input(uint32_t addr, uint8_t *data, uint16_t len)
{
    if (w25qxx_page_program_quad_input(&gs_handle, addr, data, len) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example get the status 1
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status 1 failed
 * @note       none
 */
uint8_t w25qxx_advance_get_status1(uint8_t *status)
{
    if (w25qxx_get_status1(&gs_handle, status) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example get the status 2
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status 2 failed
 * @note       none
 */
uint8_t w25qxx_advance_get_status2(uint8_t *status)
{
    if (w25qxx_get_status2(&gs_handle, status) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example get the status 3
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status 3 failed
 * @note       none
 */
uint8_t w25qxx_advance_get_status3(uint8_t *status)
{
    if (w25qxx_get_status3(&gs_handle, status) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example set the status 1
 * @param[in] status is the set status
 * @return    status code
 *            - 0 success
 *            - 1 set status 1 failed
 * @note      none
 */
uint8_t w25qxx_advance_set_status1(uint8_t status)
{
    if (w25qxx_set_status1(&gs_handle, status) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example set the status 2
 * @param[in] status is the set status
 * @return    status code
 *            - 0 success
 *            - 1 set status 2 failed
 * @note      none
 */
uint8_t w25qxx_advance_set_status2(uint8_t status)
{
    if (w25qxx_set_status2(&gs_handle, status) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example set the status 3
 * @param[in] status is the set status
 * @return    status code
 *            - 0 success
 *            - 1 set status 3 failed
 * @note      none
 */
uint8_t w25qxx_advance_set_status3(uint8_t status)
{
    if (w25qxx_set_status3(&gs_handle, status) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example get the manufacturer && device id information only in the spi dual io interface
 * @param[out] *manufacturer points to a manufacturer buffer
 * @param[out] *device_id points to a device id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get manufacturer device id dual io failed
 * @note       none
 */
uint8_t w25qxx_advance_only_spi_get_manufacturer_device_id_dual_io(uint8_t *manufacturer, uint8_t *device_id)
{
    if (w25qxx_get_manufacturer_device_id_dual_io(&gs_handle, manufacturer, device_id) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example get the manufacturer && device id information only in the spi quad io interface
 * @param[out] *manufacturer points to a manufacturer buffer
 * @param[out] *device_id points to a device id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get manufacturer device id quad io failed
 * @note       none
 */
uint8_t w25qxx_advance_only_spi_get_manufacturer_device_id_quad_io(uint8_t *manufacturer, uint8_t *device_id)
{
    if (w25qxx_get_manufacturer_device_id_quad_io(&gs_handle, manufacturer, device_id) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example get the jedec id information
 * @param[out] *manufacturer points to a manufacturer buffer
 * @param[out] *device_id points to a device id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get jedec id failed
 * @note       none
 */
uint8_t w25qxx_advance_get_get_jedec_id(uint8_t *manufacturer, uint8_t device_id[2])
{
    if (w25qxx_get_jedec_id(&gs_handle, manufacturer, device_id) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  advance example global block lock
 * @return status code
 *         - 0 success
 *         - 1 global block lock failed
 * @note   none
 */
uint8_t w25qxx_advance_global_block_lock(void)
{
    if (w25qxx_global_block_lock(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  advance example global block unlock
 * @return status code
 *         - 0 success
 *         - 1 global block unlock failed
 * @note   none
 */
uint8_t w25qxx_advance_global_block_unlock(void)
{
    if (w25qxx_global_block_unlock(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example lock the individual block
 * @param[in] addr is the block address
 * @return    status code
 *            - 0 success
 *            - 1 individual block lock failed
 * @note      none
 */
uint8_t w25qxx_advance_individual_block_lock(uint32_t addr)
{
    if (w25qxx_individual_block_lock(&gs_handle, addr) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example unlock the individual block
 * @param[in] addr is the block address
 * @return    status code
 *            - 0 success
 *            - 1 individual block unlock failed
 * @note      none
 */
uint8_t w25qxx_advance_individual_block_unlock(uint32_t addr)
{
    if (w25qxx_individual_block_unlock(&gs_handle, addr) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example read the block lock
 * @param[in]  addr is the block address
 * @param[out] *value points to a value buffer
 * @return     status code
 *             - 0 success
 *             - 1 read block lock failed
 * @note       none
 */
uint8_t w25qxx_advance_read_block_lock(uint32_t addr, uint8_t *value)
{
    if (w25qxx_read_block_lock(&gs_handle, addr, value) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief   advance example reset
 * @return  status code
 *          - 0 success
 *          - 1 reset failed
 * @note    none
 */
uint8_t w25qxx_advance_reset(void)
{
    if (w25qxx_enable_reset(&gs_handle) != 0)
    {
        return 1;
    }
    if (w25qxx_reset_device(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example set the read parameters only in the qspi interface
 * @param[in] dummy is the qspi read dummy
 * @param[in] length is the qspi read wrap length
 * @return    status code
 *            - 0 success
 *            - 1 set read parameters failed
 * @note      none
 */
uint8_t w25qxx_advance_only_qspi_set_read_parameters(w25qxx_qspi_read_dummy_t dummy, w25qxx_qspi_read_wrap_length_t length)
{
    if (w25qxx_set_read_parameters(&gs_handle, dummy, length) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example get the unique id only in the spi interface
 * @param[out] *id points to a id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get the unique id failed
 * @note       none
 */
uint8_t w25qxx_advance_only_spi_get_unique_id(uint8_t id[8])
{
    if (w25qxx_get_unique_id(&gs_handle, id) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example get the sfdp only in the spi interface
 * @param[out] *sfdp points to a sfdp buffer
 * @return     status code
 *             - 0 success
 *             - 1 get the sfdp failed
 * @note       none
 */
uint8_t w25qxx_advance_only_spi_get_sfdp(uint8_t sfdp[256])
{
    if (w25qxx_get_sfdp(&gs_handle, sfdp) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example write the security register only in the spi interface
 * @param[in] num is the security register number
 * @param[in] *data points to a data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write security register failed
 * @note      none
 */
uint8_t w25qxx_advance_only_spi_write_security_register(w25qxx_security_register_t num, uint8_t data[256])
{
    if (w25qxx_erase_security_register(&gs_handle, num) != 0)
    {
        return 1;
    }
    if (w25qxx_program_security_register(&gs_handle, num, data) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      advance example read the security register only in the spi interface
 * @param[in]  num is the security register number
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read security register failed
 * @note       none
 */
uint8_t w25qxx_advance_only_spi_read_security_register(w25qxx_security_register_t num, uint8_t data[256])
{
    if (w25qxx_read_security_register(&gs_handle, num, data) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example set the burst with wrap only in the spi interface
 * @param[in] wrap is the burst wrap
 * @return    status code
 *            - 0 success
 *            - 1 set burst with wrap failed
 * @note      none
 */
uint8_t w25qxx_advance_only_spi_set_burst_with_wrap(w25qxx_burst_wrap_t wrap)
{
    if (w25qxx_set_burst_with_wrap(&gs_handle, wrap) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
