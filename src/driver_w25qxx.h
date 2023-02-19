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
 * @file      driver_w25qxx.h
 * @brief     driver w25qxx header file
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

#ifndef DRIVER_W25QXX_H
#define DRIVER_W25QXX_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup w25qxx_driver w25qxx driver function
 * @brief    w25qxx driver modules
 * @{
 */

/**
 * @addtogroup w25qxx_basic_driver
 * @{
 */

/**
 * @brief w25qxx type enumeration definition
 */
typedef enum
{
    W25Q80  = 0XEF13U,        /**< w25q80 */
    W25Q16  = 0XEF14U,        /**< w25q16 */
    W25Q32  = 0XEF15U,        /**< w25q32 */
    W25Q64  = 0XEF16U,        /**< w25q64 */
    W25Q128 = 0XEF17U,        /**< w25q128 */
    W25Q256 = 0XEF18U,        /**< w25q256 */
} w25qxx_type_t;

/**
 * @brief w25qxx interface enumeration definition
 */
typedef enum
{
    W25QXX_INTERFACE_SPI  = 0x00,        /**< spi interface */
    W25QXX_INTERFACE_QSPI = 0x01,        /**< dspi qspi interface */
} w25qxx_interface_t;

/**
 * @brief w25qxx bool enumeration definition
 */
typedef enum
{
    W25QXX_BOOL_FALSE = 0x00,        /**< false */
    W25QXX_BOOL_TRUE  = 0x01,        /**< true */
} w25qxx_bool_t;

/**
 * @brief w25qxx address mode enumeration definition
 */
typedef enum
{
    W25QXX_ADDRESS_MODE_3_BYTE = 0x00,        /**< 3 byte mode */
    W25QXX_ADDRESS_MODE_4_BYTE = 0x01,        /**< 4 byte mode */
} w25qxx_address_mode_t;

/**
 * @}
 */

/**
 * @addtogroup w25qxx_advance_driver
 * @{
 */

/**
 * @brief w25qxx qspi read dummy enumeration definition
 */
typedef enum
{
    W25QXX_QSPI_READ_DUMMY_2_33MHZ = 0x00,        /**< qspi read dummy 2 max 33 MHz */
    W25QXX_QSPI_READ_DUMMY_4_55MHZ = 0x01,        /**< qspi read dummy 4 max 55 MHz */
    W25QXX_QSPI_READ_DUMMY_6_80MHZ = 0x02,        /**< qspi read dummy 6 max 80 MHz */
    W25QXX_QSPI_READ_DUMMY_8_80MHZ = 0x03,        /**< qspi read dummy 8 max 80 MHz */
} w25qxx_qspi_read_dummy_t;

/**
 * @brief w25qxx qspi read wrap length enumeration definition
 */
typedef enum
{
    W25QXX_QSPI_READ_WRAP_LENGTH_8_BYTE  = 0x00,        /**< read wrap length 8 byte */
    W25QXX_QSPI_READ_WRAP_LENGTH_16_BYTE = 0x01,        /**< read wrap length 16 byte */
    W25QXX_QSPI_READ_WRAP_LENGTH_32_BYTE = 0x02,        /**< read wrap length 32 byte */
    W25QXX_QSPI_READ_WRAP_LENGTH_64_BYTE = 0x03,        /**< read wrap length 64 byte */
} w25qxx_qspi_read_wrap_length_t;


/**
 * @brief w25qxx security register enumeration definition
 */
typedef enum
{
    W25QXX_SECURITY_REGISTER_1 = 0x1000,        /**< security register 1 */
    W25QXX_SECURITY_REGISTER_2 = 0x2000,        /**< security register 2 */
    W25QXX_SECURITY_REGISTER_3 = 0x3000,        /**< security register 3 */
} w25qxx_security_register_t;

/**
 * @brief w25qxx burst wrap enumeration definition
 */
typedef enum
{
    W25QXX_BURST_WRAP_NONE    = 0x10,        /**< no burst wrap */
    W25QXX_BURST_WRAP_8_BYTE  = 0x00,        /**< 8 byte burst wrap */
    W25QXX_BURST_WRAP_16_BYTE = 0x20,        /**< 16 byte burst wrap */
    W25QXX_BURST_WRAP_32_BYTE = 0x40,        /**< 32 byte burst wrap */
    W25QXX_BURST_WRAP_64_BYTE = 0x60,        /**< 64 byte burst wrap */
} w25qxx_burst_wrap_t;

/**
 * @brief w25qxx status 1 enumeration definition
 */
typedef enum
{
    W25QXX_STATUS1_STATUS_REGISTER_PROTECT_0             = (1 << 7),        /**< status register protect 0 */
    W25QXX_STATUS1_SECTOR_PROTECT_OR_TOP_BOTTOM_PROTECT  = (1 << 6),        /**< sector protect bit or top / bottom protect bit */
    W25QXX_STATUS1_TOP_BOTTOM_PROTECT_OR_BLOCK_PROTECT_3 = (1 << 5),        /**< top / bottom protect bit or block 3 protect bit */
    W25QXX_STATUS1_BLOCK_PROTECT_2                       = (1 << 4),        /**< block 2 protect bit */
    W25QXX_STATUS1_BLOCK_PROTECT_1                       = (1 << 3),        /**< block 1 protect bit */
    W25QXX_STATUS1_BLOCK_PROTECT_0                       = (1 << 2),        /**< block 0 protect bit */
    W25QXX_STATUS1_WRITE_ENABLE_LATCH                    = (1 << 1),        /**< write enable latch */
    W25QXX_STATUS1_ERASE_WRITE_PROGRESS                  = (1 << 0),        /**< erase / write in progress */
} w25qxx_status1_t;

/**
 * @brief w25qxx status 2 enumeration definition
 */
typedef enum
{
    W25QXX_STATUS2_SUSPEND_STATUS                = (1 << 7),        /**< suspend status */
    W25QXX_STATUS2_COMPLEMENT_PROTECT            = (1 << 6),        /**< complement protect */
    W25QXX_STATUS2_SECURITY_REGISTER_3_LOCK_BITS = (1 << 5),        /**< security register 3 lock bits */
    W25QXX_STATUS2_SECURITY_REGISTER_2_LOCK_BITS = (1 << 4),        /**< security register 2 lock bits */
    W25QXX_STATUS2_SECURITY_REGISTER_1_LOCK_BITS = (1 << 3),        /**< security register 1 lock bits */
    W25QXX_STATUS2_QUAD_ENABLE                   = (1 << 1),        /**< quad enable */
    W25QXX_STATUS2_STATUS_REGISTER_PROTECT_1     = (1 << 0),        /**< status register protect 1 */
} w25qxx_status2_t;

/**
 * @brief w25qxx status 3 enumeration definition
 */
typedef enum
{
    W25QXX_STATUS3_HOLD_RESET_FUNCTION                   = (1 << 7),        /**< HOLD or RESET function */
    W25QXX_STATUS3_OUTPUT_DRIVER_STRENGTH_100_PERCENTAGE = (0 << 5),        /**< output driver strength 100% */
    W25QXX_STATUS3_OUTPUT_DRIVER_STRENGTH_75_PERCENTAGE  = (1 << 5),        /**< output driver strength 75% */
    W25QXX_STATUS3_OUTPUT_DRIVER_STRENGTH_50_PERCENTAGE  = (2 << 5),        /**< output driver strength 50% */
    W25QXX_STATUS3_OUTPUT_DRIVER_STRENGTH_25_PERCENTAGE  = (3 << 5),        /**< output driver strength 25% */
    W25QXX_STATUS3_WRITE_PROTECT_SELECTION               = (1 << 2),        /**< write protect selection */
    W25QXX_STATUS3_POWER_UP_ADDRESS_MODE                 = (1 << 1),        /**< power up address mode */
    W25QXX_STATUS3_CURRENT_ADDRESS_MODE                  = (1 << 0),        /**< current address mode */
} w25qxx_status3_t;

/**
 * @}
 */

/**
 * @addtogroup w25qxx_basic_driver
 * @{
 */

/**
 * @brief w25qxx handle structure definition
 */
typedef struct w25qxx_handle_s
{
    uint8_t (*spi_qspi_init)(void);                                                                    /**< point to a spi_qspi_init function address */
    uint8_t (*spi_qspi_deinit)(void);                                                                  /**< point to a spi_qspi_deinit function address */
    uint8_t (*spi_qspi_write_read)(uint8_t instruction, uint8_t instruction_line,
                                   uint32_t address, uint8_t address_line, uint8_t address_len,
                                   uint32_t alternate, uint8_t alternate_line, uint8_t alternate_len,
                                   uint8_t dummy, uint8_t *in_buf, uint32_t in_len,
                                   uint8_t *out_buf, uint32_t out_len, uint8_t data_line);             /**< point to a spi_qspi_write_read function address */
    void (*delay_ms)(uint32_t ms);                                                                     /**< point to a delay_ms function address */
    void (*delay_us)(uint32_t us);                                                                     /**< point to a delay_us function address */
    void (*debug_print)(const char *const fmt, ...);                                                   /**< point to a debug_print function address */
    uint8_t inited;                                                                                    /**< inited flag */
    uint16_t type;                                                                                     /**< chip type */
    uint8_t address_mode;                                                                              /**< address mode */
    uint8_t param;                                                                                     /**< param */
    uint8_t dummy;                                                                                     /**< dummy */
    uint8_t dual_quad_spi_enable;                                                                      /**< dual spi and quad spi enable */
    uint8_t spi_qspi;                                                                                  /**< spi qspi interface type */
    uint8_t buf[256 + 6];                                                                              /**< inner buffer */
    uint8_t buf_4k[4096 + 1];                                                                          /**< 4k inner buffer */
} w25qxx_handle_t;

/**
 * @brief w25qxx information structure definition
 */
typedef struct w25qxx_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[16];                /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} w25qxx_info_t;

/**
 * @}
 */

/**
 * @defgroup w25qxx_link_driver w25qxx link driver function
 * @brief    w25qxx link driver modules
 * @ingroup  w25qxx_driver
 * @{
 */

/**
 * @brief     initialize w25qxx_handle_t structure
 * @param[in] HANDLE points to a w25qxx handle structure
 * @param[in] STRUCTURE is w25qxx_handle_t
 * @note      none
 */
#define DRIVER_W25QXX_LINK_INIT(HANDLE, STRUCTURE)                memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link spi_qspi_init function
 * @param[in] HANDLE points to a w25qxx handle structure
 * @param[in] FUC points to a spi_qspi_init function address
 * @note      none
 */
#define DRIVER_W25QXX_LINK_SPI_QSPI_INIT(HANDLE, FUC)             (HANDLE)->spi_qspi_init = FUC

/**
 * @brief     link spi_qspi_deinit function
 * @param[in] HANDLE points to a w25qxx handle structure
 * @param[in] FUC points to a spi_qspi_deinit function address
 * @note      none
 */
#define DRIVER_W25QXX_LINK_SPI_QSPI_DEINIT(HANDLE, FUC)           (HANDLE)->spi_qspi_deinit = FUC

/**
 * @brief     link spi_qspi_write_read function
 * @param[in] HANDLE points to a w25qxx handle structure
 * @param[in] FUC points to a spi_qspi_write_read function address
 * @note      none
 */
#define DRIVER_W25QXX_LINK_SPI_QSPI_WRITE_READ(HANDLE, FUC)       (HANDLE)->spi_qspi_write_read = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to a w25qxx handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_W25QXX_LINK_DELAY_MS(HANDLE, FUC)                  (HANDLE)->delay_ms = FUC

/**
 * @brief     link delay_us function
 * @param[in] HANDLE points to a w25qxx handle structure
 * @param[in] FUC points to a delay_us function address
 * @note      none
 */
#define DRIVER_W25QXX_LINK_DELAY_US(HANDLE, FUC)                  (HANDLE)->delay_us = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to a w25qxx handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_W25QXX_LINK_DEBUG_PRINT(HANDLE, FUC)               (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup w25qxx_basic_driver w25qxx basic driver function
 * @brief    w25qxx basic driver modules
 * @ingroup  w25qxx_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to a w25qxx info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t w25qxx_info(w25qxx_info_t *info);

/**
 * @brief     enable or disable the dual quad spi
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t w25qxx_set_dual_quad_spi(w25qxx_handle_t *handle, w25qxx_bool_t enable);

/**
 * @brief      get the dual quad spi status
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[out] enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t w25qxx_get_dual_quad_spi(w25qxx_handle_t *handle, w25qxx_bool_t *enable);

/**
 * @brief     set the chip type
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] type is the chip type
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t w25qxx_set_type(w25qxx_handle_t *handle, w25qxx_type_t type);

/**
 * @brief      get the chip type
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[out] *type points to a chip type buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t w25qxx_get_type(w25qxx_handle_t *handle, w25qxx_type_t *type);

/**
 * @brief     set the chip interface
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t w25qxx_set_interface(w25qxx_handle_t *handle, w25qxx_interface_t interface);

/**
 * @brief      get the chip interface
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[out] *interface points to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t w25qxx_get_interface(w25qxx_handle_t *handle, w25qxx_interface_t *interface);

/**
 * @brief     set the chip address mode
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] mode is the address mode
 * @return    status code
 *             - 0 success
 *             - 1 set address mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 current type can't use this function
 * @note      none
 */
uint8_t w25qxx_set_address_mode(w25qxx_handle_t *handle, w25qxx_address_mode_t mode);

/**
 * @brief      get the chip address mode
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[out] *mode points to an address mode buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t w25qxx_get_address_mode(w25qxx_handle_t *handle, w25qxx_address_mode_t *mode);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to a w25qxx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi or qspi initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 get manufacturer device id failed
 *            - 5 enter qspi failed
 *            - 6 id is invalid
 *            - 7 reset failed
 *            - 8 set address mode failed
 * @note      none
 */
uint8_t w25qxx_init(w25qxx_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to a w25qxx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi or qspi deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 * @note      none
 */
uint8_t w25qxx_deinit(w25qxx_handle_t *handle);

/**
 * @brief      read data
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 address mode is invalid
 * @note       none
 */
uint8_t w25qxx_read(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len);

/**
 * @brief     write data
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] addr is the write address
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 read failed
 *            - 5 erase sector failed
 * @note      none
 */
uint8_t w25qxx_write(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len);

/**
 * @brief      read only in the spi interface
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 only spi read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 address mode is invalid
 *             - 5 only spi interface can use this function
 * @note       none
 */
uint8_t w25qxx_only_spi_read(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len);

/**
 * @brief      read in the fast mode
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 fast read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 address mode is invalid
 * @note       none
 */
uint8_t w25qxx_fast_read(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len);

/**
 * @brief     page program
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] addr is the programming address
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 page program failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 addr is invalid
 *            - 5 address mode is invalid
 *            - 6 page program timeout
 *            - 7 length is over 256
 * @note      len <= 256
 */
uint8_t w25qxx_page_program(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint16_t len);

/**
 * @brief     erase the 4k sector
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] addr is the erase address
 * @return    status code
 *            - 0 success
 *            - 1 sector erase 4k failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 addr is invalid
 *            - 5 address mode is invalid
 *            - 6 sector erase 4k timeout
 * @note      none
 */
uint8_t w25qxx_sector_erase_4k(w25qxx_handle_t *handle, uint32_t addr);

/**
 * @brief     erase the 32k block
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] addr is the erase address
 * @return    status code
 *            - 0 success
 *            - 1 block erase 32k failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 addr is invalid
 *            - 5 address mode is invalid
 *            - 6 block erase 32k timeout
 * @note      none
 */
uint8_t w25qxx_block_erase_32k(w25qxx_handle_t *handle, uint32_t addr);

/**
 * @brief     erase the 64k block
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] addr is the erase address
 * @return    status code
 *            - 0 success
 *            - 1 block erase 64k failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 addr is invalid
 *            - 5 address mode is invalid
 *            - 6 block erase 64k timeout
 * @note      none
 */
uint8_t w25qxx_block_erase_64k(w25qxx_handle_t *handle, uint32_t addr);

/**
 * @brief     erase the chip
 * @param[in] *handle points to a w25qxx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 chip erase failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 erase timeout
 * @note      none
 */
uint8_t w25qxx_chip_erase(w25qxx_handle_t *handle);

/**
 * @brief     power down
 * @param[in] *handle points to a w25qxx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 power down failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t w25qxx_power_down(w25qxx_handle_t *handle);

/**
 * @brief     release power down
 * @param[in] *handle points to a w25qxx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 release power down failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t w25qxx_release_power_down(w25qxx_handle_t *handle);

/**
 * @brief      get the manufacturer && device id information
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[out] *manufacturer points to a manufacturer buffer
 * @param[out] *device_id points to a device id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get manufacturer device id failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t w25qxx_get_manufacturer_device_id(w25qxx_handle_t *handle, uint8_t *manufacturer, uint8_t *device_id);

/**
 * @}
 */

/**
 * @defgroup w25qxx_advance_driver w25qxx advance driver function
 * @brief    w25qxx advance driver modules
 * @ingroup  w25qxx_driver
 * @{
 */

/**
 * @brief      read with dual output in the fast mode
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 fast read dual output failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 address mode is invalid
 *             - 5 qspi can't use this function
 *             - 6 standard spi can't use this function failed
 * @note       none
 */
uint8_t w25qxx_fast_read_dual_output(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len);

/**
 * @brief      read with quad output in the fast mode
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 fast read quad output failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 address mode is invalid
 *             - 5 qspi can't use this function
 *             - 6 standard spi can't use this function failed
 * @note       none
 */
uint8_t w25qxx_fast_read_quad_output(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len);

/**
 * @brief      read with dual io in the fast mode
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 fast read dual io failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 address mode is invalid
 *             - 5 qspi can't use this function
 *             - 6 standard spi can't use this function failed
 * @note       none
 */
uint8_t w25qxx_fast_read_dual_io(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len);

/**
 * @brief      read with quad io in the fast mode
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 fast read quad io failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 address mode is invalid
 *             - 6 standard spi can't use this function failed
 * @note       none
 */
uint8_t w25qxx_fast_read_quad_io(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len);

/**
 * @brief      word read with quad io
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 word read quad io failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 address mode is invalid
 *             - 5 qspi can't use this function
 *             - 6 standard spi can't use this function failed
 * @note       none
 */
uint8_t w25qxx_word_read_quad_io(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len);

/**
 * @brief      octal word read with quad io
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 octal word read quad io failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 address mode is invalid
 *             - 5 qspi can't use this function
 *             - 6 standard spi can't use this function failed
 * @note       none
 */
uint8_t w25qxx_octal_word_read_quad_io(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len);

/**
 * @brief     quad page program with quad input
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] addr is the programming address
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 quad page program failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 addr is invalid
 *            - 5 qspi can't use this function
 *            - 6 page program timeout
 *            - 7 length is over 256
 *            - 8 standard spi can't use this function failed
 * @note      len <= 256
 */
uint8_t w25qxx_page_program_quad_input(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint16_t len);

/**
 * @brief     enable writing
 * @param[in] *handle points to a w25qxx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 enable write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t w25qxx_enable_write(w25qxx_handle_t *handle);

/**
 * @brief     enable volatile sr writing
 * @param[in] *handle points to a w25qxx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 enable volatile sr write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t w25qxx_enable_volatile_sr_write(w25qxx_handle_t *handle);

/**
 * @brief     disable writing
 * @param[in] *handle points to a w25qxx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 disable write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t w25qxx_disable_write(w25qxx_handle_t *handle);

/**
 * @brief      get the status 1
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status 1 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t w25qxx_get_status1(w25qxx_handle_t *handle, uint8_t *status);

/**
 * @brief      get the status 2
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status 2 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t w25qxx_get_status2(w25qxx_handle_t *handle, uint8_t *status);

/**
 * @brief      get the status 3
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status 3 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t w25qxx_get_status3(w25qxx_handle_t *handle, uint8_t *status);

/**
 * @brief     set the status 1
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] status is the set status
 * @return    status code
 *            - 0 success
 *            - 1 set status 1 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 write status 1 timeout
 * @note      none
 */
uint8_t w25qxx_set_status1(w25qxx_handle_t *handle, uint8_t status);

/**
 * @brief     set the status 2
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] status is the set status
 * @return    status code
 *            - 0 success
 *            - 1 set status 2 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 write status 2 timeout
 * @note      none
 */
uint8_t w25qxx_set_status2(w25qxx_handle_t *handle, uint8_t status);

/**
 * @brief     set the status 3
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] status is the set status
 * @return    status code
 *            - 0 success
 *            - 1 set status 3 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 write status 3 timeout
 * @note      none
 */
uint8_t w25qxx_set_status3(w25qxx_handle_t *handle, uint8_t status);

/**
 * @brief     suspend erase or program 
 * @param[in] *handle points to a w25qxx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 erase program suspend failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t w25qxx_erase_program_suspend(w25qxx_handle_t *handle);

/**
 * @brief     resume erase or program 
 * @param[in] *handle points to a w25qxx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 erase program resume failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t w25qxx_erase_program_resume(w25qxx_handle_t *handle);

/**
 * @brief      get the manufacturer && device id information with dual io
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[out] *manufacturer points to a manufacturer buffer
 * @param[out] *device_id points to a device id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get manufacturer device id dual io failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 address mode is invalid
 *             - 5 qspi can't use this function
 *             - 6 standard spi can't use this function failed
 * @note       none
 */
uint8_t w25qxx_get_manufacturer_device_id_dual_io(w25qxx_handle_t *handle, uint8_t *manufacturer, uint8_t *device_id);

/**
 * @brief      get the manufacturer && device id information with quad io
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[out] *manufacturer points to a manufacturer buffer
 * @param[out] *device_id points to a device id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get manufacturer device id quad io failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 address mode is invalid
 *             - 5 qspi can't use this function
 *             - 6 standard spi can't use this function failed
 * @note       none
 */
uint8_t w25qxx_get_manufacturer_device_id_quad_io(w25qxx_handle_t *handle, uint8_t *manufacturer, uint8_t *device_id);

/**
 * @brief      get the jedec id information
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[out] *manufacturer points to a manufacturer buffer
 * @param[out] *device_id points to a device id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get jedec id failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t w25qxx_get_jedec_id(w25qxx_handle_t *handle, uint8_t *manufacturer, uint8_t device_id[2]);

/**
 * @brief     lock the whole block
 * @param[in] *handle points to a w25qxx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 global block lock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t w25qxx_global_block_lock(w25qxx_handle_t *handle);

/**
 * @brief     unlock the whole block
 * @param[in] *handle points to a w25qxx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 global block unlock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t w25qxx_global_block_unlock(w25qxx_handle_t *handle);

/**
 * @brief     set the read parameters
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] dummy is the qspi read dummy
 * @param[in] length is the qspi read wrap length
 * @return    status code
 *            - 0 success
 *            - 1 set read parameters failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 spi interface can't use this function
 * @note      none
 */
uint8_t w25qxx_set_read_parameters(w25qxx_handle_t *handle, w25qxx_qspi_read_dummy_t dummy, w25qxx_qspi_read_wrap_length_t length);

/**
 * @brief     enter the qspi mode
 * @param[in] *handle points to a w25qxx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 enter qspi mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 qspi interface can't use this function
 * @note      none
 */
uint8_t w25qxx_enter_qspi_mode(w25qxx_handle_t *handle);

/**
 * @brief     exit the qspi mode
 * @param[in] *handle points to a w25qxx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 exit qspi mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 spi interface can't use this function
 * @note      none
 */
uint8_t w25qxx_exit_qspi_mode(w25qxx_handle_t *handle);

/**
 * @brief     enable the reset
 * @param[in] *handle points to a w25qxx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 enable reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t w25qxx_enable_reset(w25qxx_handle_t *handle);

/**
 * @brief     reset the device
 * @param[in] *handle points to a w25qxx handle structure
 * @return    status code
 *            - 0 success
 *            - 1 reset device failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t w25qxx_reset_device(w25qxx_handle_t *handle);

/**
 * @brief      get the unique id
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[out] *id points to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get the unique id failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 address mode is invalid
 *             - 5 qspi can't use this function
 * @note       none
 */
uint8_t w25qxx_get_unique_id(w25qxx_handle_t *handle, uint8_t id[8]);

/**
 * @brief      get the sfdp
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[out] *sfdp points to an sfdp buffer
 * @return     status code
 *             - 0 success
 *             - 1 get the sfdp failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 qspi can't use this function
 * @note       none
 */
uint8_t w25qxx_get_sfdp(w25qxx_handle_t *handle, uint8_t sfdp[256]);

/**
 * @brief     erase the security register
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] num is the security register number
 * @return    status code
 *            - 0 success
 *            - 1 erase security register failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 address mode is invalid
 *            - 5 qspi can't use this function
 *            - 6 erase security register timeout
 * @note      none
 */
uint8_t w25qxx_erase_security_register(w25qxx_handle_t *handle, w25qxx_security_register_t num);

/**
 * @brief     program the security register
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] num is the security register number
 * @param[in] *data points to a data buffer
 * @return    status code
 *            - 0 success
 *            - 1 program security register failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 address mode is invalid
 *            - 5 qspi can't use this function
 *            - 6 program security register timeout
 * @note      none
 */
uint8_t w25qxx_program_security_register(w25qxx_handle_t *handle, w25qxx_security_register_t num, uint8_t data[256]);

/**
 * @brief      read the security register
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[in]  num is the security register number
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read security register failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 address mode is invalid
 *             - 5 qspi can't use this function
 *             - 6 program security register timeout
 * @note       none
 */
uint8_t w25qxx_read_security_register(w25qxx_handle_t *handle, w25qxx_security_register_t num, uint8_t data[256]);

/**
 * @brief     lock the individual block
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] addr is the block address
 * @return    status code
 *            - 0 success
 *            - 1 individual block lock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 address mode is invalid
 * @note      none
 */
uint8_t w25qxx_individual_block_lock(w25qxx_handle_t *handle, uint32_t addr);

/**
 * @brief     unlock the individual block
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] addr is the block address
 * @return    status code
 *            - 0 success
 *            - 1 individual block unlock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 address mode is invalid
 * @note      none
 */
uint8_t w25qxx_individual_block_unlock(w25qxx_handle_t *handle, uint32_t addr);

/**
 * @brief      read the block lock
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[in]  addr is the block address
 * @param[out] *value points to a value buffer
 * @return     status code
 *             - 0 success
 *             - 1 read block lock failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 address mode is invalid
 * @note       none
 */
uint8_t w25qxx_read_block_lock(w25qxx_handle_t *handle, uint32_t addr, uint8_t *value);

/**
 * @brief     set the burst with wrap
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] wrap is the burst wrap
 * @return    status code
 *            - 0 success
 *            - 1 set burst with wrap failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 qspi can't use this function
 * @note      none
 */
uint8_t w25qxx_set_burst_with_wrap(w25qxx_handle_t *handle, w25qxx_burst_wrap_t wrap);

/**
 * @}
 */

/**
 * @defgroup w25qxx_extern_driver w25qxx extern driver function
 * @brief    w25qxx extern driver modules
 * @ingroup  w25qxx_driver
 * @{
 */

/**
 * @brief      write and read register
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
 * @param[in]  *in_buf points to an input buffer
 * @param[in]  in_len is the input length
 * @param[out] *out_buf points to an output buffer
 * @param[in]  out_len is the output length
 * @param[in]  data_line is the data phy lines
 * @return     status code
 *            - 0 success
 *            - 1 write read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note       none
 */
uint8_t w25qxx_write_read_reg(w25qxx_handle_t *handle, uint8_t instruction, uint8_t instruction_line,
                              uint32_t address, uint8_t address_line, uint8_t address_len,
                              uint32_t alternate, uint8_t alternate_line, uint8_t alternate_len,
                              uint8_t dummy, uint8_t *in_buf, uint32_t in_len,
                              uint8_t *out_buf, uint32_t out_len, uint8_t data_line);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
