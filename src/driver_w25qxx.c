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
 * @file      driver_w25qxx.c
 * @brief     driver w25qxx source file
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

#include "driver_w25qxx.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Winbond W25QXX"        /**< chip name */
#define MANUFACTURER_NAME         "Winbond"               /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        2.7f                    /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f                    /**< chip max supply voltage */
#define MAX_CURRENT               25.0f                   /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                  /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                   /**< chip max operating temperature */
#define DRIVER_VERSION            1000                    /**< driver version */

/**
 * @brief chip command definition
 */
#define W25QXX_COMMAND_WRITE_ENABLE                      0x06        /**< write enable */
#define W25QXX_COMMAND_VOLATILE_SR_WRITE_ENABLE          0x50        /**< sr write enable */
#define W25QXX_COMMAND_WRITE_DISABLE                     0x04        /**< write disable */
#define W25QXX_COMMAND_READ_STATUS_REG1                  0x05        /**< read status register-1 */
#define W25QXX_COMMAND_READ_STATUS_REG2                  0x35        /**< read status register-2 */
#define W25QXX_COMMAND_READ_STATUS_REG3                  0x15        /**< read status register-3 */
#define W25QXX_COMMAND_WRITE_STATUS_REG1                 0x01        /**< write status register-1 */
#define W25QXX_COMMAND_WRITE_STATUS_REG2                 0x31        /**< write status register-2 */
#define W25QXX_COMMAND_WRITE_STATUS_REG3                 0x11        /**< write status register-3 */
#define W25QXX_COMMAND_CHIP_ERASE                        0xC7        /**< chip erase */
#define W25QXX_COMMAND_ERASE_PROGRAM_SUSPEND             0x75        /**< erase suspend */
#define W25QXX_COMMAND_ERASE_PROGRAM_RESUME              0x7A        /**< erase resume */
#define W25QXX_COMMAND_POWER_DOWN                        0xB9        /**< power down */
#define W25QXX_COMMAND_RELEASE_POWER_DOWN                0xAB        /**< release power down */
#define W25QXX_COMMAND_READ_MANUFACTURER                 0x90        /**< manufacturer */
#define W25QXX_COMMAND_JEDEC_ID                          0x9F        /**< jedec id */
#define W25QXX_COMMAND_GLOBAL_BLOCK_SECTOR_LOCK          0x7E        /**< global block lock */
#define W25QXX_COMMAND_GLOBAL_BLOCK_SECTOR_UNLOCK        0x98        /**< global block unlock */
#define W25QXX_COMMAND_ENTER_QSPI_MODE                   0x38        /**< enter spi mode */
#define W25QXX_COMMAND_ENABLE_RESET                      0x66        /**< enable reset */
#define W25QXX_COMMAND_RESET_DEVICE                      0x99        /**< reset device */
#define W25QXX_COMMAND_READ_UNIQUE_ID                    0x4B        /**< read unique id */
#define W25QXX_COMMAND_PAGE_PROGRAM                      0x02        /**< page program */
#define W25QXX_COMMAND_QUAD_PAGE_PROGRAM                 0x32        /**< quad page program */
#define W25QXX_COMMAND_SECTOR_ERASE_4K                   0x20        /**< sector erase */
#define W25QXX_COMMAND_BLOCK_ERASE_32K                   0x52        /**< block erase */
#define W25QXX_COMMAND_BLOCK_ERASE_64K                   0xD8        /**< block erase */
#define W25QXX_COMMAND_READ_DATA                         0x03        /**< read data */
#define W25QXX_COMMAND_FAST_READ                         0x0B        /**< fast read */
#define W25QXX_COMMAND_FAST_READ_DUAL_OUTPUT             0x3B        /**< fast read dual output */
#define W25QXX_COMMAND_FAST_READ_QUAD_OUTPUT             0x6B        /**< fast read quad output */
#define W25QXX_COMMAND_READ_SFDP_REGISTER                0x5A        /**< read SFDP register */
#define W25QXX_COMMAND_ERASE_SECURITY_REGISTER           0x44        /**< erase security register */
#define W25QXX_COMMAND_PROGRAM_SECURITY_REGISTER         0x42        /**< program security register */
#define W25QXX_COMMAND_READ_SECURITY_REGISTER            0x48        /**< read security register */
#define W25QXX_COMMAND_INDIVIDUAL_BLOCK_LOCK             0x36        /**< individual block lock */
#define W25QXX_COMMAND_INDIVIDUAL_BLOCK_UNLOCK           0x39        /**< individual block unlock */
#define W25QXX_COMMAND_READ_BLOCK_LOCK                   0x3D        /**< read block lock */
#define W25QXX_COMMAND_FAST_READ_DUAL_IO                 0xBB        /**< fast read dual I/O */
#define W25QXX_COMMAND_DEVICE_ID_DUAL_IO                 0x92        /**< device id dual I/O */
#define W25QXX_COMMAND_SET_BURST_WITH_WRAP               0x77        /**< set burst with wrap */
#define W25QXX_COMMAND_FAST_READ_QUAD_IO                 0xEB        /**< fast read quad I/O */
#define W25QXX_COMMAND_WORD_READ_QUAD_IO                 0xE7        /**< word read quad I/O */
#define W25QXX_COMMAND_OCTAL_WORD_READ_QUAD_IO           0xE3        /**< octal word read quad I/O */
#define W25QXX_COMMAND_DEVICE_ID_QUAD_IO                 0x94        /**< device id quad I/O */

/**
 * @brief      spi interface write read bytes
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[in]  *in_buf points to an input buffer
 * @param[in]  in_len is the input length
 * @param[out] *out_buf points to an output buffer
 * @param[in]  out_len is the output length
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       none
 */
static uint8_t a_w25qxx_spi_write_read(w25qxx_handle_t *handle, uint8_t *in_buf, uint32_t in_len, uint8_t *out_buf, uint32_t out_len)
{
    if (handle->spi_qspi_write_read(0x00, 0x00, 0x00000000, 0x00, 0x00,                /* write read data */
                                    0x00000000, 0x00, 0x00, 
                                    0x00, in_buf, in_len, out_buf, out_len, 1) != 0)
    {
        return 1;                                                                      /* return error */
    }
    else
    {
        return 0;                                                                      /* success return 0 */
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
 * @param[in]  *in_buf points to an input buffer
 * @param[in]  in_len is the input length
 * @param[out] *out_buf points to an output buffer
 * @param[in]  out_len is the output length
 * @param[in]  data_line is the data phy lines
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       none
 */
static uint8_t a_w25qxx_qspi_write_read(w25qxx_handle_t *handle, uint8_t instruction, uint8_t instruction_line,
                                        uint32_t address, uint8_t address_line, uint8_t address_len,
                                        uint32_t alternate, uint8_t alternate_line, uint8_t alternate_len,
                                        uint8_t dummy, uint8_t *in_buf, uint32_t in_len,
                                        uint8_t *out_buf, uint32_t out_len, uint8_t data_line)
{
    if (handle->spi_qspi_write_read(instruction, instruction_line, address, address_line, address_len,        /* write read data */
                                    alternate, alternate_line, alternate_len, 
                                    dummy, in_buf, in_len, out_buf, out_len, data_line) != 0)
    {
        return 1;                                                                                             /* return error */
    }
    else
    {
        return 0;                                                                                             /* success return 0 */
    }
}

/**
 * @brief     enable or disable the dual quad spi
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t w25qxx_set_dual_quad_spi(w25qxx_handle_t *handle, w25qxx_bool_t enable)
{
    if (handle == NULL)                                    /* check handle */
    {
        return 2;                                          /* return error */
    }

    handle->dual_quad_spi_enable = (uint8_t)enable;        /* set enable */
    
    return 0;                                              /* success return 0 */
}

/**
 * @brief      get the dual quad spi status
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[out] enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t w25qxx_get_dual_quad_spi(w25qxx_handle_t *handle, w25qxx_bool_t *enable)
{
    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }

    *enable = (w25qxx_bool_t)(handle->dual_quad_spi_enable);        /* get enable */
    
    return 0;                                                       /* success return 0 */
}

/**
 * @brief     set the chip type
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] type is the chip type
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t w25qxx_set_type(w25qxx_handle_t *handle, w25qxx_type_t type)
{
    if (handle == NULL)                  /* check handle */
    {
        return 2;                        /* return error */
    }

    handle->type = (uint16_t)type;       /* set type */
    
    return 0;                            /* success return 0 */
}

/**
 * @brief      get the chip type
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[out] *type points to a chip type buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t w25qxx_get_type(w25qxx_handle_t *handle, w25qxx_type_t *type)
{
    if (handle == NULL)                           /* check handle */
    {
        return 2;                                 /* return error */
    }

    *type = (w25qxx_type_t)(handle->type);        /* get type */
    
    return 0;                                     /* success return 0 */
}

/**
 * @brief     set the chip interface
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t w25qxx_set_interface(w25qxx_handle_t *handle, w25qxx_interface_t interface)
{
    if (handle == NULL)                           /* check handle */
    {
        return 2;                                 /* return error */
    }

    handle->spi_qspi = (uint8_t)interface;        /* set interface */
    
    return 0;                                     /* success return 0 */
}

/**
 * @brief      get the chip interface
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[out] *interface points to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t w25qxx_get_interface(w25qxx_handle_t *handle, w25qxx_interface_t *interface)
{
    if (handle == NULL)                                         /* check handle */
    {
        return 2;                                               /* return error */
    }

    *interface = (w25qxx_interface_t)(handle->spi_qspi);        /* get interface */
    
    return 0;                                                   /* success return 0 */
}

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
uint8_t w25qxx_set_address_mode(w25qxx_handle_t *handle, w25qxx_address_mode_t mode)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }   
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    if (handle->type < W25Q256)                                                       /* check type */
    {
        handle->debug_print("w25qxx: current type can't use this function.\n");       /* current type can't use this function */
       
        return 4;                                                                     /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                     /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                        /* enable dual quad spi */
        {
            if (mode == W25QXX_ADDRESS_MODE_3_BYTE)                                   /* address 3 mode byte */
            {
                buf[0] = 0xE9;                                                        /* 3 byte mode */
            }
            else                                                                      /* address 4 mode byte */
            {
                buf[0] = 0xB7;                                                        /* 4 byte mode */
            }
            res = a_w25qxx_qspi_write_read(handle, buf[0], 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                         /* qspi write read */
            if (res != 0)                                                             /* check result */
            {
                handle->debug_print("w25qxx: set address mode failed.\n");            /* set address mode failed */
               
                return 1;                                                             /* return error */
            }
        }
        else                                                                          /* single spi */
        {
            if (mode == W25QXX_ADDRESS_MODE_3_BYTE)                                   /* address 3 mode byte */
            {
                buf[0] = 0xE9;                                                        /* 3 byte mode */
            }
            else                                                                      /* address 4 mode byte */
            {
                buf[0] = 0xB7;                                                        /* 4 byte mode */
            }
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);        /* spi write read */
            if (res != 0)                                                             /* check result */
            {
                handle->debug_print("w25qxx: set address mode failed.\n");            /* set address mode failed */
               
                return 1;                                                             /* return error */
            }
        }
    }
    else                                                                              /* qspi interface */
    {
        if (mode == W25QXX_ADDRESS_MODE_3_BYTE)                                       /* address 3 mode byte */
        {
            buf[0] = 0xE9;                                                            /* 3 byte mode */
        }
        else                                                                          /* address 4 mode byte */
        {
            buf[0] = 0xB7;                                                            /* 4 byte mode */
        }
        res = a_w25qxx_qspi_write_read(handle, buf[0], 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                             /* qspi write read */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("w25qxx: set address mode failed.\n");                /* set address mode failed */
           
            return 1;                                                                 /* return error */
        }
    }
    
    handle->address_mode = (uint8_t)mode;                                             /* set address mode */
    
    return 0;                                                                         /* success return 0 */
}

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
uint8_t w25qxx_get_address_mode(w25qxx_handle_t *handle, w25qxx_address_mode_t *mode)
{
    if (handle == NULL)                                          /* check handle */
    {
        return 2;                                                /* return error */
    }   
    if (handle->inited != 1)                                     /* check handle initialization */
    {
        return 3;                                                /* return error */
    }

    *mode = (w25qxx_address_mode_t)(handle->address_mode);       /* get address mode */
    
    return 0;                                                    /* success return 0 */
}

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
uint8_t w25qxx_enable_write(w25qxx_handle_t *handle)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }   
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                     /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                        /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                         /* qspi write read */
            if (res != 0)                                                             /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                /* write enable failed */
               
                return 1;                                                             /* return error */
            }
        }
        else                                                                          /* single spi */
        {
            buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                     /* write enable command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);        /* spi write read */
            if (res != 0)                                                             /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                /* write enable failed */
               
                return 1;                                                             /* return error */
            }
        }
    }
    else                                                                              /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                             /* qspi write read */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("w25qxx: write enable failed.\n");                    /* write enable failed */
           
            return 1;                                                                 /* return error */
        }
    }
    
    return 0;                                                                         /* success return 0 */
}

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
uint8_t w25qxx_enable_volatile_sr_write(w25qxx_handle_t *handle)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }   
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }

    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                            /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                               /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, 
                                           W25QXX_COMMAND_VOLATILE_SR_WRITE_ENABLE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                /* qspi write read */
            if (res != 0)                                                                    /* check result */
            {
                handle->debug_print("w25qxx: sr write enable failed.\n");                    /* sr write enable failed */
               
                return 1;                                                                    /* return error */
            }
        }
        else                                                                                 /* single spi */
        {
            buf[0] = W25QXX_COMMAND_VOLATILE_SR_WRITE_ENABLE;                                /* sr write enable command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);               /* spi write read */
            if (res != 0)                                                                    /* check result */
            {
                handle->debug_print("w25qxx: sr write enable failed.\n");                    /* sr write enable failed */
               
                return 1;                                                                    /* return error */
            }
        }
    }
    else                                                                                     /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_VOLATILE_SR_WRITE_ENABLE, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                    /* qspi write read */
        if (res != 0)                                                                        /* check result */
        {
            handle->debug_print("w25qxx: sr write enable failed.\n");                        /* sr write enable failed */
           
            return 1;                                                                        /* return error */
        }
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t w25qxx_disable_write(w25qxx_handle_t *handle)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }   
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }

    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                     /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                        /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_DISABLE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                         /* qspi write read */
            if (res != 0)                                                             /* check result */
            {
                handle->debug_print("w25qxx: write disable failed.\n");               /* write disable failed */
               
                return 1;                                                             /* return error */
            }
        }
        else                                                                          /* single spi */
        {
            buf[0] = W25QXX_COMMAND_WRITE_DISABLE;                                    /* write disable command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);        /* spi qspi write read */
            if (res != 0)                                                             /* check result */
            {
                handle->debug_print("w25qxx: write disable failed.\n");               /* write disable failed */
               
                return 1;                                                             /* return error */
            }
        }
    }
    else                                                                              /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_DISABLE, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                             /* qspi write read */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("w25qxx: write disable failed.\n");                   /* write disable failed */
           
            return 1;                                                                 /* return error */
        }
    }
    
    return 0;                                                                         /* success return 0 */
}

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
uint8_t w25qxx_get_status1(w25qxx_handle_t *handle, uint8_t *status)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }   
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }

    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                  /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                     /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle,
                                           W25QXX_COMMAND_READ_STATUS_REG1, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           status, 1, 1);                          /* qspi write read */
            if (res != 0)                                                          /* check result */
            {
                handle->debug_print("w25qxx: get status1 failed.\n");              /* get status1 failed */
               
                return 1;                                                          /* return error */
            }
        }
        else                                                                       /* single spi */
        {
            buf[0] = W25QXX_COMMAND_READ_STATUS_REG1;                              /* read status1 command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 
                                          1, status, 1);                           /* spi write read */
            if (res != 0)                                                          /* check result */
            {
                handle->debug_print("w25qxx: get status1 failed.\n");              /* get status1 failed */
               
                return 1;                                                          /* return error */
            }
        }
    }
    else                                                                           /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       status, 1, 4);                              /* qspi write read */
        if (res != 0)                                                              /* check result */
        {
            handle->debug_print("w25qxx: get status1 failed.\n");                  /* get status1 failed */
           
            return 1;                                                              /* return error */
        }
    }
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t w25qxx_get_status2(w25qxx_handle_t *handle, uint8_t *status)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }   
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }

    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                  /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                     /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, 
                                           W25QXX_COMMAND_READ_STATUS_REG2, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           status, 1, 1);                          /* qspi write read */
            if (res != 0)                                                          /* check result */
            {
                handle->debug_print("w25qxx: get status2 failed.\n");              /* get status2 failed */
               
                return 1;                                                          /* return error */
            }
        }
        else                                                                       /* single spi */
        {
            buf[0] = W25QXX_COMMAND_READ_STATUS_REG2;                              /* read status2 command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf,
                                          1, status, 1);                           /* spi write read */
            if (res != 0)                                                          /* check result */
            {
                handle->debug_print("w25qxx: get status2 failed.\n");              /* get status2 failed */
               
                return 1;                                                          /* return error */
            }
        }
    }
    else                                                                           /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG2, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       status, 1, 4);                              /* qspi write read */
        if (res != 0)                                                              /* check result */
        {
            handle->debug_print("w25qxx: get status2 failed.\n");                  /* get status2 failed */
           
            return 1;                                                              /* return error */
        }
    }
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t w25qxx_get_status3(w25qxx_handle_t *handle, uint8_t *status)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }   
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }

    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                  /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                     /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle,
                                           W25QXX_COMMAND_READ_STATUS_REG3, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           status, 1, 1);                          /* qspi write read */
            if (res != 0)                                                          /* check result */
            {
                handle->debug_print("w25qxx: get status3 failed.\n");              /* get status3 failed */
               
                return 1;                                                          /* return error */
            }
        }
        else                                                                       /* single spi */
        {
            buf[0] = W25QXX_COMMAND_READ_STATUS_REG3;                              /* read status3 command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, 
                                          status, 1);                              /* spi write read */
            if (res != 0)                                                          /* check result */
            {
                handle->debug_print("w25qxx: get status3 failed.\n");              /* get status3 failed */
               
                return 1;                                                          /* return error */
            }
        }
    }
    else                                                                           /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG3, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       status, 1, 4);                              /* qspi write read */
        if (res != 0)                                                              /* check result */
        {
            handle->debug_print("w25qxx: get status3 failed.\n");                  /* get status3 failed */
           
            return 1;                                                              /* return error */
        }
    }
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t w25qxx_set_status1(w25qxx_handle_t *handle, uint8_t status)
{
    uint8_t res;
    uint8_t buf[2];
    uint32_t timeout;
    uint8_t status_check;
    
    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }   
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }

    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                        /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                           /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_VOLATILE_SR_WRITE_ENABLE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                            /* qspi write read */
            if (res != 0)                                                                                /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                   /* write enable failed */
               
                return 1;                                                                                /* return error */
            }
            buf[0] = status;                                                                             /* set status */
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_STATUS_REG1, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, (uint8_t *)buf, 1,
                                           NULL, 0x00, 1);                                               /* qspi write read */
            if (res != 0)                                                                                /* check result */
            {
                handle->debug_print("w25qxx: set status1 failed.\n");                                    /* set status1 failed */
               
                return 1;                                                                                /* return error */
            }
            
            timeout = 1000;                                                                              /* max 1000 ms */
            while (timeout != 0)                                                                         /* check timeout */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                              (uint8_t *)&status_check, 1, 1);                           /* qspi write read */
                if (res != 0)                                                                            /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                /* get status1 failed */
                   
                    return 1;                                                                            /* return error */
                }
                if ((status_check & 0x01) == 0x00)                                                       /* check status */
                {
                    break;                                                                               /* break */
                }
                timeout--;                                                                               /* timeout-- */
                handle->delay_ms(1);                                                                     /* delay 1 ms */
            }
            if (timeout == 0)                                                                            /* check timeout */
            {
                handle->debug_print("w25qxx: write status 1 timeout.\n");                                /* write status 1 timeout */
               
                return 4;                                                                                /* return error */
            }
            else
            {
                return 0;                                                                                /* success return 0 */
            }
        }
        else                                                                                             /* single spi */
        {
            buf[0] = W25QXX_COMMAND_VOLATILE_SR_WRITE_ENABLE;                                            /* sr write enable command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                           /* spi write read */
            if (res != 0)                                                                                /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                   /* write enable failed */
               
                return 1;                                                                                /* return error */
            }
            buf[0] = W25QXX_COMMAND_WRITE_STATUS_REG1;                                                   /* write status1 command */
            buf[1] = status;                                                                             /* set status */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 2, NULL, 0);                           /* spi write read */
            if (res != 0)                                                                                /* check result */
            {
                handle->debug_print("w25qxx: set status1 failed.\n");                                    /* set status1 failed */
               
                return 1;                                                                                /* return error */
            }
            
            timeout = 1000;                                                                              /* max 1000 ms */
            while (timeout != 0)                                                                         /* check timeout */
            {
                buf[0] = W25QXX_COMMAND_READ_STATUS_REG1;                                                /* read status1 command */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1,
                                             (uint8_t *)&status_check, 1);                               /* spi write read */
                if (res != 0)                                                                            /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                /* get status1 failed */
                   
                    return 1;                                                                            /* return error */
                }
                if ((status_check & 0x01) == 0x00)                                                       /* check status */
                {
                    break;                                                                               /* break */
                }
                timeout--;                                                                               /* timeout-- */
                handle->delay_ms(1);                                                                     /* delay 1 ms */
            }
            if (timeout == 0)                                                                            /* check timeout */
            {
                handle->debug_print("w25qxx: write status 1 timeout.\n");                                /* write status 1 timeout */
               
                return 4;                                                                                /* return error */
            }
            else
            {
                return 0;                                                                                /* success return 0 */
            }
        }
    }
    else                                                                                                 /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_VOLATILE_SR_WRITE_ENABLE, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                                /* qspi write read */
        if (res != 0)                                                                                    /* check result */
        {
            handle->debug_print("w25qxx: write enable failed.\n");                                       /* write enable failed */
           
            return 1;                                                                                    /* return error */
        }
        buf[0] = status;                                                                                 /* set status */
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_STATUS_REG1, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, (uint8_t *)buf, 1,
                                       NULL, 0x00, 4);                                                   /* qspi write read */
        if (res != 0)                                                                                    /* check result */
        {
            handle->debug_print("w25qxx: set status1 failed.\n");                                        /* set status1 failed */
           
            return 1;                                                                                    /* return error */
        }
        
        timeout = 1000;                                                                                  /* max 1000 ms */
        while (timeout != 0)                                                                             /* check timeout */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 4,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                          (uint8_t *)&status_check, 1, 4);                               /* qspi write read */
            if (res != 0)                                                                                /* check result */
            {
                handle->debug_print("w25qxx: get status1 failed.\n");                                    /* get status1 failed */
               
                return 1;                                                                                /* return error */
            }
            if ((status_check & 0x01) == 0x00)                                                           /* check status */
            {
                break;                                                                                   /* break */
            }
            timeout--;                                                                                   /* timeout-- */
            handle->delay_ms(1);                                                                         /* delay 1 ms */
        }
        if (timeout == 0)                                                                                /* check timeout */
        {
            handle->debug_print("w25qxx: write status 1 timeout.\n");                                    /* write status 1 timeout */
           
            return 4;                                                                                    /* return error */
        }
        else
        {
            return 0;                                                                                    /* success return 0 */
        }
    }
}

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
uint8_t w25qxx_set_status2(w25qxx_handle_t *handle, uint8_t status)
{
    uint8_t res;
    uint8_t buf[2];
    uint32_t timeout;
    uint8_t status_check;
    
    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }   
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }

    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                        /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                           /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_VOLATILE_SR_WRITE_ENABLE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                            /* qspi write read */
            if (res != 0)                                                                                /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                   /* write enable failed */
               
                return 1;                                                                                /* return error */
            }
            buf[0] = status;                                                                             /* set status */
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_STATUS_REG2, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, (uint8_t *)buf, 1,
                                           NULL, 0x00, 1);                                               /* qspi write read */
            if (res != 0)                                                                                /* check result */
            {
                handle->debug_print("w25qxx: set status2 failed.\n");                                    /* set status2 failed */
               
                return 1;                                                                                /* return error */
            }
            
            timeout = 1000;                                                                              /* max 1000 ms */
            while (timeout != 0)                                                                         /* check timeout */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                              (uint8_t *)&status_check, 1, 1);                           /* qspi write read */
                if (res != 0)                                                                            /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                /* get status1 failed */
                   
                    return 1;                                                                            /* return error */
                }
                if ((status_check & 0x01) == 0x00)                                                       /* check status */
                {
                    break;                                                                               /* break */
                }
                timeout--;                                                                               /* timeout-- */
                handle->delay_ms(1);                                                                     /* delay 1 ms */
            }
            if (timeout == 0)                                                                            /* check timeout */
            {
                handle->debug_print("w25qxx: write status 2 timeout.\n");                                /* write status 2 timeout */
               
                return 4;                                                                                /* return error */
            }
            else
            {
                return 0;                                                                                /* success return 0 */
            }
        }
        else                                                                                             /* single spi */
        {
            buf[0] = W25QXX_COMMAND_VOLATILE_SR_WRITE_ENABLE;                                            /* sr write enable command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                           /* spi write read */
            if (res != 0)                                                                                /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                   /* write enable failed */
               
                return 1;                                                                                /* return error */
            }
            buf[0] = W25QXX_COMMAND_WRITE_STATUS_REG2;                                                   /* write status2 command */
            buf[1] = status;                                                                             /* set status */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 2, NULL, 0);                           /* spi write read */
            if (res != 0)                                                                                /* check result */
            {
                handle->debug_print("w25qxx: set status2 failed.\n");                                    /* set status2 failed */
               
                return 1;                                                                                /* return error */
            }
            
            timeout = 1000;                                                                              /* max 1000 ms */
            while (timeout != 0)                                                                         /* check timeout */
            {
                buf[0] = W25QXX_COMMAND_READ_STATUS_REG1;                                                /* read status1 command */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1,
                                             (uint8_t *)&status_check, 1);                               /* spi write read */
                if (res != 0)                                                                            /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                /* get status1 failed */
                   
                    return 1;                                                                            /* return error */
                }
                if ((status_check & 0x01) == 0x00)                                                       /* check status */
                {
                    break;                                                                               /* break */
                }
                timeout--;                                                                               /* timeout-- */
                handle->delay_ms(1);                                                                     /* delay 1 ms */
            }
            if (timeout == 0)                                                                            /* check timeout */
            {
                handle->debug_print("w25qxx: write status 2 timeout.\n");                                /* write status 2 timeout */
               
                return 4;                                                                                /* return error */
            }
            else
            {
                return 0;                                                                                /* success return 0 */
            }
        }
    }
    else                                                                                                 /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_VOLATILE_SR_WRITE_ENABLE, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                                /* qspi write read */
        if (res != 0)                                                                                    /* check result */
        {
            handle->debug_print("w25qxx: write enable failed.\n");                                       /* write enable failed */
           
            return 1;                                                                                    /* return error */
        }
        buf[0] = status;                                                                                 /* set status */
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_STATUS_REG2, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, (uint8_t *)buf, 1,
                                       NULL, 0x00, 4);                                                   /* qspi write read */
        if (res != 0)                                                                                    /* check result */
        {
            handle->debug_print("w25qxx: set status2 failed.\n");                                        /* set status2 failed */
           
            return 1;                                                                                    /* return error */
        }
        
        timeout = 1000;                                                                                  /* max 1000 ms */
        while (timeout != 0)                                                                             /* check timeout */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 4,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                          (uint8_t *)&status_check, 1, 4);                               /* qspi write read */
            if (res != 0)                                                                                /* check result */
            {
                handle->debug_print("w25qxx: get status1 failed.\n");                                    /* get status1 failed */
               
                return 1;                                                                                /* return error */
            }
            if ((status_check & 0x01) == 0x00)                                                           /* check status */
            {
                break;                                                                                   /* break */
            }
            timeout--;                                                                                   /* timeout-- */
            handle->delay_ms(1);                                                                         /* delay 1 ms */
        }
        if (timeout == 0)                                                                                /* check timeout */
        {
            handle->debug_print("w25qxx: write status 2 timeout.\n");                                    /* write status 2 timeout */
           
            return 4;                                                                                    /* return error */
        }
        else
        {
            return 0;                                                                                    /* success return 0 */
        }
    }
}

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
uint8_t w25qxx_set_status3(w25qxx_handle_t *handle, uint8_t status)
{
    uint8_t res;
    uint8_t buf[2];
    uint32_t timeout;
    uint8_t status_check;
    
    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }   
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }

    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                        /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                           /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_VOLATILE_SR_WRITE_ENABLE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                            /* qspi write read */
            if (res != 0)                                                                                /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                   /* write enable failed */
               
                return 1;                                                                                /* return error */
            }
            buf[0] = status;                                                                             /* set status */
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_STATUS_REG3, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, (uint8_t *)buf, 1,
                                           NULL, 0x00, 1);                                               /* qspi write read */
            if (res != 0)                                                                                /* check result */
            {
                handle->debug_print("w25qxx: set status3 failed.\n");                                    /* set status3 failed */
               
                return 1;                                                                                /* return error */
            }
            
            timeout = 1000;                                                                              /* max 1000 ms */
            while (timeout != 0)                                                                         /* check timeout */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                              (uint8_t *)&status_check, 1, 1);                           /* qspi write read */
                if (res != 0)                                                                            /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                /* get status1 failed */
                   
                    return 1;                                                                            /* return error */
                }
                if ((status_check & 0x01) == 0x00)                                                       /* check status */
                {
                    break;                                                                               /* break */
                }
                timeout--;                                                                               /* timeout-- */
                handle->delay_ms(1);                                                                     /* delay 1 ms */
            }
            if (timeout == 0)                                                                            /* check timeout */
            {
                handle->debug_print("w25qxx: write status 3 timeout.\n");                                /* write status 3 timeout */
               
                return 4;                                                                                /* return error */
            }
            else
            {
                return 0;                                                                                /* success return 0 */
            }
        }
        else                                                                                             /* single spi */
        {
            buf[0] = W25QXX_COMMAND_VOLATILE_SR_WRITE_ENABLE;                                            /* sr write enable command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                           /* spi write read */
            if (res != 0)                                                                                /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                   /* write enable failed */
               
                return 1;                                                                                /* return error */
            }
            buf[0] = W25QXX_COMMAND_WRITE_STATUS_REG3;                                                   /* write status3 command */
            buf[1] = status;                                                                             /* set status */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 2, NULL, 0);                           /* spi write read */
            if (res != 0)                                                                                /* check result */
            {
                handle->debug_print("w25qxx: set status3 failed.\n");                                    /* set status3 failed */
               
                return 1;                                                                                /* return error */
            }
            
            timeout = 1000;                                                                              /* max 1000 ms */
            while (timeout != 0)                                                                         /* check timeout */
            {
                buf[0] = W25QXX_COMMAND_READ_STATUS_REG1;                                                /* read status1 command */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1,
                                             (uint8_t *)&status_check, 1);                               /* spi write read */
                if (res != 0)                                                                            /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                /* get status1 failed */
                   
                    return 1;                                                                            /* return error */
                }
                if ((status_check & 0x01) == 0x00)                                                       /* check status */
                {
                    break;                                                                               /* break */
                }
                timeout--;                                                                               /* timeout-- */
                handle->delay_ms(1);                                                                     /* delay 1 ms */
            }
            if (timeout == 0)                                                                            /* check timeout */
            {
                handle->debug_print("w25qxx: write status 3 timeout.\n");                                /* write status 3 timeout */
               
                return 4;                                                                                /* return error */
            }
            else
            {
                return 0;                                                                                /* success return 0 */
            }
        }
    }
    else                                                                                                 /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_VOLATILE_SR_WRITE_ENABLE, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                                /* qspi write read */
        if (res != 0)                                                                                    /* check result */
        {
            handle->debug_print("w25qxx: write enable failed.\n");                                       /* write enable failed */
           
            return 1;                                                                                    /* return error */
        }
        buf[0] = status;                                                                                 /* set status */
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_STATUS_REG3, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, (uint8_t *)buf, 1,
                                       NULL, 0x00, 4);                                                   /* qspi write read */
        if (res != 0)                                                                                    /* check result */
        {
            handle->debug_print("w25qxx: set status3 failed.\n");                                        /* set status3 failed */
           
            return 1;                                                                                    /* return error */
        }
        
        timeout = 1000;                                                                                  /* max 1000 ms */
        while (timeout != 0)                                                                             /* check timeout */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 4,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                          (uint8_t *)&status_check, 1, 4);                               /* qspi write read */
            if (res != 0)                                                                                /* check result */
            {
                handle->debug_print("w25qxx: get status1 failed.\n");                                    /* get status1 failed */
               
                return 1;                                                                                /* return error */
            }
            if ((status_check & 0x01) == 0x00)                                                           /* check status */
            {
                break;                                                                                   /* break */
            }
            timeout--;                                                                                   /* timeout-- */
            handle->delay_ms(1);                                                                         /* delay 1 ms */
        }
        if (timeout == 0)                                                                                /* check timeout */
        {
            handle->debug_print("w25qxx: write status 3 timeout.\n");                                    /* write status 3 timeout */
           
            return 4;                                                                                    /* return error */
        }
        else
        {
            return 0;                                                                                    /* success return 0 */
        }
    }
}

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
uint8_t w25qxx_chip_erase(w25qxx_handle_t *handle)
{
    uint8_t res;
    uint8_t status;
    uint8_t buf[1];
    uint32_t timeout;
    
    if (handle == NULL)                                                                            /* check handle */
    {
        return 2;                                                                                  /* return error */
    }   
    if (handle->inited != 1)                                                                       /* check handle initialization */
    {
        return 3;                                                                                  /* return error */
    }

    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                  /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                     /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                      /* qspi write read */
            if (res != 0)                                                                          /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                             /* write enable failed */
               
                return 1;                                                                          /* return error */
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_CHIP_ERASE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                      /* qspi write read */
            if (res != 0)                                                                          /* check result */
            {
                handle->debug_print("w25qxx: chip erase failed.\n");                               /* chip erase failed */
               
                return 1;                                                                          /* return error */
            }
            timeout = 400 * 1000;                                                                  /* max 400s */
            while (timeout != 0)                                                                   /* check timeout */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                              (uint8_t *)&status, 1, 1);                           /* qspi write read */
                if (res != 0)                                                                      /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                          /* get status1 failed */
                   
                    return 1;                                                                      /* return error */
                }
                if ((status & 0x01) == 0x00)                                                       /* check status */
                {
                    break;                                                                         /* break */
                }
                timeout--;                                                                         /* timeout-- */
                handle->delay_ms(1);                                                               /* delay 1 ms */
            }
            if (timeout == 0)                                                                      /* check timeout */
            {
                handle->debug_print("w25qxx: erase timeout.\n");                                   /* erase timeout */
               
                return 4;                                                                          /* return error */
            }
            else
            {
                return 0;                                                                          /* success return 0 */
            }
        }
        else                                                                                       /* single spi */
        {
            buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                  /* write enable command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                     /* spi write read */
            if (res != 0)                                                                          /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                             /* write enable failed */
               
                return 1;                                                                          /* return error */
            }
            buf[0] = W25QXX_COMMAND_CHIP_ERASE;                                                    /* chip erase command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                     /* spi qspi write read */
            if (res != 0)                                                                          /* check result */
            {
                handle->debug_print("w25qxx: chip erase failed.\n");                               /* chip erase failed */
               
                return 1;                                                                          /* return error */
            }
            timeout = 400 * 1000;                                                                  /* max 400s */
            while (timeout != 0)                                                                   /* check timeout */
            {
                buf[0] = W25QXX_COMMAND_READ_STATUS_REG1;                                          /* read status1 command */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1,
                                             (uint8_t *)&status, 1);                               /* spi write read */
                if (res != 0)                                                                      /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                          /* get status1 failed */
                   
                    return 1;                                                                      /* return error */
                }
                if ((status & 0x01) == 0x00)                                                       /* check status */
                {
                    break;                                                                         /* break */
                }
                timeout--;                                                                         /* timeout-- */
                handle->delay_ms(1);                                                               /* delay 1 ms */
            }
            if (timeout == 0)                                                                      /* check timeout */
            {
                handle->debug_print("w25qxx: erase timeout.\n");                                   /* erase timeout */
               
                return 4;                                                                          /* return error */
            }
            else
            {
                return 0;                                                                          /* success return 0 */
            }
        }
    }
    else                                                                                           /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                          /* qspi write read */
        if (res != 0)                                                                              /* check result */
        {
            handle->debug_print("w25qxx: write enable failed.\n");                                 /* write enable failed */
           
            return 1;                                                                              /* return error */
        }
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_CHIP_ERASE, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                          /* qspi write read */
        if (res != 0)                                                                              /* check result */
        {
            handle->debug_print("w25qxx: chip erase failed.\n");                                   /* chip erase failed */
           
            return 1;                                                                              /* return error */
        }
        timeout = 400 * 1000;                                                                      /* max 400s */
        while (timeout != 0)                                                                       /* check timeout */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 4,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                          (uint8_t *)&status, 1, 4);                               /* qspi write read */
            if (res != 0)                                                                          /* check result */
            {
                handle->debug_print("w25qxx: get status1 failed.\n");                              /* get status1 failed */
               
                return 1;                                                                          /* return error */
            }
            if ((status & 0x01) == 0x00)                                                           /* check status */
            {
                break;                                                                             /* break */
            }
            timeout--;                                                                             /* timeout-- */
            handle->delay_ms(1);                                                                   /* delay 1 ms */
        }
        if (timeout == 0)                                                                          /* check timeout */
        {
            handle->debug_print("w25qxx: erase timeout.\n");                                       /* erase timeout */
           
            return 4;                                                                              /* return error */
        }
        else
        {
            return 0;                                                                              /* success return 0 */
        }
    }
}

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
uint8_t w25qxx_erase_program_suspend(w25qxx_handle_t *handle)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }   
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                             /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_ERASE_PROGRAM_SUSPEND, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                 /* qspi write read */
            if (res != 0)                                                                     /* check result */
            {
                handle->debug_print("w25qxx: erase program suspend failed.\n");               /* erase program suspend failed */
               
                return 1;                                                                     /* return error */
            }
        }
        else                                                                                  /* single spi */
        {
            buf[0] = W25QXX_COMMAND_ERASE_PROGRAM_SUSPEND;                                    /* erase program suspend command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                /* spi write read */
            if (res != 0)                                                                     /* check result */
            {
                handle->debug_print("w25qxx: erase program suspend failed.\n");               /* erase program suspend failed */
               
                return 1;                                                                     /* return error */
            }
            
            return 0;                                                                         /* success return 0 */
        }
    }
    else                                                                                      /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_ERASE_PROGRAM_SUSPEND, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                     /* qspi write read */
        if (res != 0)                                                                         /* check result */
        {
            handle->debug_print("w25qxx: erase program suspend failed.\n");                   /* erase program suspend failed */
           
            return 1;                                                                         /* return error */
        }
    }
    
    return 0;                                                                                 /* success return 0 */
}

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
uint8_t w25qxx_erase_program_resume(w25qxx_handle_t *handle)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }   
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                             /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_ERASE_PROGRAM_RESUME, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                 /* qspi write read */
            if (res != 0)                                                                     /* check result */
            {
                handle->debug_print("w25qxx: erase program resume failed.\n");                /* erase program resume failed */
               
                return 1;                                                                     /* return error */
            }
        }
        else                                                                                  /* single spi */
        {
            buf[0] = W25QXX_COMMAND_ERASE_PROGRAM_RESUME;                                     /* erase program resume command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                /* spi write read */
            if (res != 0)                                                                     /* check result */
            {
                handle->debug_print("w25qxx: erase program resume failed.\n");                /* erase program resume failed */
               
                return 1;                                                                     /* return error */
            }
        }
    }
    else                                                                                      /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_ERASE_PROGRAM_RESUME, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                     /* qspi write read */
        if (res != 0)                                                                         /* check result */
        {
            handle->debug_print("w25qxx: erase program resume failed.\n");                    /* erase program resume failed */
           
            return 1;                                                                         /* return error */
        }
    }
    
    return 0;                                                                                 /* success return 0 */
}

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
uint8_t w25qxx_power_down(w25qxx_handle_t *handle)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }   
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }

    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                  /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                     /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_POWER_DOWN, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                      /* qspi write read */
            if (res != 0)                                                          /* check result */
            {
                handle->debug_print("w25qxx: power down failed.\n");               /* power down failed */
               
                return 1;                                                          /* return error */
            }
        }
        else                                                                       /* single spi */
        {
            buf[0] = W25QXX_COMMAND_POWER_DOWN;                                    /* power down command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf,
                                          1, NULL, 0);                             /* spi write read */
            if (res != 0)                                                          /* check result */
            {
                handle->debug_print("w25qxx: power down failed.\n");               /* power down failed */
               
                return 1;                                                          /* return error */
            }
        }
    }
    else                                                                           /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, 0xFF, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0, NULL, 0x00,
                                       NULL, 0x00, 0);                             /* spi write read */
        if (res != 0)                                                              /* check result */
        {
            handle->debug_print("w25qxx: exit qspi mode failed.\n");               /* exit qspi mode failed */
           
            return 1;                                                              /* return error */
        }
        handle->delay_ms(10);                                                      /* delay 10 ms */
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_POWER_DOWN, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                          /* qspi write read */
        if (res != 0)                                                              /* check result */
        {
            handle->debug_print("w25qxx: power down failed.\n");                   /* power down failed */
           
            return 1;                                                              /* return error */
        }
    }
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t w25qxx_release_power_down(w25qxx_handle_t *handle)
{
    uint8_t res;
    uint8_t id;
    uint8_t buf[4];
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }   
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                          /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                             /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle,
                                           W25QXX_COMMAND_RELEASE_POWER_DOWN, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           3 * 8, NULL, 0x00,
                                          (uint8_t *)&id, 1, 1);                           /* qspi write read */
            if (res != 0)                                                                  /* check result */
            {
                handle->debug_print("w25qxx: release power down failed.\n");               /* release power down failed */
               
                return 1;                                                                  /* return error */
            }
        }
        else                                                                               /* single spi */
        {
            buf[0] = W25QXX_COMMAND_RELEASE_POWER_DOWN;                                    /* release power down command */
            buf[1] = 0xFF;                                                                 /* dummy */
            buf[2] = 0xFF;                                                                 /* dummy */
            buf[3] = 0xFF;                                                                 /* dummy */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 4, (uint8_t *)&id, 1);   /* spi write read */
            if (res != 0)                                                                  /* check result */
            {
                handle->debug_print("w25qxx: release power down failed.\n");               /* release power down failed */
               
                return 1;                                                                  /* return error */
            }
        }
    }
    else                                                                                   /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_RELEASE_POWER_DOWN, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       6, NULL, 0x00,
                                      (uint8_t *)&id, 1, 4);                               /* qspi write read */
        if (res != 0)                                                                      /* check result */
        {
            handle->debug_print("w25qxx: release power down failed.\n");                   /* release power down failed */
           
            return 1;                                                                      /* return error */
        }
    }
    
    return 0;                                                                              /* success return 0 */
}

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
uint8_t w25qxx_get_manufacturer_device_id(w25qxx_handle_t *handle, uint8_t *manufacturer, uint8_t *device_id)
{
    uint8_t res;
    uint8_t buf[4];
    uint8_t out[2];
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }   
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                          /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                             /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_MANUFACTURER, 1,
                                           0x00000000, 1, 3,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                          (uint8_t *)out, 2, 1);                           /* qspi write read */
            if (res != 0)                                                                  /* check result */
            {
                handle->debug_print("w25qxx: get manufacturer device id failed.\n");       /* get manufacturer device id failed */
               
                return 1;                                                                  /* return error */
            }
            *manufacturer = out[0];                                                        /* set manufacturer */
            *device_id = out[1];                                                           /* set device id */
        }
        else                                                                               /* single spi */
        {
            buf[0] = W25QXX_COMMAND_READ_MANUFACTURER;                                     /* read manufacturer command */
            buf[1] = 0x00;                                                                 /* dummy */
            buf[2] = 0x00;                                                                 /* dummy */
            buf[3] = 0x00;                                                                 /* dummy */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 4,
                                         (uint8_t *)out, 2);                               /* spi write read */
            if (res != 0)                                                                  /* check result */
            {
                handle->debug_print("w25qxx: get manufacturer device id failed.\n");       /* get manufacturer device id failed */
               
                return 1;                                                                  /* return error */
            }
            *manufacturer = out[0];                                                        /* set manufacturer */
            *device_id = out[1];                                                           /* set device id */
        }
    }
    else                                                                                   /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_MANUFACTURER, 4,
                                       0x00000000, 4, 3,
                                       0x00000000, 0x00, 0x00,
                                       0, NULL, 0x00,
                                      (uint8_t *)out, 2, 4);                               /* qspi write read */
        if (res != 0)                                                                      /* check result */
        {
            handle->debug_print("w25qxx: get manufacturer device id failed.\n");           /* get manufacturer device id failed */
           
            return 1;                                                                      /* return error */
        }
        *manufacturer = out[0];                                                            /* set manufacturer */
        *device_id = out[1];                                                               /* set device id */
    }
    
    return 0;                                                                              /* success return 0 */
}

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
uint8_t w25qxx_get_manufacturer_device_id_dual_io(w25qxx_handle_t *handle, uint8_t *manufacturer, uint8_t *device_id)
{
    uint8_t res;
    uint8_t out[2];
    
    if (handle == NULL)                                                                             /* check handle */
    {
        return 2;                                                                                   /* return error */
    }   
    if (handle->inited != 1)                                                                        /* check handle initialization */
    {
        return 3;                                                                                   /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                   /* spi interface */
    {
        if (handle->dual_quad_spi_enable == 0)                                                      /* check spi */
        {
            handle->debug_print("w25qxx: standard spi can't use this function failed.\n");          /* standard spi can't use this function failed */
           
            return 6;                                                                               /* return error */
        }
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                     /* 3 address mode */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_DEVICE_ID_DUAL_IO, 1,
                                           0x00000000, 2, 3,
                                           0x000000FF, 2, 1,
                                           0x00, NULL, 0x00,
                                          (uint8_t *)out, 2, 2);                                    /* spi write read */
            if (res != 0)                                                                           /* check result */
            {
                handle->debug_print("w25qxx: get manufacturer device id dual io failed.\n");        /* get manufacturer device id dual io failed */
               
                return 1;                                                                           /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_DEVICE_ID_DUAL_IO, 1,
                                           0x00000000, 2, 4,
                                           0x000000FF, 2, 1,
                                           0x00, NULL, 0x00,
                                          (uint8_t *)out, 2, 2);                                    /* spi write read */
            if (res != 0)                                                                           /* check result */
            {
                handle->debug_print("w25qxx: get manufacturer device id dual io failed.\n");        /* get manufacturer device id dual io failed */
               
                return 1;                                                                           /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                              /* address mode is invalid */
           
            return 4;                                                                               /* return error */
        }
        *manufacturer = out[0];                                                                     /* set manufacturer */
        *device_id = out[1];                                                                        /* set device id */
    }
    else
    {
        handle->debug_print("w25qxx: qspi can't use this function.\n");                             /* qspi can't use this function */
       
        return 5;                                                                                   /* return error */
    }
    
    return 0;                                                                                       /* success return 0 */
}

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
uint8_t w25qxx_get_manufacturer_device_id_quad_io(w25qxx_handle_t *handle, uint8_t *manufacturer, uint8_t *device_id)
{
    uint8_t res;
    uint8_t out[2];
    
    if (handle == NULL)                                                                             /* check handle */
    {
        return 2;                                                                                   /* return error */
    }   
    if (handle->inited != 1)                                                                        /* check handle initialization */
    {
        return 3;                                                                                   /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                   /* spi interface */
    {
        if (handle->dual_quad_spi_enable == 0)                                                      /* check spi */
        {
            handle->debug_print("w25qxx: standard spi can't use this function failed.\n");          /* standard spi can't use this function failed */
           
            return 6;                                                                               /* return error */
        }
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                     /* 3 address mode */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_DEVICE_ID_QUAD_IO, 1,
                                           0x00000000, 4, 3,
                                           0x000000FF, 4, 1,
                                           4, NULL, 0x00,
                                          (uint8_t *)out, 2, 4);                                    /* spi write read */
            if (res != 0)                                                                           /* check result */
            {
                handle->debug_print("w25qxx: get manufacturer device id quad io failed.\n");        /* get manufacturer device id quad io failed */
               
                return 1;                                                                           /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_DEVICE_ID_QUAD_IO, 1,
                                           0x00000000, 4, 4,
                                           0x000000FF, 4, 1,
                                           4, NULL, 0x00,
                                          (uint8_t *)out, 2, 4);                                    /* spi write read */
            if (res != 0)                                                                           /* check result */
            {
                handle->debug_print("w25qxx: get manufacturer device id quad io failed.\n");        /* get manufacturer device id quad io failed */
               
                return 1;                                                                           /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                              /* address mode is invalid */
           
            return 4;                                                                               /* return error */
        }
        *manufacturer = out[0];                                                                     /* set manufacturer */
        *device_id = out[1];                                                                        /* set device id */
    }
    else
    {
        handle->debug_print("w25qxx: qspi can't use this function.\n");                             /* qspi can't use this function */
       
        return 5;                                                                                   /* return error */
    }
    
    return 0;                                                                                       /* success return 0 */
}

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
uint8_t w25qxx_get_jedec_id(w25qxx_handle_t *handle, uint8_t *manufacturer, uint8_t device_id[2])
{
    uint8_t res;
    uint8_t buf[1];
    uint8_t out[3];
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }   
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                          /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                             /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_JEDEC_ID, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                          (uint8_t *)out, 3, 1);                           /* qspi write read */
            if (res != 0)                                                                  /* check result */
            {
                handle->debug_print("w25qxx: get jedec id failed.\n");                     /* get jedec id failed */
               
                return 1;                                                                  /* return error */
            }
            *manufacturer = out[0];                                                        /* set manufacturer */
            device_id[0] = out[1];                                                         /* set device id 0 */
            device_id[1] = out[2];                                                         /* set device id 1 */
        }
        else                                                                               /* single spi */
        {
            buf[0] = W25QXX_COMMAND_JEDEC_ID;                                              /* jedec id command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1,
                                         (uint8_t *)out, 3);                               /* spi write read */
            if (res != 0)                                                                  /* check result */
            {
                handle->debug_print("w25qxx: get jedec id failed.\n");                     /* get jedec id  failed */
               
                return 1;                                                                  /* return error */
            }
            *manufacturer = out[0];                                                        /* set manufacturer */
            device_id[0] = out[1];                                                         /* set device id 0 */
            device_id[1] = out[2];                                                         /* set device id 1 */
        }
    }
    else                                                                                   /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_JEDEC_ID, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0, NULL, 0x00,
                                      (uint8_t *)out, 3, 4);                               /* qspi write read */
        if (res != 0)                                                                      /* check result */
        {
            handle->debug_print("w25qxx: get jedec id failed.\n");                         /* get jedec id failed */
           
            return 1;                                                                      /* return error */
        }
        *manufacturer = out[0];                                                            /* set manufacturer */
        device_id[0] = out[1];                                                             /* set device id 0 */
        device_id[1] = out[2];                                                             /* set device id 1 */
    }
    
    return 0;                                                                              /* success return 0 */
}

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
uint8_t w25qxx_global_block_lock(w25qxx_handle_t *handle)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }   
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                   /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_GLOBAL_BLOCK_SECTOR_LOCK, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                    /* qspi write read */
            if (res != 0)                                                                        /* check result */
            {
                handle->debug_print("w25qxx: global block lock failed.\n");                      /* global block lock failed */
               
                return 1;                                                                        /* return error */
            }
        }
        else                                                                                     /* single spi */
        {
            buf[0] = W25QXX_COMMAND_GLOBAL_BLOCK_SECTOR_LOCK;                                    /* global block lock command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                   /* spi write read */
            if (res != 0)                                                                        /* check result */
            {
                handle->debug_print("w25qxx: global block lock failed.\n");                      /* global block lock failed */
               
                return 1;                                                                        /* return error */
            }
        }
    }
    else                                                                                         /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_GLOBAL_BLOCK_SECTOR_LOCK, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                        /* qspi write read */
        if (res != 0)                                                                            /* check result */
        {
            handle->debug_print("w25qxx: global block lock failed.\n");                          /* global block lock failed */
           
            return 1;                                                                            /* return error */
        }
    }
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t w25qxx_global_block_unlock(w25qxx_handle_t *handle)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }   
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                   /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_GLOBAL_BLOCK_SECTOR_UNLOCK, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                    /* qspi write read */
            if (res != 0)                                                                        /* check result */
            {
                handle->debug_print("w25qxx: global block unlock failed.\n");                    /* global block unlock failed */
               
                return 1;                                                                        /* return error */
            }
        }
        else                                                                                     /* single spi */
        {
            buf[0] = W25QXX_COMMAND_GLOBAL_BLOCK_SECTOR_UNLOCK;                                  /* global block unlock command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                   /* spi qspi write read */
            if (res != 0)                                                                        /* check result */
            {
                handle->debug_print("w25qxx: global block unlock failed.\n");                    /* global block unlock failed */
               
                return 1;                                                                        /* return error */
            }
        }
    }
    else                                                                                         /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_GLOBAL_BLOCK_SECTOR_UNLOCK, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                        /* qspi write read */
        if (res != 0)                                                                            /* check result */
        {
            handle->debug_print("w25qxx: global block unlock failed.\n");                        /* global block unlock failed */
           
            return 1;                                                                            /* return error */
        }
    }
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t w25qxx_set_read_parameters(w25qxx_handle_t *handle, w25qxx_qspi_read_dummy_t dummy, w25qxx_qspi_read_wrap_length_t length)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }   
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                       /* spi interface */
    {
        handle->debug_print("w25qxx: spi interface can't use this function.\n");        /* spi interface can't use this function */
       
        return 4;                                                                       /* return error */
    }
    else
    {
        handle->param = (uint8_t)((dummy << 4) | (length << 0));                        /* set param */
        buf[0] = handle->param;                                                         /* set param */
        res = a_w25qxx_qspi_write_read(handle, 0xC0, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0, (uint8_t *)buf, 1,
                                       NULL, 0x00, 4);                                  /* qspi write read */
        if (res != 0)                                                                   /* check result */
        {
            handle->debug_print("w25qxx: set read parameters failed.\n");               /* set read parameters failed */
           
            return 5;                                                                   /* return error */
        }
        if (((buf[0] >> 4) & 0x03) == 0x00)                                             /* if 0x00 */
        {
            handle->dummy = 2;                                                          /* dummy 2 */
        }
        else if (((buf[0] >> 4) & 0x03) == 0x01)                                        /* if 0x01 */
        {
            handle->dummy = 4;                                                          /* dummy 4 */
        }
        else if (((buf[0] >> 4) & 0x03) == 0x02)                                        /* if 0x02 */
        {
            handle->dummy = 6;                                                          /* dummy 6 */
        }
        else                                                                            /* if 0x03 */
        {
            handle->dummy = 8;                                                          /* dummy 8 */
        }
    }
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t w25qxx_enter_qspi_mode(w25qxx_handle_t *handle)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }   
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_QSPI)                                      /* qspi interface */
    {
        handle->debug_print("w25qxx: qspi interface can't use this function.\n");       /* qspi interface can't use this function */
       
        return 4;                                                                       /* return error */
    }
    else
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_ENTER_QSPI_MODE, 1,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0, NULL, 0x00,
                                       NULL, 0x00, 0x00);                               /* qspi write read */
        if (res != 0)                                                                   /* check result */
        {
            handle->debug_print("w25qxx: enter qspi mode failed.\n");                   /* enter qspi mode failed */
           
            return 1;                                                                   /* return error */
        }
        handle->delay_ms(10);                                                           /* delay 10 ms */
        buf[0] = handle->param;                                                         /* set param */
        res = a_w25qxx_qspi_write_read(handle, 0xC0, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0, (uint8_t *)buf, 1,
                                       NULL, 0x00, 4);                                  /* qspi write read */
        if (res != 0)                                                                   /* check result */
        {
            handle->debug_print("w25qxx: set read parameters failed.\n");               /* set read parameters failed */
           
            return 5;                                                                   /* return error */
        }
        if (((buf[0] >> 4) & 0x03) == 0x00)                                             /* if 0x00 */
        {
            handle->dummy = 2;                                                          /* dummy 2 */
        }
        else if (((buf[0] >> 4) & 0x03) == 0x01)                                        /* if 0x01 */
        {
            handle->dummy = 4;                                                          /* dummy 4 */
        }
        else if (((buf[0] >> 4) & 0x03) == 0x02)                                        /* if 0x02 */
        {
            handle->dummy = 6;                                                          /* dummy 6 */
        }
        else                                                                            /* if 0x03 */
        {
            handle->dummy = 8;                                                          /* dummy 8 */
        }
    }
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t w25qxx_exit_qspi_mode(w25qxx_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }   
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                       /* spi interface */
    {
        handle->debug_print("w25qxx: spi interface can't use this function.\n");        /* spi interface can't use this function */
       
        return 4;                                                                       /* return error */
    }
    else
    {
        res = a_w25qxx_qspi_write_read(handle, 0xFF, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0, NULL, 0x00,
                                       NULL, 0x00, 0x00);                               /* qspi write read */
        if (res != 0)                                                                   /* check result */
        {
            handle->debug_print("w25qxx: exit qspi mode failed.\n");                    /* exit qspi mode failed */
           
            return 1;                                                                   /* return error */
        }
    }
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t w25qxx_enable_reset(w25qxx_handle_t *handle)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }   
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                    /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                       /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_ENABLE_RESET, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           NULL, 0x00, 0x00);                        /* qspi write read */
            if (res != 0)                                                            /* check result */
            {
                handle->debug_print("w25qxx: enable reset failed.\n");               /* enable reset failed */
               
                return 1;                                                            /* return error */
            }
        }
        else                                                                         /* single spi */
        {
            buf[0] = W25QXX_COMMAND_ENABLE_RESET;                                    /* enable reset command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf,
                                          1, NULL, 0);                               /* spi write read */
            if (res != 0)                                                            /* check result */
            {
                handle->debug_print("w25qxx: enable reset failed.\n");               /* enable reset failed */
               
                return 1;                                                            /* return error */
            }
        }
    }
    else                                                                             /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_ENABLE_RESET, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0, NULL, 0x00,
                                       NULL, 0x00, 0x00);                            /* qspi write read */
        if (res != 0)                                                                /* check result */
        {
            handle->debug_print("w25qxx: enable reset failed.\n");                   /* enable reset failed */
           
            return 1;                                                                /* return error */
        }
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t w25qxx_reset_device(w25qxx_handle_t *handle)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }   
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                    /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                       /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_RESET_DEVICE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           NULL, 0x00, 0x00);                        /* qspi write read */
            if (res != 0)                                                            /* check result */
            {
                handle->debug_print("w25qxx: reset device failed.\n");               /* reset device failed */
               
                return 1;                                                            /* return error */
            }
        }
        else                                                                         /* single spi */
        {
            buf[0] = W25QXX_COMMAND_RESET_DEVICE;                                    /* reset device command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf,
                                          1, NULL, 0);                               /* spi write read */
            if (res != 0)                                                            /* check result */
            {
                handle->debug_print("w25qxx: reset device failed.\n");               /* reset device failed */
               
                return 1;                                                            /* return error */
            }
        }
    }
    else                                                                             /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_RESET_DEVICE, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0, NULL, 0x00,
                                       NULL, 0x00, 0x00);                            /* qspi write read */
        if (res != 0)                                                                /* check result */
        {
            handle->debug_print("w25qxx: reset device failed.\n");                   /* reset device failed */
           
            return 1;                                                                /* return error */
        }
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t w25qxx_get_unique_id(w25qxx_handle_t *handle, uint8_t id[8])
{
    uint8_t res;
    uint8_t buf[6];
    
    if (handle == NULL)                                                                                           /* check handle */
    {
        return 2;                                                                                                 /* return error */
    }   
    if (handle->inited != 1)                                                                                      /* check handle initialization */
    {
        return 3;                                                                                                 /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                                 /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                                    /* enable dual quad spi */
        {
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                               /* 3 address mode */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_UNIQUE_ID, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               4 * 8, NULL, 0x00,
                                              (uint8_t *)id, 8, 1);                                               /* qspi write read */
                if (res != 0)                                                                                     /* check result */
                {
                    handle->debug_print("w25qxx: read unique id failed.\n");                                      /* read unique id failed */
                   
                    return 1;                                                                                     /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_UNIQUE_ID, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               5 * 8, NULL, 0x00,
                                              (uint8_t *)id, 8, 1);                                               /* qspi write read */
                if (res != 0)                                                                                     /* check result */
                {
                    handle->debug_print("w25qxx: read unique id failed.\n");                                      /* read unique id failed */
                   
                    return 1;                                                                                     /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                        /* address mode is invalid */
               
                return 4;                                                                                         /* return error */
            }
        }
        else                                                                                                      /* single spi */
        {
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                               /* 3 address mode */
            {
                buf[0] = W25QXX_COMMAND_READ_UNIQUE_ID;                                                           /* read unique id command */
                buf[1] = 0x00;                                                                                    /* dummy */
                buf[2] = 0x00;                                                                                    /* dummy */
                buf[3] = 0x00;                                                                                    /* dummy */
                buf[4] = 0x00;                                                                                    /* dummy */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 5, (uint8_t *)id, 8);                       /* spi write read */
                if (res != 0)                                                                                     /* check result */
                {
                    handle->debug_print("w25qxx: read unique id failed.\n");                                      /* read unique id failed */
                   
                    return 1;                                                                                     /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                buf[0] = W25QXX_COMMAND_READ_UNIQUE_ID;                                                           /* read unique id command */
                buf[1] = 0x00;                                                                                    /* dummy */
                buf[2] = 0x00;                                                                                    /* dummy */
                buf[3] = 0x00;                                                                                    /* dummy */
                buf[4] = 0x00;                                                                                    /* dummy */
                buf[5] = 0x00;                                                                                    /* dummy */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 6, (uint8_t *)id, 8);                       /* spi write read */
                if (res != 0)                                                                                     /* check result */
                {
                    handle->debug_print("w25qxx: read unique id failed.\n");                                      /* read unique id failed */
                   
                    return 1;                                                                                     /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                        /* address mode is invalid */
               
                return 4;                                                                                         /* return error */
            }
        }
    }
    else                                                                                                          /* qspi interface */
    {
        handle->debug_print("w25qxx: qspi can't use this function.\n");                                           /* qspi can't use this function */
       
        return 5;                                                                                                 /* return error */
    }
    
    return 0;                                                                                                     /* success return 0 */
}

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
uint8_t w25qxx_get_sfdp(w25qxx_handle_t *handle, uint8_t sfdp[256])
{
    uint8_t res;
    uint8_t buf[5];
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }   
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                             /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle,
                                           W25QXX_COMMAND_READ_SFDP_REGISTER, 1,
                                           0x00000000, 1, 3,
                                           0x00000000, 0x00, 0x00,
                                           8, NULL, 0x00,
                                          (uint8_t *)sfdp, 256, 1);                           /* qspi write read */
            if (res != 0)                                                                     /* check result */
            {
                handle->debug_print("w25qxx: read unique id failed.\n");                      /* read unique id failed */
               
                return 1;                                                                     /* return error */
            }
        }
        else                                                                                  /* single spi */
        {
            buf[0] = W25QXX_COMMAND_READ_SFDP_REGISTER;                                       /* read sfdp command */
            buf[1] = 0x00;                                                                    /* 0x00 */
            buf[2] = 0x00;                                                                    /* 0x00 */
            buf[3] = 0x00;                                                                    /* 0x00 */
            buf[4] = 0x00;                                                                    /* dummy */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 5,
                                         (uint8_t *)sfdp, 256);                               /* spi write read */
            if (res != 0)                                                                     /* check result */
            {
                handle->debug_print("w25qxx: get sfdp failed.\n");                            /* get sfdp failed */
               
                return 1;                                                                     /* return error */
            }
        }
    }
    else                                                                                      /* qspi interface */
    {
        handle->debug_print("w25qxx: qspi can't use this function.\n");                       /* qspi can't use this function */
       
        return 4;                                                                             /* return error */
    }
    
    return 0;                                                                                 /* success return 0 */
}

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
uint8_t w25qxx_erase_security_register(w25qxx_handle_t *handle, w25qxx_security_register_t num)
{
    uint8_t res;
    uint8_t status;
    uint32_t timeout;
    uint8_t buf[5];
    
    if (handle == NULL)                                                                                       /* check handle */
    {
        return 2;                                                                                             /* return error */
    }   
    if (handle->inited != 1)                                                                                  /* check handle initialization */
    {
        return 3;                                                                                             /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                             /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                                /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle,
                                           W25QXX_COMMAND_WRITE_ENABLE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                                 /* qspi write read */
            if (res != 0)                                                                                     /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                        /* write enable failed */
               
                return 1;                                                                                     /* return error */
            }
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                           /* 3 address mode */
            {
                res = a_w25qxx_qspi_write_read(handle,
                                               W25QXX_COMMAND_ERASE_SECURITY_REGISTER, 1,
                                               num << 8, 1, 3,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                             /* qspi write read */
                if (res != 0)                                                                                 /* check result */
                {
                    handle->debug_print("w25qxx: erase security register failed.\n");                         /* erase security register failed */
                   
                    return 1;                                                                                 /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                res = a_w25qxx_qspi_write_read(handle,
                                               W25QXX_COMMAND_ERASE_SECURITY_REGISTER, 1,
                                               num << 8, 1, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                             /* qspi write read */
                if (res != 0)                                                                                 /* check result */
                {
                    handle->debug_print("w25qxx: erase security register failed.\n");                         /* erase security register failed */
                   
                    return 1;                                                                                 /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                    /* address mode is invalid */
               
                return 4;                                                                                     /* return error */
            }
            
            timeout = 100 * 100;                                                                              /* max 1000 ms */
            while (timeout != 0)                                                                              /* check timeout */
            {
                res = a_w25qxx_qspi_write_read(handle,
                                               W25QXX_COMMAND_READ_STATUS_REG1, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                              (uint8_t *)&status, 1, 1);                                      /* qspi write read */
                if (res != 0)                                                                                 /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                     /* get status1 failed */
                   
                    return 1;                                                                                 /* return error */
                }
                if ((status & 0x01) == 0x00)                                                                  /* check status */
                {
                    break;                                                                                    /* break */
                }
                timeout--;                                                                                    /* timeout-- */
                handle->delay_us(10);                                                                         /* delay 10 us */
            }
            if (timeout == 0)
            {
                handle->debug_print("w25qxx: erase security register timeout.\n");                            /* erase security register timeout */
               
                return 6;                                                                                     /* return error */
            }
        }
        else                                                                                                  /* single spi */
        {
            buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                             /* write enable command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                                /* spi write read */
            if (res != 0)                                                                                     /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                        /* write enable failed */
               
                return 1;                                                                                     /* return error */
            }
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                           /* 3 address mode */
            {
                buf[0] = W25QXX_COMMAND_ERASE_SECURITY_REGISTER;                                              /* erase security register command */
                buf[1] = 0x00;                                                                                /* 0x00 */
                buf[2] = num;                                                                                 /* num */
                buf[3] = 0x00;                                                                                /* 0x00 */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 4, NULL, 0);                            /* spi write read */
                if (res != 0)                                                                                 /* check result */
                {
                    handle->debug_print("w25qxx: erase security register failed.\n");                         /* erase security register failed */
                   
                    return 1;                                                                                 /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                buf[0] = W25QXX_COMMAND_ERASE_SECURITY_REGISTER;                                              /* erase security register command */
                buf[1] = 0x00;                                                                                /* 0x00 */
                buf[2] = 0x00;                                                                                /* 0x00 */
                buf[3] = num;                                                                                 /* num */
                buf[4] = 0x00;                                                                                /* 0x00 */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 5, NULL, 0);                            /* spi write read */
                if (res != 0)                                                                                 /* check result */
                {
                    handle->debug_print("w25qxx: erase security register failed.\n");                         /* erase security register failed */
                   
                    return 1;                                                                                 /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                    /* address mode is invalid */
               
                return 4;                                                                                     /* return error */
            }
            
            timeout = 100 * 100;                                                                              /* max 1000 ms */
            while (timeout != 0)                                                                              /* check timeout */
            {
                buf[0] = W25QXX_COMMAND_READ_STATUS_REG1;                                                     /* read status1 command */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, (uint8_t *)&status, 1);              /* spi write read */
                if (res != 0)                                                                                 /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                     /* get status1 failed */
                   
                    return 1;                                                                                 /* return error */
                }
                if ((status & 0x01) == 0x00)                                                                  /* check status */
                {
                    break;                                                                                    /* break */
                }
                timeout--;                                                                                    /* timeout-- */
                handle->delay_us(10);                                                                         /* delay 10 us */
            }
            if (timeout == 0)
            {
                handle->debug_print("w25qxx: erase security register timeout.\n");                            /* erase security register timeout */
               
                return 6;                                                                                     /* return error */
            }
        }
    }
    else                                                                                                      /* qspi interface */
    {
        handle->debug_print("w25qxx: qspi can't use this function.\n");                                       /* qspi can't use this function */
       
        return 5;                                                                                             /* return error */
    }
    
    return 0;                                                                                                 /* success return 0 */
}

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
uint8_t w25qxx_program_security_register(w25qxx_handle_t *handle, w25qxx_security_register_t num, uint8_t data[256])
{
    uint8_t res;
    uint8_t status;
    uint32_t timeout;
    uint8_t buf[5];
    
    if (handle == NULL)                                                                                       /* check handle */
    {
        return 2;                                                                                             /* return error */
    }   
    if (handle->inited != 1)                                                                                  /* check handle initialization */
    {
        return 3;                                                                                             /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                             /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                                /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle,
                                           W25QXX_COMMAND_WRITE_ENABLE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                                 /* qspi write read */
            if (res != 0)                                                                                     /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                        /* write enable failed */
               
                return 1;                                                                                     /* return error */
            }
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                           /* 3 address mode */
            {
                res = a_w25qxx_qspi_write_read(handle,
                                               W25QXX_COMMAND_PROGRAM_SECURITY_REGISTER, 1,
                                               num << 8, 1, 3,
                                               0x00000000, 0x00, 0x00,
                                               0x00, data, 256,
                                               NULL, 0x00, 1);                                                /* qspi write read */
                if (res != 0)                                                                                 /* check result */
                {
                    handle->debug_print("w25qxx: program security register failed.\n");                       /* program security register failed */
                   
                    return 1;                                                                                 /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                res = a_w25qxx_qspi_write_read(handle,
                                               W25QXX_COMMAND_PROGRAM_SECURITY_REGISTER, 1,
                                               num << 8, 1, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00, data, 256,
                                               NULL, 0x00, 1);                                                /* qspi write read */
                if (res != 0)                                                                                 /* check result */
                {
                    handle->debug_print("w25qxx: program security register failed.\n");                       /* program security register failed */
                   
                    return 1;                                                                                 /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                    /* address mode is invalid */
               
                return 4;                                                                                     /* return error */
            }
            
            timeout = 3 * 100;                                                                                /* max 3 ms */
            while (timeout != 0)                                                                              /* check timeout */
            {
                res = a_w25qxx_qspi_write_read(handle,
                                               W25QXX_COMMAND_READ_STATUS_REG1, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                              (uint8_t *)&status, 1, 1);                                      /* qspi write read */
                if (res != 0)                                                                                 /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                     /* get status1 failed */
                   
                    return 1;                                                                                 /* return error */
                }
                if ((status & 0x01) == 0x00)                                                                  /* check status */
                {
                    break;                                                                                    /* break */
                }
                timeout--;                                                                                    /* timeout-- */
                handle->delay_us(10);                                                                         /* delay 10 us */
            }
            if (timeout == 0)
            {
                handle->debug_print("w25qxx: program security register timeout.\n");                          /* program security register timeout */
               
                return 6;                                                                                     /* return error */
            }
        }
        else                                                                                                  /* single spi */
        {
            buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                             /* write enable command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                                /* spi write read */
            if (res != 0)                                                                                     /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                        /* write enable failed */
               
                return 1;                                                                                     /* return error */
            }
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                           /* 3 address mode */
            {
                handle->buf[0] = W25QXX_COMMAND_PROGRAM_SECURITY_REGISTER;                                    /* program security register command */
                handle->buf[1] = 0x00;                                                                        /* 0x00 */
                handle->buf[2] = num;                                                                         /* num */
                handle->buf[3] = 0x00;                                                                        /* 0x00 */
                memcpy(&handle->buf[4], data, 256);                                                           /* copy data */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)handle->buf, 260, NULL, 0);                  /* spi write read */
                if (res != 0)                                                                                 /* check result */
                {
                    handle->debug_print("w25qxx: program security register failed.\n");                       /* program security register failed */
                   
                    return 1;                                                                                 /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                handle->buf[0] = W25QXX_COMMAND_PROGRAM_SECURITY_REGISTER;                                    /* program security register command */
                handle->buf[1] = 0x00;                                                                        /* 0x00 */
                handle->buf[2] = 0x00;                                                                        /* 0x00 */
                handle->buf[3] = num;                                                                         /* num */
                handle->buf[4] = 0x00;                                                                        /* 0x00 */
                memcpy(&handle->buf[5], data, 256);                                                           /* copy data */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)handle->buf, 261, NULL, 0);                  /* spi write read */
                if (res != 0)                                                                                 /* check result */
                {
                    handle->debug_print("w25qxx: program security register failed.\n");                       /* program security register failed */
                   
                    return 1;                                                                                 /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                    /* address mode is invalid */
               
                return 4;                                                                                     /* return error */
            }
            
            timeout = 3 * 100;                                                                                /* max 3 ms */
            while (timeout != 0)                                                                              /* check timeout */
            {
                buf[0] = W25QXX_COMMAND_READ_STATUS_REG1;                                                     /* read status1 command */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, (uint8_t *)&status, 1);              /* spi write read */
                if (res != 0)                                                                                 /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                     /* get status1 failed */
                   
                    return 1;                                                                                 /* return error */
                }
                if ((status & 0x01) == 0x00)                                                                  /* check status */
                {
                    break;                                                                                    /* break */
                }
                timeout--;                                                                                    /* timeout-- */
                handle->delay_us(10);                                                                         /* delay 10 us */
            }
            if (timeout == 0)
            {
                handle->debug_print("w25qxx: program security register timeout.\n");                          /* program security register timeout */
               
                return 6;                                                                                     /* return error */
            }
        }
    }
    else                                                                                                      /* qspi interface */
    {
        handle->debug_print("w25qxx: qspi can't use this function.\n");                                       /* qspi can't use this function */
       
        return 5;                                                                                             /* return error */
    }
    
    return 0;                                                                                                 /* success return 0 */
}

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
uint8_t w25qxx_read_security_register(w25qxx_handle_t *handle, w25qxx_security_register_t num, uint8_t data[256])
{
    uint8_t res;
    uint8_t buf[6];
    
    if (handle == NULL)                                                                                       /* check handle */
    {
        return 2;                                                                                             /* return error */
    }   
    if (handle->inited != 1)                                                                                  /* check handle initialization */
    {
        return 3;                                                                                             /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                             /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                                /* enable dual quad spi */
        {
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                           /* 3 address mode */
            {
                res = a_w25qxx_qspi_write_read(handle,
                                               W25QXX_COMMAND_READ_SECURITY_REGISTER, 1,
                                               num << 8, 1, 3,
                                               0x00000000, 0x00, 0x00,
                                               8, NULL, 0x00,
                                               data, 256, 1);                                                 /* qspi write read */
                if (res != 0)                                                                                 /* check result */
                {
                    handle->debug_print("w25qxx: read security register failed.\n");                          /* read security register failed */
                   
                    return 1;                                                                                 /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))       /* 4 address mode */
            {
                res = a_w25qxx_qspi_write_read(handle,
                                               W25QXX_COMMAND_READ_SECURITY_REGISTER, 1,
                                               num << 8, 1, 4,
                                               0x00000000, 0x00, 0x00,
                                               8, NULL, 0x00,
                                               data, 256, 1);                                                 /* qspi write read */
                if (res != 0)                                                                                 /* check result */
                {
                    handle->debug_print("w25qxx: read security register failed.\n");                          /* read security register failed */
                   
                    return 1;                                                                                 /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                    /* address mode is invalid */
               
                return 4;                                                                                     /* return error */
            }
        }
        else                                                                                                  /* single spi */
        {
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                           /* 3 address mode */
            {
                buf[0] = W25QXX_COMMAND_READ_SECURITY_REGISTER;                                               /* read security register command */
                buf[1] = 0x00;                                                                                /* 0x00 */
                buf[2] = num;                                                                                 /* num */
                buf[3] = 0x00;                                                                                /* 0x00 */
                buf[4] = 0x00;                                                                                /* dummy */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 5, data, 256);                          /* spi write read */
                if (res != 0)                                                                                 /* check result */
                {
                    handle->debug_print("w25qxx: read security register failed.\n");                          /* read security register failed */
                   
                    return 1;                                                                                 /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                buf[0] = W25QXX_COMMAND_READ_SECURITY_REGISTER;                                               /* read security register command */
                buf[1] = 0x00;                                                                                /* 0x00 */
                buf[2] = 0x00;                                                                                /* 0x00 */
                buf[3] = num;                                                                                 /* num */
                buf[4] = 0x00;                                                                                /* 0x00 */
                buf[5] = 0x00;                                                                                /* dummy */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 6, data, 256);                          /* spi write read */
                if (res != 0)                                                                                 /* check result */
                {
                    handle->debug_print("w25qxx: read security register failed.\n");                          /* read security register failed */
                   
                    return 1;                                                                                 /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                    /* address mode is invalid */
               
                return 4;                                                                                     /* return error */
            }
        }
    }
    else                                                                                                      /* qspi interface */
    {
        handle->debug_print("w25qxx: qspi can't use this function.\n");                                       /* qspi can't use this function */
       
        return 5;                                                                                             /* return error */
    }
    
    return 0;                                                                                                 /* success return 0 */
}

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
uint8_t w25qxx_only_spi_read(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len)
{
    uint8_t res;
    uint8_t buf[5];
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                                              /* check handle initialization */
    {
        return 3;                                                                                         /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                         /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                            /* enable dual quad spi */
        {
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                       /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                              /* >128Mb */
                {
                    res = a_w25qxx_qspi_write_read(handle,
                                                   W25QXX_COMMAND_WRITE_ENABLE, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00, NULL, 0x00,
                                                   NULL, 0x00, 0x00);                                     /* qspi write read */
                    if (res != 0)                                                                         /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                            /* write enable failed */
                       
                        return 1;                                                                         /* return error */
                    }
                    buf[0] = (addr >> 24) & 0xFF;                                                         /* 31 - 24 bits */
                    res = a_w25qxx_qspi_write_read(handle,
                                                   0xC5, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00, (uint8_t *)buf, 1,
                                                   NULL, 0x00, 1);                                        /* qspi write read */
                    if (res != 0)                                                                         /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");            /* write extended addr register failed */
                       
                        return 1;                                                                         /* return error */
                    }
                    
                }
                res = a_w25qxx_qspi_write_read(handle,
                                               W25QXX_COMMAND_READ_DATA, 1,
                                               addr, 1, 3,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               data, len, 1);                                             /* qspi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: only spi read failed.\n");                               /* only spi read failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && 
                     (handle->type >= W25Q256))                                                           /* check address mode */
            {
                res = a_w25qxx_qspi_write_read(handle,
                                               W25QXX_COMMAND_READ_DATA, 1,
                                               addr, 1, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               data, len, 1);                                             /* qspi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: only spi read failed.\n");                               /* only spi read failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                /* address mode is invalid */
               
                return 4;                                                                                 /* return error */
            }
        }
        else                                                                                              /* single spi */
        {
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                       /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                              /* >128Mb */
                {
                    buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                 /* write enable command */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                    /* spi write read */
                    if (res != 0)                                                                         /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                            /* write enable failed */
                       
                        return 1;                                                                         /* return error */
                    }
                    buf[0] = 0xC5;                                                                        /* write extended addr register command */
                    buf[1] = (addr >> 24) & 0xFF;                                                         /* 31 - 24 bits */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 2, NULL, 0);                    /* spi write read */
                    if (res != 0)                                                                         /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");            /* write extended addr register failed */
                       
                        return 1;                                                                         /* return error */
                    }
                }
                buf[0] = W25QXX_COMMAND_READ_DATA;                                                        /* only spi read command */
                buf[1] = (addr >> 16) & 0xFF;                                                             /* 23 - 16 bits */
                buf[2] = (addr >> 8) & 0xFF;                                                              /* 15 - 8  bits */
                buf[3] = (addr >> 0) & 0xFF;                                                              /* 7 - 0 bits */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 4, (uint8_t *)data, (uint32_t)len); /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: only spi read failed.\n");                               /* only spi read failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && 
                     (handle->type >= W25Q256))                                                           /* check address mode */
            {
                buf[0] = W25QXX_COMMAND_READ_DATA;                                                        /* only spi read command */
                buf[1] = (addr >> 24) & 0xFF;                                                             /* 31 - 24 bits */
                buf[2] = (addr >> 16) & 0xFF;                                                             /* 23 - 16 bits */
                buf[3] = (addr >> 8) & 0xFF;                                                              /* 15 - 8  bits */
                buf[4] = (addr >> 0) & 0xFF;                                                              /* 7 - 0 bits */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 5, (uint8_t *)data, (uint32_t)len); /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: only spi read failed.\n");                               /* only spi read failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                /* address mode is invalid */
               
                return 4;                                                                                 /* return error */
            }
        }
    }
    else
    {
        handle->debug_print("w25qxx: only spi interface can use this function.\n");                       /* only spi interface can use this function */
       
        return 5;                                                                                         /* return error */
    }
    
    return 0;                                                                                             /* success return 0 */
}

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
uint8_t w25qxx_fast_read(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len)
{
    uint8_t res;
    uint8_t buf[6];
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                                              /* check handle initialization */
    {
        return 3;                                                                                         /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                         /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                            /* enable dual quad spi */
        {
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                       /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                              /* >128Mb */
                {
                    res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00, NULL, 0x00,
                                                   NULL, 0x00, 0x00);                                     /* qspi write read */
                    if (res != 0)                                                                         /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                            /* write enable failed */
                       
                        return 1;                                                                         /* return error */
                    }
                    buf[0] = (addr >> 24) & 0xFF;                                                         /* 31 - 24 bits */
                    res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0, (uint8_t *)buf, 0x01,
                                                   NULL, 0x00, 1);                                        /* spi write read */
                    if (res != 0)                                                                         /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");            /* write extended addr register failed */
                       
                        return 1;                                                                         /* return error */
                    }
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ, 1,
                                               addr, 1, 3,
                                               0x00000000, 0x00, 0x00,
                                               8, NULL, 0x00,
                                               data, len, 1);                                             /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: fast read failed.\n");                                   /* fast read failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ, 1,
                                               addr, 1, 4,
                                               0x00000000, 0x00, 0x00,
                                               8, NULL, 0x00,
                                               data, len, 1);                                             /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: fast read failed.\n");                                   /* fast read failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                /* address mode is invalid */
               
                return 4;                                                                                 /* return error */
            }
        }
        else                                                                                              /* single spi */
        {
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                       /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                              /* >128Mb */
                {
                    buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                 /* write enable command */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                    /* spi write read */
                    if (res != 0)                                                                         /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                            /* write enable failed */
                       
                        return 1;                                                                         /* return error */
                    }
                    buf[0] = 0xC5;                                                                        /* write extended addr register command */
                    buf[1] = (addr >> 24) & 0xFF;                                                         /* 31 - 24 bits */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 2, NULL, 0);                    /* spi write read */
                    if (res != 0)                                                                         /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");            /* write extended addr register failed */
                       
                        return 1;                                                                         /* return error */
                    }
                }
                buf[0] = W25QXX_COMMAND_FAST_READ;                                                        /* fast read command */
                buf[1] = (addr >> 16) & 0xFF;                                                             /* 23 - 16 bits */
                buf[2] = (addr >> 8) & 0xFF;                                                              /* 15 - 8  bits */
                buf[3] = (addr >> 0) & 0xFF;                                                              /* 7 - 0 bits */
                buf[4] = 0x00;                                                                            /* dummy */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 5, (uint8_t *)data, (uint32_t)len); /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: fast read failed.\n");                                   /* fast read failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE)
                     && (handle->type >= W25Q256))                                                        /* check address mode */
            {
                buf[0] = W25QXX_COMMAND_FAST_READ;                                                        /* fast read command */
                buf[1] = (addr >> 24) & 0xFF;                                                             /* 31 - 24 bits */
                buf[2] = (addr >> 16) & 0xFF;                                                             /* 23 - 16 bits */
                buf[3] = (addr >> 8) & 0xFF;                                                              /* 15 - 8  bits */
                buf[4] = (addr >> 0) & 0xFF;                                                              /* 7 - 0 bits */
                buf[5] = 0x00;                                                                            /* dummy */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 6, (uint8_t *)data, (uint32_t)len); /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: fast read failed.\n");                                   /* fast read failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                /* address mode is invalid */
               
                return 4;                                                                                 /* return error */
            }
        }
    }
    else                                                                                                  /* qspi interface */
    {
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                           /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                  /* >128Mb */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                         /* qspi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                /* write enable failed */
                   
                    return 1;                                                                             /* return error */
                }
                buf[0] = (addr >> 24) & 0xFF;                                                             /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 4);                                            /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                /* write extended addr register failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ, 4,
                                           addr, 4, 3,
                                           0x00000000, 0x00, 0x00,
                                           handle->dummy, NULL, 0x00,
                                           data, len, 4);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: fast read failed.\n");                                       /* fast read failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ, 4,
                                           addr, 4, 4,
                                           0x00000000, 0x00, 0x00,
                                           handle->dummy, NULL, 0x00,                                     /* spi write */
                                           data, len, 4);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: fast read failed.\n");                                       /* fast read failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                    /* address mode is invalid */
           
            return 4;                                                                                     /* return error */
        }
    }
    
    return 0;                                                                                             /* success return 0 */
}

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
uint8_t w25qxx_fast_read_dual_output(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                                              /* check handle initialization */
    {
        return 3;                                                                                         /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                         /* spi interface */
    {
        if (handle->dual_quad_spi_enable == 0)                                                            /* check spi */
        {
            handle->debug_print("w25qxx: standard spi can't use this function failed.\n");                /* standard spi can't use this function failed */
           
            return 6;                                                                                     /* return error */
        }
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                           /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                  /* >128Mb */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                         /* qspi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                /* write enable failed */
                   
                    return 1;                                                                             /* return error */
                }
                buf[0] = (addr >> 24) & 0xFF;                                                             /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 1);                                            /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                /* write extended addr register failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ_DUAL_OUTPUT, 1,
                                           addr, 1, 3,
                                           0x00000000, 0x00, 0x00,
                                           8, NULL, 0x00,                                                 /* spi write */
                                           data, len, 2);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: fast read dual output failed.\n");                           /* fast read dual output failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ_DUAL_OUTPUT, 1,
                                           addr, 1, 4,
                                           0x00000000, 0x00, 0x00,
                                           8, NULL, 0x00,                                                 /* spi write */
                                           data, len, 2);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: fast read dual output failed.\n");                           /* fast read dual output failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                    /* address mode is invalid */
           
            return 4;                                                                                     /* return error */
        }
    }
    else
    {
        handle->debug_print("w25qxx: qspi can't use this function.\n");                                   /* qspi can't use this function */
       
        return 5;                                                                                         /* return error */
    }
    
    return 0;                                                                                             /* success return 0 */
}

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
uint8_t w25qxx_fast_read_quad_output(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                                              /* check handle initialization */
    {
        return 3;                                                                                         /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                         /* spi interface */
    {
        if (handle->dual_quad_spi_enable == 0)                                                            /* check spi */
        {
            handle->debug_print("w25qxx: standard spi can't use this function failed.\n");                /* standard spi can't use this function failed */
           
            return 6;                                                                                     /* return error */
        }
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                           /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                  /* >128Mb */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                         /* qspi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                /* write enable failed */
                   
                    return 1;                                                                             /* return error */
                }
                buf[0] = (addr >> 24) & 0xFF;                                                             /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 1);                                            /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                /* write extended addr register failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ_QUAD_OUTPUT, 1,
                                           addr, 1, 3,
                                           0x00000000, 0x00, 0x00,
                                           8, NULL, 0x00,                                                 /* spi write */
                                           data, len, 4);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: fast read quad output failed.\n");                           /* fast read quad output failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ_QUAD_OUTPUT, 1,
                                           addr, 1, 4,
                                           0x00000000, 0x00, 0x00,
                                           8, NULL, 0x00,                                                 /* spi write */
                                           data, len, 4);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: fast read quad output failed.\n");                           /* fast read quad output failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                    /* address mode is invalid */
           
            return 4;                                                                                     /* return error */
        }
    }
    else
    {
        handle->debug_print("w25qxx: qspi can't use this function.\n");                                   /* qspi can't use this function */
       
        return 5;                                                                                         /* return error */
    }
    
    return 0;                                                                                             /* success return 0 */
}

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
uint8_t w25qxx_fast_read_dual_io(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                                              /* check handle initialization */
    {
        return 3;                                                                                         /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                         /* spi interface */
    {
        if (handle->dual_quad_spi_enable == 0)                                                            /* check spi */
        {
            handle->debug_print("w25qxx: standard spi can't use this function failed.\n");                /* standard spi can't use this function failed */
           
            return 6;                                                                                     /* return error */
        }
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                           /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                  /* >128Mb */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                         /* qspi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                /* write enable failed */
                   
                    return 1;                                                                             /* return error */
                }
                buf[0] = (addr >> 24) & 0xFF;                                                             /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 1);                                            /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                /* write extended addr register failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ_DUAL_IO, 1,
                                           addr, 2, 3,
                                           0x000000FF, 2, 1,
                                           0, NULL, 0x00,                                                 /* spi write */
                                           data, len, 2);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: fast read dual io failed.\n");                               /* fast read dual io failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ_DUAL_IO, 1,
                                           addr, 2, 4,
                                           0x000000FF, 2, 1,
                                           0, NULL, 0x00,                                                 /* spi write */
                                           data, len, 2);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: fast read dual io failed.\n");                               /* fast read io output failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                    /* address mode is invalid */
           
            return 4;                                                                                     /* return error */
        }
    }
    else
    {
        handle->debug_print("w25qxx: qspi can't use this function.\n");                                   /* qspi can't use this function */
       
        return 5;                                                                                         /* return error */
    }
    
    return 0;                                                                                             /* success return 0 */
}

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
uint8_t w25qxx_fast_read_quad_io(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                                              /* check handle initialization */
    {
        return 3;                                                                                         /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                         /* spi interface */
    {
        if (handle->dual_quad_spi_enable == 0)                                                            /* check spi */
        {
            handle->debug_print("w25qxx: standard spi can't use this function failed.\n");                /* standard spi can't use this function failed */
           
            return 6;                                                                                     /* return error */
        }
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                           /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                  /* >128Mb */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                         /* qspi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                /* write enable failed */
                   
                    return 1;                                                                             /* return error */
                }
                buf[0] = (addr >> 24) & 0xFF;                                                             /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 1);                                            /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                /* write extended addr register failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ_QUAD_IO, 1,
                                           addr, 4, 3,
                                           0x000000FF, 4, 1,
                                           4, NULL, 0x00,                                                 /* spi write */
                                           data, len, 4);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: fast read quad io failed.\n");                               /* fast read quad io failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ_QUAD_IO, 1,
                                           addr, 4, 4,
                                           0x000000FF, 4, 1,
                                           4, NULL, 0x00,                                                 /* spi write */
                                           data, len, 4);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: fast read quad io failed.\n");                               /* fast quad io output failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                    /* address mode is invalid */
           
            return 4;                                                                                     /* return error */
        }
    }
    else
    {
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                           /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                  /* >128Mb */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                         /* qspi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                /* write enable failed */
                   
                    return 1;                                                                             /* return error */
                }
                buf[0] = (addr >> 24) & 0xFF;                                                             /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 4);                                            /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                /* write extended addr register failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ_QUAD_IO, 4,
                                           addr, 4, 3,
                                           0x000000FF, 4, 1,
                                           handle->dummy, NULL, 0x00,
                                           data, len, 4);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: fast read quad io failed.\n");                               /* fast read quad io failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ_QUAD_IO, 4,
                                           addr, 4, 4,
                                           0x000000FF, 4, 1,
                                           handle->dummy, NULL, 0x00,
                                           data, len, 4);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: fast read quad io failed.\n");                               /* fast quad io output failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                    /* address mode is invalid */
           
            return 4;                                                                                     /* return error */
        }
    }
    
    return 0;                                                                                             /* success return 0 */
}

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
uint8_t w25qxx_word_read_quad_io(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                                              /* check handle initialization */
    {
        return 3;                                                                                         /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                         /* spi interface */
    {
        if (handle->dual_quad_spi_enable == 0)                                                            /* check spi */
        {
            handle->debug_print("w25qxx: standard spi can't use this function failed.\n");                /* standard spi can't use this function failed */
           
            return 6;                                                                                     /* return error */
        }
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                           /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                  /* >128Mb */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                         /* qspi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                /* write enable failed */
                   
                    return 1;                                                                             /* return error */
                }
                buf[0] = (addr >> 24) & 0xFF;                                                             /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 1);                                            /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                /* write extended addr register failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WORD_READ_QUAD_IO, 1,
                                           addr, 4, 3,
                                           0x000000FF, 4, 1,
                                           2, NULL, 0x00,                                                 /* spi write */
                                           data, len, 4);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: word read quad io failed.\n");                               /* word read quad io failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WORD_READ_QUAD_IO, 1,
                                           addr, 4, 4,
                                           0x000000FF, 4, 1,
                                           2, NULL, 0x00,                                                 /* spi write */
                                           data, len, 4);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: word read quad io failed.\n");                               /* word read quad io failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                    /* address mode is invalid */
           
            return 4;                                                                                     /* return error */
        }
    }
    else
    {
        handle->debug_print("w25qxx: qspi can't use this function.\n");                                   /* qspi can't use this function */
       
        return 5;                                                                                         /* return error */
    }
    
    return 0;                                                                                             /* success return 0 */
}

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
uint8_t w25qxx_octal_word_read_quad_io(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                                              /* check handle initialization */
    {
        return 3;                                                                                         /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                         /* spi interface */
    {
        if (handle->dual_quad_spi_enable == 0)                                                            /* check spi */
        {
            handle->debug_print("w25qxx: standard spi can't use this function failed.\n");                /* standard spi can't use this function failed */
           
            return 6;                                                                                     /* return error */
        }
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                           /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                  /* >128Mb */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                         /* qspi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                /* write enable failed */
                   
                    return 1;                                                                             /* return error */
                }
                buf[0] = (addr >> 24) & 0xFF;                                                             /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 1);                                            /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                /* write extended addr register failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_OCTAL_WORD_READ_QUAD_IO, 1,
                                           addr, 4, 3,
                                           0x000000FF, 4, 1,
                                           0, NULL, 0x00,                                                 /* spi write */
                                           data, len, 4);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: octal word read quad io failed.\n");                         /* octal word read quad io failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_OCTAL_WORD_READ_QUAD_IO, 1,
                                           addr, 4, 4,
                                           0x000000FF, 4, 1,
                                           0, NULL, 0x00,                                                 /* spi write */
                                           data, len, 4);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: octal word read quad io failed.\n");                         /* octal word read quad io failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                    /* address mode is invalid */
           
            return 4;                                                                                     /* return error */
        }
    }
    else
    {
        handle->debug_print("w25qxx: qspi can't use this function.\n");                                   /* qspi can't use this function */
       
        return 5;                                                                                         /* return error */
    }
    
    return 0;                                                                                             /* success return 0 */
}

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
uint8_t w25qxx_page_program(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint16_t len)
{
    uint8_t res;
    uint8_t status;
    uint32_t timeout;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                                     /* check handle */
    {
        return 2;                                                                                           /* return error */
    }   
    if (handle->inited != 1)                                                                                /* check handle initialization */
    {
        return 3;                                                                                           /* return error */
    }
    if ((addr % 256) != 0)                                                                                  /* check address */
    {
        handle->debug_print("w25qxx: addr is invalid.\n");                                                  /* addr is invalid */
       
        return 4;                                                                                           /* return error */
    }
    if (len > 256)                                                                                          /* check address */
    {
        handle->debug_print("w25qxx: length is over 256.\n");                                               /* length is over 256 */
       
        return 7;                                                                                           /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                           /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                              /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                               /* qspi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                      /* write enable failed */
               
                return 1;                                                                                   /* return error */
            }
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                         /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                                /* >128Mb */
                {
                    buf[0] = (addr >> 24) & 0xFF;                                                           /* 31 - 24 bits */
                    res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0, (uint8_t *)buf, 0x01,
                                                   NULL, 0x00, 1);                                          /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");              /* write extended addr register failed */
                       
                        return 1;                                                                           /* return error */
                    }
                    res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00, NULL, 0x00,
                                                   NULL, 0x00, 0x00);                                       /* qspi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                              /* write enable failed */
                       
                        return 1;                                                                           /* return error */
                    }
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_PAGE_PROGRAM, 1,
                                               addr, 1, 3,
                                               0x00000000, 0x00, 0x00,
                                               0, data, len,
                                               NULL, 0x00, 1);                                              /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: page program failed.\n");                                  /* page program failed */
                   
                      return 1;                                                                             /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_PAGE_PROGRAM, 1,
                                               addr, 1, 4,
                                               0x00000000, 0x00, 0x00,
                                               0, data, len,
                                               NULL, 0x00, 1);                                              /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: page program failed.\n");                                  /* page program failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                  /* address mode is invalid */
               
                return 5;                                                                                   /* return error */
            }
            
            timeout = 3 * 100;                                                                              /* max 3 ms */
            while (timeout != 0)                                                                            /* check timeout */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0,
                                              (uint8_t *)&status, 1, 1);                                    /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: page program failed.\n");                                  /* page program failed */
                   
                    return 1;                                                                               /* return error */
                }
                if ((status & 0x01) == 0x00)                                                                /* check status */
                {
                    break;                                                                                  /* break */
                }
                timeout--;                                                                                  /* timeout-- */
                handle->delay_us(10);                                                                       /* delay 10 us */
            }
            if (timeout == 0)
            {
                handle->debug_print("w25qxx: page program timeout.\n");                                     /* page program timeout */
               
                return 6;                                                                                   /* return error */
            }
        }
        else                                                                                                /* single spi */
        {
            buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                           /* write enable command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                              /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                      /* write enable failed */
               
                return 1;                                                                                   /* return error */
            }
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                         /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                                /* >128Mb */
                {
                    buf[0] = 0xC5;                                                                          /* write extended addr register command */
                    buf[1] = (addr >> 24) & 0xFF;                                                           /* 31 - 24 bits */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 2, NULL, 0);                      /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");              /* write extended addr register failed */
                       
                        return 1;                                                                           /* return error */
                    }
                    buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                   /* write enable command */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                      /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                              /* write enable failed */
                       
                        return 1;                                                                           /* return error */
                    }
                }
                handle->buf[0] = W25QXX_COMMAND_PAGE_PROGRAM;                                               /* page program command */
                handle->buf[1] = (addr >> 16) & 0xFF;                                                       /* 23 - 16 bits */
                handle->buf[2] = (addr >> 8) & 0xFF;                                                        /* 15 - 8  bits */
                handle->buf[3] = (addr >> 0) & 0xFF;                                                        /* 7 - 0 bits */
                memcpy(&handle->buf[4], data, len);                                                         /* copy data */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)handle->buf, 4 + len, NULL, 0);            /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: page program failed.\n");                                  /* page program failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE)
                     && (handle->type >= W25Q256))                                                          /* 4 address mode */
            {
                handle->buf[0] = W25QXX_COMMAND_PAGE_PROGRAM;                                               /* page program command */
                handle->buf[1] = (addr >> 24) & 0xFF;                                                       /* 31 - 24 bits */
                handle->buf[2] = (addr >> 16) & 0xFF;                                                       /* 23 - 16 bits */
                handle->buf[3] = (addr >> 8) & 0xFF;                                                        /* 15 - 8  bits */
                handle->buf[4] = (addr >> 0) & 0xFF;                                                        /* 7 - 0 bits */
                memcpy(&handle->buf[5], data, len);                                                         /* copy data */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)handle->buf, 5 + len, NULL, 0);            /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: page program failed.\n");                                  /* page program failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                  /* address mode is invalid */
               
                return 5;                                                                                   /* return error */
            }
            
            timeout = 3 * 100;                                                                              /* max 3 ms */
            while (timeout != 0)                                                                            /* check timeout */
            {
                buf[0] = W25QXX_COMMAND_READ_STATUS_REG1;                                                   /* read status1 command */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, (uint8_t *)&status, 1);            /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                   /* get status1 failed */
                   
                    return 1;                                                                               /* return error */
                }
                if ((status & 0x01) == 0x00)                                                                /* check status */
                {
                    break;                                                                                  /* break */
                }
                timeout--;                                                                                  /* timeout-- */
                handle->delay_us(10);                                                                       /* delay 10 us */
            }
            if (timeout == 0)
            {
                handle->debug_print("w25qxx: page program timeout.\n");                                     /* page program timeout */
               
                return 6;                                                                                   /* return error */
            }
        }
    }
    else
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                                   /* qspi write read */
        if (res != 0)                                                                                       /* check result */
        {
            handle->debug_print("w25qxx: write enable failed.\n");                                          /* write enable failed */
           
            return 1;                                                                                       /* return error */
        }
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                             /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                    /* >128Mb */
            {
                buf[0] = (addr >> 24) & 0xFF;                                                               /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 4);                                              /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                  /* write extended addr register failed */
                   
                    return 1;                                                                               /* return error */
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* qspi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                  /* write enable failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_PAGE_PROGRAM, 4,
                                           addr, 4, 3,
                                           0x00000000, 0x00, 0x00,
                                           0, data, len,
                                           NULL, 0x00, 4);                                                  /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: page program failed.\n");                                      /* page program failed */
               
                return 1;                                                                                   /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_PAGE_PROGRAM, 4,
                                           addr, 4, 4,
                                           0x00000000, 0x00, 0x00,
                                           0, data, len,
                                           NULL, 0x00, 4);                                                  /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: page program failed.\n");                                      /* page program failed */
               
                return 1;                                                                                   /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                      /* address mode is invalid */
           
            return 5;                                                                                       /* return error */
        }
        
        timeout = 3 * 100;                                                                                  /* max 3 ms */
        while (timeout != 0)                                                                                /* check timeout */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 4,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0,
                                          (uint8_t *)&status, 1, 4);                                        /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: page program failed.\n");                                      /* page program failed */
               
                return 1;                                                                                   /* return error */
            }
            if ((status & 0x01) == 0x00)                                                                    /* check status */
            {
                break;                                                                                      /* break */
            }
            timeout--;                                                                                      /* timeout-- */
            handle->delay_us(10);                                                                           /* delay 10 us */
        }
        if (timeout == 0)
        {
            handle->debug_print("w25qxx: page program timeout.\n");                                         /* page program timeout */
           
            return 6;                                                                                       /* return error */
        }
    }
    
    return 0;                                                                                               /* success return 0 */
}

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
uint8_t w25qxx_page_program_quad_input(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint16_t len)
{
    uint8_t res;
    uint8_t status;
    uint32_t timeout;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                                     /* check handle */
    {
        return 2;                                                                                           /* return error */
    }   
    if (handle->inited != 1)                                                                                /* check handle initialization */
    {
        return 3;                                                                                           /* return error */
    }
    if ((addr % 256) != 0)                                                                                  /* check address */
    {
        handle->debug_print("w25qxx: addr is invalid.\n");                                                  /* addr is invalid */
       
        return 4;                                                                                           /* return error */
    }
    if (len > 256)                                                                                          /* check address */
    {
        handle->debug_print("w25qxx: length is over 256.\n");                                               /* length is over 256 */
       
        return 7;                                                                                           /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_QSPI)                                                          /* qspi interface */
    {
        handle->debug_print("w25qxx: qspi can't use this function.\n");                                     /* qspi can't use this function */
       
        return 5;                                                                                           /* return error */
    }
    else
    {
        if (handle->dual_quad_spi_enable == 0)                                                              /* check spi */
        {
            handle->debug_print("w25qxx: standard spi can't use this function failed.\n");                  /* standard spi can't use this function failed */
           
            return 8;                                                                                       /* return error */
        }
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                                   /* qspi write read */
        if (res != 0)                                                                                       /* check result */
        {
            handle->debug_print("w25qxx: write enable failed.\n");                                          /* write enable failed */
           
            return 1;                                                                                       /* return error */
        }
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                             /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                    /* >128Mb */
            {
                buf[0] = (addr >> 24) & 0xFF;                                                               /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 1);                                              /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                  /* write extended addr register failed */
                   
                    return 1;                                                                               /* return error */
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* qspi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                  /* write enable failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_QUAD_PAGE_PROGRAM, 1,
                                           addr, 1, 3,
                                           0x00000000, 0x00, 0x00,
                                           0, data, len,
                                           NULL, 0x00, 4);                                                  /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: quad page program failed.\n");                                 /* quad page program failed */
               
                  return 1;                                                                                 /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_QUAD_PAGE_PROGRAM, 1,
                                           addr, 1, 4,
                                           0x00000000, 0x00, 0x00,
                                           0, data, len,
                                           NULL, 0x00, 4);                                                  /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: quad page program failed.\n");                                 /* quad page program failed */
               
                return 1;                                                                                   /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                      /* address mode is invalid */
           
            return 5;                                                                                       /* return error */
        }
        
        timeout = 3 * 100;                                                                                  /* max 3 ms */
        while (timeout != 0)                                                                                /* check timeout */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0,
                                          (uint8_t *)&status, 1, 1);                                        /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: get status1 failed.\n");                                       /* get status1 failed */
               
                return 1;                                                                                   /* return error */
            }
            if ((status & 0x01) == 0x00)                                                                    /* check status */
            {
                break;                                                                                      /* break */
            }
            timeout--;                                                                                      /* timeout-- */
            handle->delay_us(10);                                                                           /* delay 10 us */
        }
        if (timeout == 0)
        {
            handle->debug_print("w25qxx: quad page program timeout.\n");                                    /* quad page program timeout */
           
            return 6;                                                                                       /* return error */
        }
    }
    
    return 0;                                                                                               /* success return 0 */
}

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
uint8_t w25qxx_sector_erase_4k(w25qxx_handle_t *handle, uint32_t addr)
{
    uint8_t res;
    uint8_t status;
    uint32_t timeout;
    uint8_t buf[5];
    
    if (handle == NULL)                                                                                     /* check handle */
    {
        return 2;                                                                                           /* return error */
    }   
    if (handle->inited != 1)                                                                                /* check handle initialization */
    {
        return 3;                                                                                           /* return error */
    }
    if ((addr % 4096) != 0)                                                                                 /* check address */
    {
        handle->debug_print("w25qxx: addr is invalid.\n");                                                  /* addr is invalid */
       
        return 4;                                                                                           /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                           /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                              /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                               /* qspi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                      /* write enable failed */
               
                return 1;                                                                                   /* return error */
            }
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                         /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                                /* >128Mb */
                {
                    buf[0] = (addr >> 24) & 0xFF;                                                           /* 31 - 24 bits */
                    res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0, (uint8_t *)buf, 0x01,
                                                   NULL, 0x00, 1);                                          /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");              /* write extended addr register failed */
                       
                        return 1;                                                                           /* return error */
                    }
                    res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00, NULL, 0x00,
                                                   NULL, 0x00, 0x00);                                       /* qspi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                              /* write enable failed */
                       
                        return 1;                                                                           /* return error */
                    }
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_SECTOR_ERASE_4K, 1,
                                               addr, 1, 3,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: sector erase 4k failed.\n");                               /* sector erase 4k failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_SECTOR_ERASE_4K, 1,
                                               addr, 1, 4,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: sector erase 4k failed.\n");                               /* sector erase 4k failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                  /* address mode is invalid */
               
                return 5;                                                                                   /* return error */
            }
            
            timeout = 400;                                                                                  /* max 400 ms */
            while (timeout != 0)                                                                            /* check timeout */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0,
                                              (uint8_t *)&status, 1, 1);                                    /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                   /* get status1 failed */
                   
                    return 1;                                                                               /* return error */
                }
                if ((status & 0x01) == 0x00)                                                                /* check status */
                {
                    break;                                                                                  /* break */
                }
                timeout--;                                                                                  /* timeout-- */
                handle->delay_ms(1);                                                                        /* delay 1 ms */
            }
            if (timeout == 0)
            {
                handle->debug_print("w25qxx: sector erase 4k timeout.\n");                                  /* sector erase 4k timeout */
               
                return 6;                                                                                   /* return error */
            }
        }
        else                                                                                                /* single spi */
        {
            buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                           /* write enable command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                              /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                      /* write enable failed */
               
                return 1;                                                                                   /* return error */
            }
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                         /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                                /* >128Mb */
                {
                    buf[0] = 0xC5;                                                                          /* write extended addr register command */
                    buf[1] = (addr >> 24) & 0xFF;                                                           /* 31 - 24 bits */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 2, NULL, 0);                      /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");              /* write extended addr register failed */
                       
                        return 1;                                                                           /* return error */
                    }
                    buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                   /* write enable command */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                      /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                              /* write enable failed */
                       
                        return 1;                                                                           /* return error */
                    }
                }
                buf[0] = W25QXX_COMMAND_SECTOR_ERASE_4K;                                                    /* sector erase 4k command */
                buf[1] = (addr >> 16) & 0xFF;                                                               /* 23 - 16 bits */
                buf[2] = (addr >> 8) & 0xFF;                                                                /* 15 - 8  bits */
                buf[3] = (addr >> 0) & 0xFF;                                                                /* 7 - 0 bits */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 4, NULL, 0);                          /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: sector erase 4k failed.\n");                               /* sector erase 4k failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE)
                     && (handle->type >= W25Q256))
            {
                buf[0] = W25QXX_COMMAND_SECTOR_ERASE_4K;                                                    /* sector erase 4k command */
                buf[1] = (addr >> 24) & 0xFF;                                                               /* 31 - 24 bits */
                buf[2] = (addr >> 16) & 0xFF;                                                               /* 23 - 16 bits */
                buf[3] = (addr >> 8) & 0xFF;                                                                /* 15 - 8  bits */
                buf[4] = (addr >> 0) & 0xFF;                                                                /* 7 - 0 bits */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 5, NULL, 0);                          /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: sector erase 4k failed.\n");                               /* sector erase 4k failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                  /* address mode is invalid */
               
                return 5;                                                                                   /* return error */
            }
            
            timeout = 400;                                                                                  /* max 400 ms */
            while (timeout != 0)                                                                            /* check timeout */
            {
                buf[0] = W25QXX_COMMAND_READ_STATUS_REG1;                                                   /* read status1 command */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, (uint8_t *)&status, 1);            /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                   /* get status1 failed */
                   
                    return 1;                                                                               /* return error */
                }
                if ((status & 0x01) == 0x00)                                                                /* check status */
                {
                    break;                                                                                  /* break */
                }
                timeout--;                                                                                  /* timeout-- */
                handle->delay_ms(1);                                                                        /* delay 1 ms */
            }
            if (timeout == 0)
            {
                handle->debug_print("w25qxx: sector erase 4k timeout.\n");                                  /* sector erase 4k timeout */
               
                return 6;                                                                                   /* return error */
            }
        }
    }
    else
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                                   /* qspi write read */
        if (res != 0)                                                                                       /* check result */
        {
            handle->debug_print("w25qxx: write enable failed.\n");                                          /* write enable failed */
           
            return 1;                                                                                       /* return error */
        }
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                             /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                    /* >128Mb */
            {
                buf[0] = (addr >> 24) & 0xFF;                                                               /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 4);                                              /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                  /* write extended addr register failed */
                   
                    return 1;                                                                               /* return error */
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* qspi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                  /* write enable failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_SECTOR_ERASE_4K, 4,
                                           addr, 4, 3,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                               /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: sector erase 4k failed.\n");                                   /* sector erase 4k failed */
               
                return 1;                                                                                   /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_SECTOR_ERASE_4K, 4,
                                           addr, 4, 4,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                               /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: sector erase 4k failed.\n");                                   /* sector erase 4k failed */
               
                return 1;                                                                                   /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                      /* address mode is invalid */
           
            return 5;                                                                                       /* return error */
        }
        
        timeout = 400;                                                                                      /* max 400 ms */
        while (timeout != 0)                                                                                /* check timeout */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 4,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0,
                                          (uint8_t *)&status, 1, 4);                                        /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: get status1 failed.\n");                                       /* get status1 failed */
               
                return 1;                                                                                   /* return error */
            }
            if ((status & 0x01) == 0x00)                                                                    /* check status */
            {
                break;                                                                                      /* break */
            }
            timeout--;                                                                                      /* timeout-- */
            handle->delay_ms(1);                                                                            /* delay 1 ms */
        }
        if (timeout == 0)
        {
            handle->debug_print("w25qxx: sector erase 4k timeout.\n");                                      /* sector erase 4k timeout */
           
            return 6;                                                                                       /* return error */
        }
    }
    
    return 0;                                                                                               /* success return 0 */
}

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
uint8_t w25qxx_block_erase_32k(w25qxx_handle_t *handle, uint32_t addr)
{
    uint8_t res;
    uint8_t status;
    uint32_t timeout;
    uint8_t buf[5];
    
    if (handle == NULL)                                                                                     /* check handle */
    {
        return 2;                                                                                           /* return error */
    }   
    if (handle->inited != 1)                                                                                /* check handle initialization */
    {
        return 3;                                                                                           /* return error */
    }
    if ((addr % (32 * 1024)) != 0)                                                                          /* check address */
    {
        handle->debug_print("w25qxx: addr is invalid.\n");                                                  /* addr is invalid */
       
        return 4;                                                                                           /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                           /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                              /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                               /* qspi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                      /* write enable failed */
               
                return 1;                                                                                   /* return error */
            }
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                         /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                                /* >128Mb */
                {
                    buf[0] = (addr >> 24) & 0xFF;                                                           /* 31 - 24 bits */
                    res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0, (uint8_t *)buf, 0x01,
                                                   NULL, 0x00, 1);                                          /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");              /* write extended addr register failed */
                       
                        return 1;                                                                           /* return error */
                    }
                    res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00, NULL, 0x00,
                                                   NULL, 0x00, 0x00);                                       /* qspi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                              /* write enable failed */
                       
                        return 1;                                                                           /* return error */
                    }
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_BLOCK_ERASE_32K, 1,
                                               addr, 1, 3,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: block erase 32k failed.\n");                               /* block erase 32k failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_BLOCK_ERASE_32K, 1,
                                               addr, 1, 4,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: block erase 32k failed.\n");                               /* block erase 32k failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                  /* address mode is invalid */
               
                return 5;                                                                                   /* return error */
            }
            
            timeout = 1600;                                                                                 /* max 1600 ms */
            while (timeout != 0)                                                                            /* check timeout */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0,
                                              (uint8_t *)&status, 1, 1);                                    /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                   /* get status1 failed */
                   
                    return 1;                                                                               /* return error */
                }
                if ((status & 0x01) == 0x00)                                                                /* check status */
                {
                    break;                                                                                  /* break */
                }
                timeout--;                                                                                  /* timeout-- */
                handle->delay_ms(1);                                                                        /* delay 1 ms */
            }
            if (timeout == 0)
            {
                handle->debug_print("w25qxx: block erase 32k timeout.\n");                                  /* block erase 32k timeout */
               
                return 6;                                                                                   /* return error */
            }
        }
        else                                                                                                /* single spi */
        {
            buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                           /* write enable command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                              /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                      /* write enable failed */
               
                return 1;                                                                                   /* return error */
            }
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                         /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                                /* >128Mb */
                {
                    buf[0] = 0xC5;                                                                          /* write extended addr register command */
                    buf[1] = (addr >> 24) & 0xFF;                                                           /* 31 - 24 bits */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 2, NULL, 0);                      /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");              /* write extended addr register failed */
                       
                        return 1;                                                                           /* return error */
                    }
                    buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                   /* write enable command */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                      /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                              /* write enable failed */
                       
                        return 1;                                                                           /* return error */
                    }
                }
                buf[0] = W25QXX_COMMAND_BLOCK_ERASE_32K;                                                    /* block erase 32k command */
                buf[1] = (addr >> 16) & 0xFF;                                                               /* 23 - 16 bits */
                buf[2] = (addr >> 8) & 0xFF;                                                                /* 15 - 8  bits */
                buf[3] = (addr >> 0) & 0xFF;                                                                /* 7 - 0 bits */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 4, NULL, 0);                          /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: block erase 32k failed.\n");                               /* block erase 32k failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE)
                     && (handle->type >= W25Q256))
            {
                buf[0] = W25QXX_COMMAND_BLOCK_ERASE_32K;                                                    /* block erase 32k command */
                buf[1] = (addr >> 24) & 0xFF;                                                               /* 31 - 24 bits */
                buf[2] = (addr >> 16) & 0xFF;                                                               /* 23 - 16 bits */
                buf[3] = (addr >> 8) & 0xFF;                                                                /* 15 - 8  bits */
                buf[4] = (addr >> 0) & 0xFF;                                                                /* 7 - 0 bits */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 5, NULL, 0);                          /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: block erase 32k failed.\n");                               /* block erase 32k failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                  /* address mode is invalid */
               
                return 5;                                                                                   /* return error */
            }
            
            timeout = 1600;                                                                                 /* max 1600 ms */
            while (timeout != 0)                                                                            /* check timeout */
            {
                buf[0] = W25QXX_COMMAND_READ_STATUS_REG1;                                                   /* read status1 command */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, (uint8_t *)&status, 1);            /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                   /* get status1 failed */
                   
                    return 1;                                                                               /* return error */
                }
                if ((status & 0x01) == 0x00)                                                                /* check status */
                {
                    break;                                                                                  /* break */
                }
                timeout--;                                                                                  /* timeout-- */
                handle->delay_ms(1);                                                                        /* delay 1 ms */
            }
            if (timeout == 0)
            {
                handle->debug_print("w25qxx: block erase 32k timeout.\n");                                  /* block erase 32k timeout */
               
                return 6;                                                                                   /* return error */
            }
        }
    }
    else
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                                   /* qspi write read */
        if (res != 0)                                                                                       /* check result */
        {
            handle->debug_print("w25qxx: write enable failed.\n");                                          /* write enable failed */
           
            return 1;                                                                                       /* return error */
        }
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                             /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                    /* >128Mb */
            {
                buf[0] = (addr >> 24) & 0xFF;                                                               /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 4);                                              /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                  /* write extended addr register failed */
                   
                    return 1;                                                                               /* return error */
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* qspi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                  /* write enable failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_BLOCK_ERASE_32K, 4,
                                           addr, 4, 3,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                               /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: block erase 32k failed.\n");                                   /* block erase 32k failed */
               
                return 1;                                                                                   /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_BLOCK_ERASE_32K, 4,
                                           addr, 4, 4,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                               /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: block erase 32k failed.\n");                                   /* block erase 32k failed */
               
                return 1;                                                                                   /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                      /* address mode is invalid */
           
            return 5;                                                                                       /* return error */
        }
        
        timeout = 1600;                                                                                     /* max 1600 ms */
        while (timeout != 0)                                                                                /* check timeout */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 4,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0,
                                          (uint8_t *)&status, 1, 4);                                        /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: get status1 failed.\n");                                       /* get status1 failed */
               
                return 1;                                                                                   /* return error */
            }
            if ((status & 0x01) == 0x00)                                                                    /* check status */
            {
                break;                                                                                      /* break */
            }
            timeout--;                                                                                      /* timeout-- */
            handle->delay_ms(1);                                                                            /* delay 1 ms */
        }
        if (timeout == 0)
        {
            handle->debug_print("w25qxx: block erase 32k timeout.\n");                                      /* block erase 32k timeout */
           
            return 6;                                                                                       /* return error */
        }
    }
    
    return 0;                                                                                               /* success return 0 */
}

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
uint8_t w25qxx_block_erase_64k(w25qxx_handle_t *handle, uint32_t addr)
{
    uint8_t res;
    uint8_t status;
    uint32_t timeout;
    uint8_t buf[5];
    
    if (handle == NULL)                                                                                     /* check handle */
    {
        return 2;                                                                                           /* return error */
    }   
    if (handle->inited != 1)                                                                                /* check handle initialization */
    {
        return 3;                                                                                           /* return error */
    }
    if ((addr % (64 * 1024)) != 0)                                                                          /* check address */
    {
        handle->debug_print("w25qxx: addr is invalid.\n");                                                  /* addr is invalid */
       
        return 4;                                                                                           /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                           /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                              /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                               /* qspi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                      /* write enable failed */
               
                return 1;                                                                                   /* return error */
            }
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                         /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                                /* >128Mb */
                {
                    buf[0] = (addr >> 24) & 0xFF;                                                           /* 31 - 24 bits */
                    res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0, (uint8_t *)buf, 0x01,
                                                   NULL, 0x00, 1);                                          /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");              /* write extended addr register failed */
                       
                        return 1;                                                                           /* return error */
                    }
                    res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00, NULL, 0x00,
                                                   NULL, 0x00, 0x00);                                       /* qspi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                              /* write enable failed */
                       
                        return 1;                                                                           /* return error */
                    }
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_BLOCK_ERASE_64K, 1,
                                               addr, 1, 3,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: block erase 64k failed.\n");                               /* block erase 64k failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_BLOCK_ERASE_64K, 1,
                                               addr, 1, 4,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: block erase 64k failed.\n");                               /* block erase 64k failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                  /* address mode is invalid */
               
                return 5;                                                                                   /* return error */
            }
            
            timeout = 2000;                                                                                 /* max 2000 ms */
            while (timeout != 0)                                                                            /* check timeout */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0,
                                              (uint8_t *)&status, 1, 1);                                    /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                   /* get status1 failed */
                   
                    return 1;                                                                               /* return error */
                }
                if ((status & 0x01) == 0x00)                                                                /* check status */
                {
                    break;                                                                                  /* break */
                }
                timeout--;                                                                                  /* timeout-- */
                handle->delay_ms(1);                                                                        /* delay 1 ms */
            }
            if (timeout == 0)
            {
                handle->debug_print("w25qxx: block erase 64k timeout.\n");                                  /* block erase 64k timeout */
               
                return 6;                                                                                   /* return error */
            }
        }
        else                                                                                                /* single spi */
        {
            buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                           /* write enable command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                              /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                      /* write enable failed */
               
                return 1;                                                                                   /* return error */
            }
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                         /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                                /* >128Mb */
                {
                    buf[0] = 0xC5;                                                                          /* write extended addr register command */
                    buf[1] = (addr >> 24) & 0xFF;                                                           /* 31 - 24 bits */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 2, NULL, 0);                      /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");              /* write extended addr register failed */
                       
                        return 1;                                                                           /* return error */
                    }
                    buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                   /* write enable command */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                      /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                              /* write enable failed */
                       
                        return 1;                                                                           /* return error */
                    }
                }
                buf[0] = W25QXX_COMMAND_BLOCK_ERASE_64K;                                                    /* block erase 64k command */
                buf[1] = (addr >> 16) & 0xFF;                                                               /* 23 - 16 bits */
                buf[2] = (addr >> 8) & 0xFF;                                                                /* 15 - 8  bits */
                buf[3] = (addr >> 0) & 0xFF;                                                                /* 7 - 0 bits */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 4, NULL, 0);                          /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: block erase 64k failed.\n");                               /* block erase 64k failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                buf[0] = W25QXX_COMMAND_BLOCK_ERASE_64K;                                                    /* block erase 64k command */
                buf[1] = (addr >> 24) & 0xFF;                                                               /* 31 - 24 bits */
                buf[2] = (addr >> 16) & 0xFF;                                                               /* 23 - 16 bits */
                buf[3] = (addr >> 8) & 0xFF;                                                                /* 15 - 8  bits */
                buf[4] = (addr >> 0) & 0xFF;                                                                /* 7 - 0 bits */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 5, NULL, 0);                          /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: block erase 64k failed.\n");                               /* block erase 64k failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                  /* address mode is invalid */
               
                return 5;                                                                                   /* return error */
            }
            
            timeout = 2000;                                                                                 /* max 2000 ms */
            while (timeout != 0)                                                                            /* check timeout */
            {
                buf[0] = W25QXX_COMMAND_READ_STATUS_REG1;                                                   /* read status1 command */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, (uint8_t *)&status, 1);            /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                   /* get status1 failed */
                   
                    return 1;                                                                               /* return error */
                }
                if ((status & 0x01) == 0x00)                                                                /* check status */
                {
                    break;                                                                                  /* break */
                }
                timeout--;                                                                                  /* timeout-- */
                handle->delay_ms(1);                                                                        /* delay 1 ms */
            }
            if (timeout == 0)
            {
                handle->debug_print("w25qxx: block erase 64k timeout.\n");                                  /* block erase 64k timeout */
               
                return 6;                                                                                   /* return error */
            }
        }
    }
    else
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                                   /* qspi write read */
        if (res != 0)                                                                                       /* check result */
        {
            handle->debug_print("w25qxx: write enable failed.\n");                                          /* write enable failed */
           
            return 1;                                                                                       /* return error */
        }
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                             /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                    /* >128Mb */
            {
                buf[0] = (addr >> 24) & 0xFF;                                                               /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 4);                                              /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                  /* write extended addr register failed */
                   
                    return 1;                                                                               /* return error */
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* qspi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                  /* write enable failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_BLOCK_ERASE_64K, 4,
                                           addr, 4, 3,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                               /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: block erase 64k failed.\n");                                   /* block erase 64k failed */
               
                return 1;                                                                                   /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_BLOCK_ERASE_64K, 4,
                                           addr, 4, 4,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                               /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: block erase 64k failed.\n");                                   /* block erase 64k failed */
               
                return 1;                                                                                   /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                      /* address mode is invalid */
           
            return 5;                                                                                       /* return error */
        }
        
        timeout = 2000;                                                                                     /* max 2000 ms */
        while (timeout != 0)                                                                                /* check timeout */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 4,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0,
                                          (uint8_t *)&status, 1, 4);                                        /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: get status1 failed.\n");                                       /* get status1 failed */
               
                return 1;                                                                                   /* return error */
            }
            if ((status & 0x01) == 0x00)                                                                    /* check status */
            {
                break;                                                                                      /* break */
            }
            timeout--;                                                                                      /* timeout-- */
            handle->delay_ms(1);                                                                            /* delay 1 ms */
        }
        if (timeout == 0)
        {
            handle->debug_print("w25qxx: block erase 64k timeout.\n");                                      /* block erase 64k timeout */
           
            return 6;                                                                                       /* return error */
        }
    }
    
    return 0;                                                                                               /* success return 0 */
}

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
uint8_t w25qxx_individual_block_lock(w25qxx_handle_t *handle, uint32_t addr)
{
    uint8_t res;
    uint8_t buf[5];
    
    if (handle == NULL)                                                                                     /* check handle */
    {
        return 2;                                                                                           /* return error */
    }   
    if (handle->inited != 1)                                                                                /* check handle initialization */
    {
        return 3;                                                                                           /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                           /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                              /* enable dual quad spi */
        {
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                         /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                                /* >128Mb */
                {
                    res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00, NULL, 0x00,
                                                   NULL, 0x00, 0x00);                                       /* qspi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                              /* write enable failed */
                       
                        return 1;                                                                           /* return error */
                    }
                    buf[0] = (addr >> 24) & 0xFF;                                                           /* 31 - 24 bits */
                    res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0, (uint8_t *)buf, 0x01,
                                                   NULL, 0x00, 1);                                          /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");              /* write extended addr register failed */
                       
                        return 1;                                                                           /* return error */
                    }
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_INDIVIDUAL_BLOCK_LOCK, 1,
                                               addr, 1, 3,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: individual block lock failed.\n");                         /* individual block lock failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_INDIVIDUAL_BLOCK_LOCK, 1,
                                               addr, 1, 4,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: individual block lock failed.\n");                         /* individual block lock failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                  /* address mode is invalid */
               
                return 4;                                                                                   /* return error */
            }
        }
        else                                                                                                /* single spi */
        {
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                         /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                                /* >128Mb */
                {
                    buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                   /* write enable command */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                      /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                              /* write enable failed */
                       
                        return 1;                                                                           /* return error */
                    }
                    buf[0] = 0xC5;                                                                          /* write extended addr register command */
                    buf[1] = (addr >> 24) & 0xFF;                                                           /* 31 - 24 bits */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 2, NULL, 0);                      /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");              /* write extended addr register failed */
                       
                        return 1;                                                                           /* return error */
                    }
                }
                buf[0] = W25QXX_COMMAND_INDIVIDUAL_BLOCK_LOCK;                                              /* individual block lock command */
                buf[1] = (addr >> 16) & 0xFF;                                                               /* 23 - 16 bits */
                buf[2] = (addr >> 8) & 0xFF;                                                                /* 15 - 8  bits */
                buf[3] = (addr >> 0) & 0xFF;                                                                /* 7 - 0 bits */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 4, NULL, 0);                          /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: individual block lock failed.\n");                         /* individual block lock failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                buf[0] = W25QXX_COMMAND_INDIVIDUAL_BLOCK_LOCK;                                              /* individual block lock command */
                buf[1] = (addr >> 24) & 0xFF;                                                               /* 31 - 24 bits */
                buf[2] = (addr >> 16) & 0xFF;                                                               /* 23 - 16 bits */
                buf[3] = (addr >> 8) & 0xFF;                                                                /* 15 - 8  bits */
                buf[4] = (addr >> 0) & 0xFF;                                                                /* 7 - 0 bits */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 5, NULL, 0);                          /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: individual block lock failed.\n");                         /* individual block lock failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                  /* address mode is invalid */
               
                return 4;                                                                                   /* return error */
            }
        }
    }
    else
    {
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                             /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                    /* >128Mb */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* qspi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                  /* write enable failed */
                   
                    return 1;                                                                               /* return error */
                }
                buf[0] = (addr >> 24) & 0xFF;                                                               /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 4);                                              /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                  /* write extended addr register failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_INDIVIDUAL_BLOCK_LOCK, 4,
                                           addr, 4, 3,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                               /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: individual block lock failed.\n");                             /* individual block lock failed */
               
                return 1;                                                                                   /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_INDIVIDUAL_BLOCK_LOCK, 4,
                                           addr, 4, 4,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                               /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: individual block lock failed.\n");                             /* individual block lock failed */
               
                return 1;                                                                                   /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                      /* address mode is invalid */
           
            return 4;                                                                                       /* return error */
        }
    }
    
    return 0;                                                                                               /* success return 0 */
}

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
uint8_t w25qxx_individual_block_unlock(w25qxx_handle_t *handle, uint32_t addr)
{
    uint8_t res;
    uint8_t buf[5];
    
    if (handle == NULL)                                                                                     /* check handle */
    {
        return 2;                                                                                           /* return error */
    }   
    if (handle->inited != 1)                                                                                /* check handle initialization */
    {
        return 3;                                                                                           /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                           /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                              /* enable dual quad spi */
        {
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                         /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                                /* >128Mb */
                {
                    res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00, NULL, 0x00,
                                                   NULL, 0x00, 0x00);                                       /* qspi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                              /* write enable failed */
                       
                        return 1;                                                                           /* return error */
                    }
                    buf[0] = (addr >> 24) & 0xFF;                                                           /* 31 - 24 bits */
                    res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0, (uint8_t *)buf, 0x01,
                                                   NULL, 0x00, 1);                                          /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");              /* write extended addr register failed */
                       
                        return 1;                                                                           /* return error */
                    }
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_INDIVIDUAL_BLOCK_UNLOCK, 1,
                                               addr, 1, 3,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: individual block unlock failed.\n");                       /* individual unblock lock failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_INDIVIDUAL_BLOCK_UNLOCK, 1,
                                               addr, 1, 4,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: individual block unlock failed.\n");                       /* individual block unlock failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                  /* address mode is invalid */
               
                return 4;                                                                                   /* return error */
            }
        }
        else                                                                                                /* single spi */
        {
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                         /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                                /* >128Mb */
                {
                    buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                   /* write enable command */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                      /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                              /* write enable failed */
                       
                        return 1;                                                                           /* return error */
                    }
                    buf[0] = 0xC5;                                                                          /* write extended addr register command */
                    buf[1] = (addr >> 24) & 0xFF;                                                           /* 31 - 24 bits */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 2, NULL, 0);                      /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");              /* write extended addr register failed */
                       
                        return 1;                                                                           /* return error */
                    }
                }
                buf[0] = W25QXX_COMMAND_INDIVIDUAL_BLOCK_UNLOCK;                                            /* individual block unlock command */
                buf[1] = (addr >> 16) & 0xFF;                                                               /* 23 - 16 bits */
                buf[2] = (addr >> 8) & 0xFF;                                                                /* 15 - 8  bits */
                buf[3] = (addr >> 0) & 0xFF;                                                                /* 7 - 0 bits */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 4, NULL, 0);                          /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: individual block unlock failed.\n");                       /* individual block unlock failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                buf[0] = W25QXX_COMMAND_INDIVIDUAL_BLOCK_UNLOCK;                                            /* individual block unlock command */
                buf[1] = (addr >> 24) & 0xFF;                                                               /* 31 - 24 bits */
                buf[2] = (addr >> 16) & 0xFF;                                                               /* 23 - 16 bits */
                buf[3] = (addr >> 8) & 0xFF;                                                                /* 15 - 8  bits */
                buf[4] = (addr >> 0) & 0xFF;                                                                /* 7 - 0 bits */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 5, NULL, 0);                          /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: individual block unlock failed.\n");                       /* individual block unlock failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                  /* address mode is invalid */
               
                return 4;                                                                                   /* return error */
            }
        }
    }
    else
    {
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                             /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                    /* >128Mb */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* qspi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                  /* write enable failed */
                   
                    return 1;                                                                               /* return error */
                }
                buf[0] = (addr >> 24) & 0xFF;                                                               /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 4);                                              /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                  /* write extended addr register failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_INDIVIDUAL_BLOCK_UNLOCK, 4,
                                           addr, 4, 3,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                               /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: individual block unlock failed.\n");                           /* individual unblock lock failed */
               
                return 1;                                                                                   /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_INDIVIDUAL_BLOCK_UNLOCK, 4,
                                           addr, 4, 4,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                               /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: individual block unlock failed.\n");                           /* individual block unlock failed */
               
                return 1;                                                                                   /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                      /* address mode is invalid */
           
            return 4;                                                                                       /* return error */
        }
    }
    
    return 0;                                                                                               /* success return 0 */
}

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
uint8_t w25qxx_read_block_lock(w25qxx_handle_t *handle, uint32_t addr, uint8_t *value)
{
    uint8_t res;
    uint8_t buf[5];
    
    if (handle == NULL)                                                                                     /* check handle */
    {
        return 2;                                                                                           /* return error */
    }   
    if (handle->inited != 1)                                                                                /* check handle initialization */
    {
        return 3;                                                                                           /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                           /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                              /* enable dual quad spi */
        {
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                         /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                                /* >128Mb */
                {
                    res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00, NULL, 0x00,
                                                   NULL, 0x00, 0x00);                                       /* qspi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                              /* write enable failed */
                       
                        return 1;                                                                           /* return error */
                    }
                    buf[0] = (addr >> 24) & 0xFF;                                                           /* 31 - 24 bits */
                    res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0, (uint8_t *)buf, 0x01,
                                                   NULL, 0x00, 1);                                          /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");              /* write extended addr register failed */
                       
                        return 1;                                                                           /* return error */
                    }
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_BLOCK_LOCK, 1,
                                               addr, 1, 3,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0x00,
                                               value, 1, 1);                                                /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: read block lock failed.\n");                               /* read block lock failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_BLOCK_LOCK, 1,
                                               addr, 1, 4,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0x00,
                                               value, 1, 1);                                                /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: read block lock failed.\n");                               /* read block lock failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                  /* address mode is invalid */
               
                return 4;                                                                                   /* return error */
            }
        }
        else                                                                                                /* single spi */
        {
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                         /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                                /* >128Mb */
                {
                    buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                   /* write enable command */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                      /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                              /* write enable failed */
                       
                        return 1;                                                                           /* return error */
                    }
                    buf[0] = 0xC5;                                                                          /* write extended addr register command */
                    buf[1] = (addr >> 24) & 0xFF;                                                           /* 31 - 24 bits */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 2, NULL, 0);                      /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");              /* write extended addr register failed */
                       
                        return 1;                                                                           /* return error */
                    }
                }
                buf[0] = W25QXX_COMMAND_READ_BLOCK_LOCK;                                                    /* read block lock command */
                buf[1] = (addr >> 16) & 0xFF;                                                               /* 23 - 16 bits */
                buf[2] = (addr >> 8) & 0xFF;                                                                /* 15 - 8  bits */
                buf[3] = (addr >> 0) & 0xFF;                                                                /* 7 - 0 bits */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 4, value, 1);                         /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: read block lock failed.\n");                               /* read block lock failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                buf[0] = W25QXX_COMMAND_READ_BLOCK_LOCK;                                                    /* read block lock command */
                buf[1] = (addr >> 24) & 0xFF;                                                               /* 31 - 24 bits */
                buf[2] = (addr >> 16) & 0xFF;                                                               /* 23 - 16 bits */
                buf[3] = (addr >> 8) & 0xFF;                                                                /* 15 - 8  bits */
                buf[4] = (addr >> 0) & 0xFF;                                                                /* 7 - 0 bits */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 5, value, 1);                         /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: read block lock failed.\n");                               /* read block lock failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                  /* address mode is invalid */
               
                return 4;                                                                                   /* return error */
            }
        }
    }
    else
    {
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                             /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                    /* >128Mb */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* qspi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                  /* write enable failed */
                   
                    return 1;                                                                               /* return error */
                }
                buf[0] = (addr >> 24) & 0xFF;                                                               /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 4);                                              /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                  /* write extended addr register failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_BLOCK_LOCK, 4,
                                           addr, 4, 3,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           value, 1, 4);                                                    /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: read block lock failed.\n");                                   /* read block lock failed */
               
                return 1;                                                                                   /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_BLOCK_LOCK, 4,
                                           addr, 4, 4,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           value, 1, 4);                                                    /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: read block lock failed.\n");                                   /* read block lock failed */
               
                return 1;                                                                                   /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                      /* address mode is invalid */
           
            return 4;                                                                                       /* return error */
        }
    }
    
    return 0;                                                                                               /* success return 0 */
}

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
uint8_t w25qxx_set_burst_with_wrap(w25qxx_handle_t *handle, w25qxx_burst_wrap_t wrap)
{
    uint8_t res;
    uint8_t buf[5];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }   
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                    /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                       /* enable dual quad spi */
        {
            buf[0] = wrap;
            res = a_w25qxx_qspi_write_read(handle,
                                           W25QXX_COMMAND_SET_BURST_WITH_WRAP, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           3 * 8, (uint8_t *)buf, 1,
                                           NULL, 0x00, 1);                           /* spi write read */
            if (res != 0)                                                            /* check result */
            {
                handle->debug_print("w25qxx: set burst with wrap failed.\n");        /* set burst with wrap failed */
               
                return 1;                                                            /* return error */
            }
        }
        else                                                                         /* single spi */
        {
            buf[0] = W25QXX_COMMAND_SET_BURST_WITH_WRAP;                             /* set burst with wrap command */
            buf[1] = 0x00;                                                           /* dummy */
            buf[2] = 0x00;                                                           /* dummy */
            buf[3] = 0x00;                                                           /* dummy */
            buf[4] = wrap;                                                           /* wrap */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 5, NULL, 0);       /* spi write read */
            if (res != 0)                                                            /* check result */
            {
                handle->debug_print("w25qxx: set burst with wrap failed.\n");        /* set burst with wrap failed */
               
                return 1;                                                            /* return error */
            }
        }
    }
    else
    {
        handle->debug_print("w25qxx: qspi can't use this function.\n");              /* qspi can't use this function */
       
        return 4;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t w25qxx_init(w25qxx_handle_t *handle)
{
    uint8_t res;
    uint8_t status;
    uint8_t buf[4];
    uint8_t out[2];
    uint16_t id;
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->debug_print == NULL)                                                       /* check debug_print */
    {
        return 3;                                                                          /* return error */
    }
    if (handle->spi_qspi_init == NULL)                                                     /* check spi_qspi_init */
    {
        handle->debug_print("w25qxx: spi_qspi_init is null.\n");                           /* spi_qspi_init is null */
       
        return 3;                                                                          /* return error */
    }
    if (handle->spi_qspi_deinit == NULL)                                                   /* check spi_qspi_deinit */
    {
        handle->debug_print("w25qxx: spi_qspi_deinit is null.\n");                         /* spi_qspi_deinit is null */
       
        return 3;                                                                          /* return error */
    }
    if (handle->spi_qspi_write_read == NULL)                                               /* check spi_qspi_write_read */
    {
        handle->debug_print("w25qxx: spi_qspi_write_read is null.\n");                     /* spi_qspi_write_read is null */
       
        return 3;                                                                          /* return error */
    }
    if (handle->delay_us == NULL)                                                          /* check delay_us */
    {
        handle->debug_print("w25qxx: delay_us is null.\n");                                /* delay_us is null */
       
        return 3;                                                                          /* return error */
    }
    if (handle->delay_ms == NULL)                                                          /* check delay_ms */
    {
        handle->debug_print("w25qxx: delay_ms is null.\n");                                /* delay_ms is null */
       
        return 3;                                                                          /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                          /* spi interface */
    {
        res = handle->spi_qspi_init();                                                     /* spi init */
        if (res != 0)                                                                      /* check result */
        {
            handle->debug_print("w25qxx: spi init failed.\n");                             /* spi init failed */
           
            return 1;                                                                      /* return error */
        }
        if (handle->dual_quad_spi_enable != 0)                                             /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle,
                                           W25QXX_COMMAND_RELEASE_POWER_DOWN, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           3 * 8, NULL, 0x00,
                                          (uint8_t *)&id, 1, 1);                           /* spi write read */
            if (res != 0)                                                                  /* check result */
            {
                handle->debug_print("w25qxx: release power down failed.\n");               /* release power down failed */
                (void)handle->spi_qspi_deinit();                                           /** deinit */
                
                return 1;                                                                  /* return error */
            }
            res = a_w25qxx_qspi_write_read(handle,
                                           W25QXX_COMMAND_ENABLE_RESET, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                              /* spi write read */
            if (res != 0)                                                                  /* check result */
            {
                handle->debug_print("w25qxx: enable reset failed.\n");                     /* enable reset failed */
                (void)handle->spi_qspi_deinit();                                           /** deinit */
                
                return 7;                                                                  /* return error */
            }
            res = a_w25qxx_qspi_write_read(handle,
                                           W25QXX_COMMAND_RESET_DEVICE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                              /* spi write read */
            if (res != 0)                                                                  /* check result */
            {
                handle->debug_print("w25qxx: reset device failed.\n");                     /* reset device failed */
                (void)handle->spi_qspi_deinit();                                           /** deinit */
                
                return 7;                                                                  /* return error */
            }
            handle->delay_ms(10);                                                          /* delay 10 ms */
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_MANUFACTURER, 1,
                                           0x00000000, 1, 3,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                          (uint8_t *)out, 2, 1);                           /* qspi write read */
            if (res != 0)                                                                  /* check result */
            {
                handle->debug_print("w25qxx: get manufacturer device id failed.\n");       /* get manufacturer device id failed */
                (void)handle->spi_qspi_deinit();                                           /** deinit */
                
                return 1;                                                                  /* return error */
            }
            id = (uint16_t)out[0] << 8 | out[1];                                           /* set id */
            if (id != handle->type)                                                        /* check id */
            {
                handle->debug_print("w25qxx: id is invalid.\n");                           /* id is invalid */
                (void)handle->spi_qspi_deinit();                                           /** deinit */
                
                return 6;                                                                  /* return error */
            }
            if (handle->type >= W25Q256)
            {
                res = a_w25qxx_qspi_write_read(handle, 0xE9, 1,
                                               0x00000000, 0, 0,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0x00,
                                               NULL, 0, 0);                                /* qspi write read */
                if (res != 0)                                                              /* check result */
                {
                    handle->debug_print("w25qxx: enter 3 byte mode failed.\n");            /* enter 3 byte mode failed */
                    (void)handle->spi_qspi_deinit();                                       /** deinit */
                    
                    return 1;                                                              /* return error */
                }
            }
        }
        else                                                                               /* single spi */
        {
            buf[0] = W25QXX_COMMAND_RELEASE_POWER_DOWN;                                    /* release power down command */
            buf[1] = 0xFF;                                                                 /* dummy */
            buf[2] = 0xFF;                                                                 /* dummy */
            buf[3] = 0xFF;                                                                 /* dummy */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 4,
                                         (uint8_t *)&id, 1);                               /* spi write read */
            if (res != 0)                                                                  /* check result */
            {
                handle->debug_print("w25qxx: release power down failed.\n");               /* release power down failed */
                (void)handle->spi_qspi_deinit();                                           /** deinit */
                
                return 1;                                                                  /* return error */
            }
            buf[0] = W25QXX_COMMAND_ENABLE_RESET;                                          /* enable reset command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);             /* spi write read */
            if (res != 0)                                                                  /* check result */
            {
                handle->debug_print("w25qxx: enable reset failed.\n");                     /* enable reset failed */
                (void)handle->spi_qspi_deinit();                                           /** deinit */
                
                return 7;                                                                  /* return error */
            }
            buf[0] = W25QXX_COMMAND_RESET_DEVICE;                                          /* reset device command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);             /* spi write read */
            if (res != 0)                                                                  /* check result */
            {
                handle->debug_print("w25qxx: reset device failed.\n");                     /* reset device failed */
                (void)handle->spi_qspi_deinit();                                           /** deinit */
                
                return 7;                                                                  /* return error */
            }
            handle->delay_ms(10);                                                          /* delay 10 ms */
            buf[0] = W25QXX_COMMAND_READ_MANUFACTURER;                                     /* read manufacturer command */
            buf[1] = 0x00;                                                                 /* dummy */
            buf[2] = 0x00;                                                                 /* dummy */
            buf[3] = 0x00;                                                                 /* dummy */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 4,
                                         (uint8_t *)out, 2);                               /* spi write read */
            if (res != 0)                                                                  /* check result */
            {
                handle->debug_print("w25qxx: get manufacturer device id failed.\n");       /* get manufacturer device id failed */
                (void)handle->spi_qspi_deinit();                                           /** deinit */
                
                return 4;                                                                  /* return error */
            }
            id = (uint16_t)out[0] << 8 | out[1];                                           /* set id */
            if (id != handle->type)                                                        /* check id */
            {
                handle->debug_print("w25qxx: id is invalid.\n");                           /* id is invalid */
                (void)handle->spi_qspi_deinit();                                           /** deinit */
                
                return 6;                                                                  /* return error */
            }
            if (handle->type >= W25Q256)
            {
                buf[0] = 0xE9;                                                             /* 3 byte mode */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);         /* spi write read */
                if (res != 0)                                                              /* check result */
                {
                    handle->debug_print("w25qxx: set address mode failed.\n");             /* set address mode failed */
                    (void)handle->spi_qspi_deinit();                                       /** deinit */
                    
                    return 8;                                                              /* return error */
                }
            }
        }
    }
    else
    {
        res = handle->spi_qspi_init();                                                     /* qspi init */
        if (res != 0)                                                                      /* check result */
        {
            handle->debug_print("w25qxx: qspi init failed.\n");                            /* qspi init failed */
           
            return 1;                                                                      /* return error */
        }
        res = a_w25qxx_qspi_write_read(handle,
                                       W25QXX_COMMAND_RELEASE_POWER_DOWN, 1,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       3 * 8, NULL, 0x00,
                                      (uint8_t *)&id, 1, 1);                               /* spi write read */
        if (res != 0)                                                                      /* check result */
        {
            handle->debug_print("w25qxx: release power down failed.\n");                   /* release power down failed */
            (void)handle->spi_qspi_deinit();                                               /** deinit */
            
            return 1;                                                                      /* return error */
        }
        res = a_w25qxx_qspi_write_read(handle,
                                       W25QXX_COMMAND_ENABLE_RESET, 1,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                  /* spi write read */
        if (res != 0)                                                                      /* check result */
        {
            handle->debug_print("w25qxx: enable reset failed.\n");                         /* enable reset failed */
            (void)handle->spi_qspi_deinit();                                               /** deinit */
            
            return 7;                                                                      /* return error */
        }
        res = a_w25qxx_qspi_write_read(handle,
                                       W25QXX_COMMAND_RESET_DEVICE, 1,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                  /* spi write read */
        if (res != 0)                                                                      /* check result */
        {
            handle->debug_print("w25qxx: reset device failed.\n");                         /* reset device failed */
            (void)handle->spi_qspi_deinit();                                               /** deinit */
            
            return 7;                                                                      /* return error */
        }
        handle->delay_ms(10);                                                              /* delay 10 ms */
        res = a_w25qxx_qspi_write_read(handle,
                                       W25QXX_COMMAND_READ_STATUS_REG2, 1,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                      (uint8_t *)&status, 1, 1);                           /* spi write read */
        if (res != 0)                                                                      /* check result */
        {
            handle->debug_print("w25qxx: read status 2 failed.\n");                        /* read status 2 failed */
            (void)handle->spi_qspi_deinit();                                               /** deinit */
            
            return 5;                                                                      /* return error */
        }
        if ((status & 0x02) == 0)                                                          /* check status */
        {
            res = a_w25qxx_qspi_write_read(handle,
                                           W25QXX_COMMAND_VOLATILE_SR_WRITE_ENABLE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                              /* spi write read */
            if (res != 0)                                                                  /* check result */
            {
                handle->debug_print("w25qxx: set sr write enable failed.\n");              /* set sr write enable failed */
                (void)handle->spi_qspi_deinit();                                           /** deinit */
                
                return 5;                                                                  /* return error */
            }
            res = a_w25qxx_qspi_write_read(handle,
                                           W25QXX_COMMAND_WRITE_STATUS_REG2, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                              /* spi write read */
            if (res != 0)                                                                  /* check result */
            {
                handle->debug_print("w25qxx: write status 2 failed.\n");                   /* write status 2 failed */
                (void)handle->spi_qspi_deinit();                                           /** deinit */
                
                return 5;                                                                  /* return error */
            }
        }
        res = a_w25qxx_qspi_write_read(handle,
                                       W25QXX_COMMAND_ENTER_QSPI_MODE, 1,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                  /* spi write read */
        if (res != 0)                                                                      /* check result */
        {
            handle->debug_print("w25qxx: enter qspi failed.\n");                           /* enter qspi failed */
            (void)handle->spi_qspi_deinit();                                               /** deinit */
            
            return 5;                                                                      /* return error */
        }
        handle->delay_ms(10);                                                              /* delay 10 ms */
        buf[0] = 3 << 4;                                                                   /* set 8 read dummy */
        handle->param = buf[0];                                                            /* set param */
        handle->dummy = 8;                                                                 /* set dummy */
        res = a_w25qxx_qspi_write_read(handle, 0xC0, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0, (uint8_t *)buf, 1,
                                       NULL, 0x00, 4);                                     /* qspi write read */
        if (res != 0)                                                                      /* check result */
        {
            handle->debug_print("w25qxx: set read parameters failed.\n");                  /* set read parameters failed */
            (void)handle->spi_qspi_deinit();                                               /** deinit */
            
            return 5;                                                                      /* return error */
        }
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_MANUFACTURER, 4,
                                       0x00000000, 4, 3,
                                       0x00000000, 0x00, 0x00,
                                       0, NULL, 0x00,
                                      (uint8_t *)out, 2, 4);                               /* qspi write read */
        if (res != 0)                                                                      /* check result */
        {
            handle->debug_print("w25qxx: get manufacturer device id failed.\n");           /* get manufacturer device id failed */
            (void)handle->spi_qspi_deinit();                                               /** deinit */
            
            return 1;                                                                      /* return error */
        }
        id = (uint16_t)out[0] << 8 | out[1];                                               /* set id */
        if (id != handle->type)                                                            /* check id */
        {
            handle->debug_print("w25qxx: id is invalid.\n");                               /* id is invalid */
            (void)handle->spi_qspi_deinit();                                               /** deinit */
            
            return 6;                                                                      /* return error */
        }
        if (handle->type >= W25Q256)
        {
            res = a_w25qxx_qspi_write_read(handle, 0xE9, 4,
                                           0x00000000, 0, 0,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           NULL, 0, 0);                                    /* qspi write read */
            if (res != 0)                                                                  /* check result */
            {
                handle->debug_print("w25qxx: enter 3 byte mode failed.\n");                /* enter 3 byte mode failed */
                (void)handle->spi_qspi_deinit();                                           /** deinit */
                
                return 1;                                                                  /* return error */
            }
        }
    }
    handle->address_mode = W25QXX_ADDRESS_MODE_3_BYTE;                                     /* set address mode */
    handle->inited = 1;                                                                    /* initialize inited */
    
    return 0;                                                                              /* success return 0 */
}

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
uint8_t w25qxx_deinit(w25qxx_handle_t *handle)
{
    uint8_t res;
    uint8_t buf[1];
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }   
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }

    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                  /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                     /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_POWER_DOWN, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           NULL, 0x00, 0);                         /* spi write read */
            if (res != 0)                                                          /* check result */
            {
                handle->debug_print("w25qxx: power down failed.\n");               /* power down failed */
               
                return 4;                                                          /* return error */
            }
        }
        else                                                                       /* single spi */
        {
            buf[0] = W25QXX_COMMAND_POWER_DOWN;                                    /* power down command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf,
                                          1, NULL, 0);                             /* spi write read */
            if (res != 0)                                                          /* check result */
            {
                handle->debug_print("w25qxx: power down failed.\n");               /* power down failed */
               
                return 4;                                                          /* return error */
            }
        }
    }
    else                                                                           /* qspi interface */
    {
        res = a_w25qxx_qspi_write_read(handle, 0xFF, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0, NULL, 0x00,
                                       NULL, 0x00, 0);                             /* spi write read */
        if (res != 0)                                                              /* check result */
        {
            handle->debug_print("w25qxx: exit qspi mode failed.\n");               /* exit qspi mode failed */
           
            return 1;                                                              /* return error */
        }
        handle->delay_ms(10);                                                      /* delay 10 ms */
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_POWER_DOWN, 1,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0, NULL, 0x00,
                                       NULL, 0x00, 0);                             /* spi write read */
        if (res != 0)                                                              /* check result */
        {
            handle->debug_print("w25qxx: power down failed.\n");                   /* power down failed */
           
            return 4;                                                              /* return error */
        }
    }
    res = handle->spi_qspi_deinit();                                               /* qspi deinit */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("w25qxx: spi or qspi deinit failed.\n");               /* spi or qspi deinit failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t w25qxx_read(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len)
{
    uint8_t res;
    uint8_t buf[6];
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }   
    if (handle->inited != 1)                                                                              /* check handle initialization */
    {
        return 3;                                                                                         /* return error */
    }
    
    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                         /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                            /* enable dual quad spi */
        {
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                       /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                              /* >128Mb */
                {
                    res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00, NULL, 0x00,
                                                   NULL, 0x00, 0x00);                                     /* qspi write read */
                    if (res != 0)                                                                         /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                            /* write enable failed */
                       
                        return 1;                                                                         /* return error */
                    }
                    buf[0] = (addr >> 24) & 0xFF;                                                         /* 31 - 24 bits */
                    res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0, (uint8_t *)buf, 0x01,
                                                   NULL, 0x00, 1);                                        /* spi write read */
                    if (res != 0)                                                                         /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");            /* write extended addr register failed */
                       
                        return 1;                                                                         /* return error */
                    }
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ, 1,
                                               addr, 1, 3,
                                               0x00000000, 0x00, 0x00,
                                               8, NULL, 0x00,                                             /* spi write */
                                               data, len, 1);                                             /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: fast read failed.\n");                                   /* fast read failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ, 1,
                                               addr, 1, 4,
                                               0x00000000, 0x00, 0x00,
                                               8, NULL, 0x00,                                             /* spi write */
                                               data, len, 1);                                             /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: fast read failed.\n");                                   /* fast read failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                /* address mode is invalid */
               
                return 4;                                                                                 /* return error */
            }
        }
        else                                                                                              /* single spi */
        {
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                       /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                              /* >128Mb */
                {
                    buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                 /* write enable command */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                    /* spi write read */
                    if (res != 0)                                                                         /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                            /* write enable failed */
                       
                        return 1;                                                                         /* return error */
                    }
                    buf[0] = 0xC5;                                                                        /* write extended addr register command */
                    buf[1] = (addr >> 24) & 0xFF;                                                         /* 31 - 24 bits */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 2, NULL, 0);                    /* spi write read */
                    if (res != 0)                                                                         /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");            /* write extended addr register failed */
                       
                        return 1;                                                                         /* return error */
                    }
                }
                buf[0] = W25QXX_COMMAND_FAST_READ;                                                        /* fast read command */
                buf[1] = (addr >> 16) & 0xFF;                                                             /* 23 - 16 bits */
                buf[2] = (addr >> 8) & 0xFF;                                                              /* 15 - 8  bits */
                buf[3] = (addr >> 0) & 0xFF;                                                              /* 7 - 0 bits */
                buf[4] = 0x00;                                                                            /* dummy */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 5, (uint8_t *)data, (uint32_t)len); /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: fast read failed.\n");                                   /* fast read failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE)
                    && (handle->type >= W25Q256))                                                         /* check address mode */
            {
                buf[0] = W25QXX_COMMAND_FAST_READ;                                                        /* fast read command */
                buf[1] = (addr >> 24) & 0xFF;                                                             /* 31 - 24 bits */
                buf[2] = (addr >> 16) & 0xFF;                                                             /* 23 - 16 bits */
                buf[3] = (addr >> 8) & 0xFF;                                                              /* 15 - 8  bits */
                buf[4] = (addr >> 0) & 0xFF;                                                              /* 7 - 0 bits */
                buf[5] = 0x00;                                                                            /* dummy */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 6, (uint8_t *)data, (uint32_t)len); /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: fast read failed.\n");                                   /* fast read failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                /* address mode is invalid */
               
                return 4;                                                                                 /* return error */
            }
        }
    }
    else                                                                                                  /* qspi interface */
    {
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                           /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                  /* >128Mb */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                         /* qspi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                /* write enable failed */
                   
                    return 1;                                                                             /* return error */
                }
                buf[0] = (addr >> 24) & 0xFF;                                                             /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 4);                                            /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                /* write extended addr register failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ, 4,
                                           addr, 4, 3,
                                           0x00000000, 0x00, 0x00,
                                           handle->dummy, NULL, 0x00,                                     /* spi write */
                                           data, len, 4);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: fast read failed.\n");                                       /* fast read failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ, 4,
                                           addr, 4, 4,
                                           0x00000000, 0x00, 0x00,
                                           handle->dummy, NULL, 0x00,                                     /* spi write */
                                           data, len, 4);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: fast read failed.\n");                                       /* fast read failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                    /* address mode is invalid */
           
            return 4;                                                                                     /* return error */
        }
    }
    
    return 0;                                                                                             /* success return 0 */
}

/**
 * @brief      read data
 * @param[in]  *handle points to a w25qxx handle structure
 * @param[in]  addr is the read address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_w25qxx_read(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len)
{
    uint8_t res;
    uint8_t buf[6];

    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                         /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                            /* enable dual quad spi */
        {
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                       /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                              /* >128Mb */
                {
                    res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00, NULL, 0x00,
                                                   NULL, 0x00, 0x00);                                     /* qspi write read */
                    if (res != 0)                                                                         /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                            /* write enable failed */
                       
                        return 1;                                                                         /* return error */
                    }
                    buf[0] = (addr >> 24) & 0xFF;                                                         /* 31 - 24 bits */
                    res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0, (uint8_t *)buf, 0x01,
                                                   NULL, 0x00, 1);                                        /* spi write read */
                    if (res != 0)                                                                         /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");            /* write extended addr register failed */
                       
                        return 1;                                                                         /* return error */
                    }
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ, 1,
                                               addr, 1, 3,
                                               0x00000000, 0x00, 0x00,
                                               8, NULL, 0x00,
                                               data, len, 1);                                             /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: fast read failed.\n");                                   /* fast read failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ, 1,
                                               addr, 1, 4,
                                               0x00000000, 0x00, 0x00,
                                               8, NULL, 0x00,
                                               data, len, 1);                                             /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: fast read failed.\n");                                   /* fast read failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                /* address mode is invalid */
               
                return 1;                                                                                 /* return error */
            }
        }
        else
        {
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                       /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                              /* >128Mb */
                {
                    buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                 /* write enable command */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                    /* spi write read */
                    if (res != 0)                                                                         /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                            /* write enable failed */
                       
                        return 1;                                                                         /* return error */
                    }
                    buf[0] = 0xC5;                                                                        /* write extended addr register command */
                    buf[1] = (addr >> 24) & 0xFF;                                                         /* 31 - 24 bits */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 2, NULL, 0);                    /* spi write read */
                    if (res != 0)                                                                         /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");            /* write extended addr register failed */
                       
                        return 1;                                                                         /* return error */
                    }
                }
                buf[0] = W25QXX_COMMAND_FAST_READ;                                                        /* fast read command */
                buf[1] = (addr >> 16) & 0xFF;                                                             /* 23 - 16 bits */
                buf[2] = (addr >> 8) & 0xFF;                                                              /* 15 - 8  bits */
                buf[3] = (addr >> 0) & 0xFF;                                                              /* 7 - 0 bits */
                buf[4] = 0x00;                                                                            /* dummy */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 5, (uint8_t *)data, (uint32_t)len); /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: fast read failed.\n");                                   /* fast read failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))   /* check address mode */
            {
                buf[0] = W25QXX_COMMAND_FAST_READ;                                                        /* fast read command */
                buf[1] = (addr >> 24) & 0xFF;                                                             /* 31 - 24 bits */
                buf[2] = (addr >> 16) & 0xFF;                                                             /* 23 - 16 bits */
                buf[3] = (addr >> 8) & 0xFF;                                                              /* 15 - 8  bits */
                buf[4] = (addr >> 0) & 0xFF;                                                              /* 7 - 0 bits */
                buf[5] = 0x00;                                                                            /* dummy */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 6, (uint8_t *)data, (uint32_t)len); /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: fast read failed.\n");                                   /* fast read failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                /* address mode is invalid */
               
                return 1;                                                                                 /* return error */
            }
        }
    }
    else                                                                                                  /* qspi interface */
    {
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                           /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                  /* >128Mb */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                         /* qspi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                /* write enable failed */
                   
                    return 1;                                                                             /* return error */
                }
                buf[0] = (addr >> 24) & 0xFF;                                                             /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 4);                                            /* spi write read */
                if (res != 0)                                                                             /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                /* write extended addr register failed */
                   
                    return 1;                                                                             /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ, 4,
                                           addr, 4, 3,
                                           0x00000000, 0x00, 0x00,
                                           handle->dummy, NULL, 0x00,                                     /* spi write */
                                           data, len, 4);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: fast read failed.\n");                                       /* fast read failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_FAST_READ, 4,
                                           addr, 4, 4,
                                           0x00000000, 0x00, 0x00,
                                           handle->dummy, NULL, 0x00,                                     /* spi write */
                                           data, len, 4);                                                 /* spi write read */
            if (res != 0)                                                                                 /* check result */
            {
                handle->debug_print("w25qxx: fast read failed.\n");                                       /* fast read failed */
               
                return 1;                                                                                 /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                    /* address mode is invalid */
           
            return 1;                                                                                     /* return error */
        }
    }
    
    return 0;                                                                                             /* success return 0 */
}

/**
 * @brief     erase the sector
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] addr is the erase address
 * @return    status code
 *            - 0 success
 *            - 1 sector erase failed
 * @note      none
 */
static uint8_t a_w25qxx_erase_sector(w25qxx_handle_t *handle, uint32_t addr)
{
    uint8_t res;
    uint8_t status;
    uint32_t timeout;
    uint8_t buf[5];

    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                           /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                              /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                               /* qspi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                      /* write enable failed */
               
                return 1;                                                                                   /* return error */
            }
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                         /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                                /* >128Mb */
                {
                    buf[0] = (addr >> 24) & 0xFF;                                                           /* 31 - 24 bits */
                    res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0, (uint8_t *)buf, 0x01,
                                                   NULL, 0x00, 1);                                          /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");              /* write extended addr register failed */
                       
                        return 1;                                                                           /* return error */
                    }
                    res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00, NULL, 0x00,
                                                   NULL, 0x00, 0x00);                                       /* qspi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                              /* write enable failed */
                       
                        return 1;                                                                           /* return error */
                    }
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_SECTOR_ERASE_4K, 1,
                                               addr, 1, 3,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: sector erase 4k failed.\n");                               /* sector erase 4k failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_SECTOR_ERASE_4K, 1,
                                               addr, 1, 4,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: sector erase 4k failed.\n");                               /* sector erase 4k failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                  /* address mode is invalid */
               
                return 1;                                                                                   /* return error */
            }
            
            timeout = 400;                                                                                  /* max 400 ms */
            while (timeout != 0)                                                                            /* check timeout */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0,
                                              (uint8_t *)&status, 1, 1);                                    /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                   /* get status1 failed */
                   
                    return 1;                                                                               /* return error */
                }
                if ((status & 0x01) == 0x00)                                                                /* check status */
                {
                    break;                                                                                  /* break */
                }
                timeout--;                                                                                  /* timeout-- */
                handle->delay_ms(1);                                                                        /* delay 1 ms */
            }
            if (timeout == 0)
            {
                handle->debug_print("w25qxx: sector erase 4k timeout.\n");                                  /* sector erase 4k timeout */
               
                return 1;                                                                                   /* return error */
            }
        }
        else                                                                                                /* single spi */
        {
            buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                           /* write enable command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                              /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                      /* write enable failed */
               
                return 1;                                                                                   /* return error */
            }
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                         /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                                /* >128Mb */
                {
                    buf[0] = 0xC5;                                                                          /* write extended addr register command */
                    buf[1] = (addr >> 24) & 0xFF;                                                           /* 31 - 24 bits */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 2, NULL, 0);                      /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");              /* write extended addr register failed */
                       
                        return 1;                                                                           /* return error */
                    }
                    buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                   /* write enable command */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                      /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                              /* write enable failed */
                       
                        return 1;                                                                           /* return error */
                    }
                }
                buf[0] = W25QXX_COMMAND_SECTOR_ERASE_4K;                                                    /* sector erase 4k command */
                buf[1] = (addr >> 16) & 0xFF;                                                               /* 23 - 16 bits */
                buf[2] = (addr >> 8) & 0xFF;                                                                /* 15 - 8  bits */
                buf[3] = (addr >> 0) & 0xFF;                                                                /* 7 - 0 bits */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 4, NULL, 0);                          /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: sector erase 4k failed.\n");                               /* sector erase 4k failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                buf[0] = W25QXX_COMMAND_SECTOR_ERASE_4K;                                                    /* sector erase 4k command */
                buf[1] = (addr >> 24) & 0xFF;                                                               /* 31 - 24 bits */
                buf[2] = (addr >> 16) & 0xFF;                                                               /* 23 - 16 bits */
                buf[3] = (addr >> 8) & 0xFF;                                                                /* 15 - 8  bits */
                buf[4] = (addr >> 0) & 0xFF;                                                                /* 7 - 0 bits */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 5, NULL, 0);                          /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: sector erase 4k failed.\n");                               /* sector erase 4k failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                  /* address mode is invalid */
               
                return 1;                                                                                   /* return error */
            }
            
            timeout = 400;                                                                                  /* max 400 ms */
            while (timeout != 0)                                                                            /* check timeout */
            {
                buf[0] = W25QXX_COMMAND_READ_STATUS_REG1;                                                   /* read status1 command */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, (uint8_t *)&status, 1);            /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                   /* get status1 failed */
                   
                    return 1;                                                                               /* return error */
                }
                if ((status & 0x01) == 0x00)                                                                /* check status */
                {
                    break;                                                                                  /* break */
                }
                timeout--;                                                                                  /* timeout-- */
                handle->delay_ms(1);                                                                        /* delay 1 ms */
            }
            if (timeout == 0)
            {
                handle->debug_print("w25qxx: sector erase 4k timeout.\n");                                  /* sector erase 4k timeout */
               
                return 1;                                                                                   /* return error */
            }
        }
    }
    else
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                                   /* qspi write read */
        if (res != 0)                                                                                       /* check result */
        {
            handle->debug_print("w25qxx: write enable failed.\n");                                          /* write enable failed */
           
            return 1;                                                                                       /* return error */
        }
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                             /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                    /* >128Mb */
            {
                buf[0] = (addr >> 24) & 0xFF;                                                               /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 4);                                              /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                  /* write extended addr register failed */
                   
                    return 1;                                                                               /* return error */
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* qspi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                  /* write enable failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_SECTOR_ERASE_4K, 4,
                                           addr, 4, 3,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                               /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: sector erase 4k failed.\n");                                   /* sector erase 4k failed */
               
                return 1;                                                                                   /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_SECTOR_ERASE_4K, 4,
                                           addr, 4, 4,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                               /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: sector erase 4k failed.\n");                                   /* sector erase 4k failed */
               
                return 1;                                                                                   /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                      /* address mode is invalid */
           
            return 1;                                                                                       /* return error */
        }
        
        timeout = 400;                                                                                      /* max 400 ms */
        while (timeout != 0)                                                                                /* check timeout */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 4,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0,
                                          (uint8_t *)&status, 1, 4);                                        /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: get status1 failed.\n");                                       /* get status1 failed */
               
                return 1;                                                                                   /* return error */
            }
            if ((status & 0x01) == 0x00)                                                                    /* check status */
            {
                break;                                                                                      /* break */
            }
            timeout--;                                                                                      /* timeout-- */
            handle->delay_ms(1);                                                                            /* delay 1 ms */
        }
        if (timeout == 0)
        {
            handle->debug_print("w25qxx: sector erase 4k timeout.\n");                                      /* sector erase 4k timeout */
           
            return 1;                                                                                       /* return error */
        }
    }
    
    return 0;                                                                                               /* success return 0 */
}

/**
 * @brief     page program
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] addr is the programming address
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 page program failed
 * @note      none
 */
static uint8_t a_w25qxx_page_program(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint16_t len)
{
    uint8_t res;
    uint8_t status;
    uint32_t timeout;
    uint8_t buf[2];

    if (handle->spi_qspi == W25QXX_INTERFACE_SPI)                                                           /* spi interface */
    {
        if (handle->dual_quad_spi_enable != 0)                                                              /* enable dual quad spi */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0x00, NULL, 0x00,
                                           NULL, 0x00, 0x00);                                               /* qspi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                      /* write enable failed */
               
                return 1;                                                                                   /* return error */
            }
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                         /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                                /* >128Mb */
                {
                    buf[0] = (addr >> 24) & 0xFF;                                                           /* 31 - 24 bits */
                    res = a_w25qxx_qspi_write_read(handle, 0xC5, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0, (uint8_t *)buf, 0x01,
                                                   NULL, 0x00, 1);                                          /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");              /* write extended addr register failed */
                       
                        return 1;                                                                           /* return error */
                    }
                    res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 1,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00000000, 0x00, 0x00,
                                                   0x00, NULL, 0x00,
                                                   NULL, 0x00, 0x00);                                       /* qspi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                              /* write enable failed */
                       
                        return 1;                                                                           /* return error */
                    }
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_PAGE_PROGRAM, 1,
                                               addr, 1, 3,
                                               0x00000000, 0x00, 0x00,
                                               0, data, len,
                                               NULL, 0x00, 1);                                              /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: page program failed.\n");                                  /* page program failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_PAGE_PROGRAM, 1,
                                               addr, 1, 4,
                                               0x00000000, 0x00, 0x00,
                                               0, data, len,
                                               NULL, 0x00, 1);                                              /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: page program failed.\n");                                  /* page program failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                  /* address mode is invalid */
               
                return 1;                                                                                   /* return error */
            }
            
            timeout = 3 * 100;                                                                              /* max 3 ms */
            while (timeout != 0)                                                                            /* check timeout */
            {
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 1,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, NULL, 0,
                                              (uint8_t *)&status, 1, 1);                                    /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: page program failed.\n");                                  /* page program failed */
                   
                    return 1;                                                                               /* return error */
                }
                if ((status & 0x01) == 0x00)                                                                /* check status */
                {
                    break;                                                                                  /* break */
                }
                timeout--;                                                                                  /* timeout-- */
                handle->delay_us(10);                                                                       /* delay 10 us */
            }
            if (timeout == 0)
            {
                handle->debug_print("w25qxx: page program timeout.\n");                                     /* page program timeout */
               
                return 1;                                                                                   /* return error */
            }
        }
        else                                                                                                /* single spi */
        {
            buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                           /* write enable command */
            res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                              /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: write enable failed.\n");                                      /* write enable failed */
               
                return 1;                                                                                   /* return error */
            }
            if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                         /* 3 address mode */
            {
                if (handle->type >= W25Q256)                                                                /* >128Mb */
                {
                    buf[0] = 0xC5;                                                                          /* write extended addr register command */
                    buf[1] = (addr >> 24) & 0xFF;                                                           /* 31 - 24 bits */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 2, NULL, 0);                      /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write extended addr register failed.\n");              /* write extended addr register failed */
                       
                        return 1;                                                                           /* return error */
                    }
                    buf[0] = W25QXX_COMMAND_WRITE_ENABLE;                                                   /* write enable command */
                    res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, NULL, 0);                      /* spi write read */
                    if (res != 0)                                                                           /* check result */
                    {
                        handle->debug_print("w25qxx: write enable failed.\n");                              /* write enable failed */
                       
                        return 1;                                                                           /* return error */
                    }
                }
                handle->buf[0] = W25QXX_COMMAND_PAGE_PROGRAM;                                               /* page program command */
                handle->buf[1] = (addr >> 16) & 0xFF;                                                       /* 23 - 16 bits */
                handle->buf[2] = (addr >> 8) & 0xFF;                                                        /* 15 - 8  bits */
                handle->buf[3] = (addr >> 0) & 0xFF;                                                        /* 7 - 0 bits */
                memcpy(&handle->buf[4], data, len);                                                         /* copy data */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)handle->buf, 4 + len, NULL, 0);            /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: page program failed.\n");                                  /* page program failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE)
                     && (handle->type >= W25Q256))                                                          /* 4 address mode */
            {
                handle->buf[0] = W25QXX_COMMAND_PAGE_PROGRAM;                                               /* page program command */
                handle->buf[1] = (addr >> 24) & 0xFF;                                                       /* 31 - 24 bits */
                handle->buf[2] = (addr >> 16) & 0xFF;                                                       /* 23 - 16 bits */
                handle->buf[3] = (addr >> 8) & 0xFF;                                                        /* 15 - 8  bits */
                handle->buf[4] = (addr >> 0) & 0xFF;                                                        /* 7 - 0 bits */
                memcpy(&handle->buf[5], data, len);                                                         /* copy data */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)handle->buf, 5 + len, NULL, 0);            /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: page program failed.\n");                                  /* page program failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            else
            {
                handle->debug_print("w25qxx: address mode is invalid.\n");                                  /* address mode is invalid */
               
                return 1;                                                                                   /* return error */
            }
            
            timeout = 3 * 100;                                                                              /* max 3 ms */
            while (timeout != 0)                                                                            /* check timeout */
            {
                buf[0] = W25QXX_COMMAND_READ_STATUS_REG1;                                                   /* read status1 command */
                res = a_w25qxx_spi_write_read(handle, (uint8_t *)buf, 1, (uint8_t *)&status, 1);            /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: get status1 failed.\n");                                   /* get status1 failed */
                   
                    return 1;                                                                               /* return error */
                }
                if ((status & 0x01) == 0x00)                                                                /* check status */
                {
                    break;                                                                                  /* break */
                }
                timeout--;                                                                                  /* timeout-- */
                handle->delay_us(10);                                                                       /* delay 10 us */
            }
            if (timeout == 0)
            {
                handle->debug_print("w25qxx: page program timeout.\n");                                     /* page program timeout */
               
                return 1;                                                                                   /* return error */
            }
        }
    }
    else
    {
        res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                       0x00000000, 0x00, 0x00,
                                       0x00000000, 0x00, 0x00,
                                       0x00, NULL, 0x00,
                                       NULL, 0x00, 0x00);                                                   /* qspi write read */
        if (res != 0)                                                                                       /* check result */
        {
            handle->debug_print("w25qxx: write enable failed.\n");                                          /* write enable failed */
           
            return 1;                                                                                       /* return error */
        }
        if (handle->address_mode == W25QXX_ADDRESS_MODE_3_BYTE)                                             /* 3 address mode */
        {
            if (handle->type >= W25Q256)                                                                    /* >128Mb */
            {
                buf[0] = (addr >> 24) & 0xFF;                                                               /* 31 - 24 bits */
                res = a_w25qxx_qspi_write_read(handle, 0xC5, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0, (uint8_t *)buf, 0x01,
                                               NULL, 0x00, 4);                                              /* spi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write extended addr register failed.\n");                  /* write extended addr register failed */
                   
                    return 1;                                                                               /* return error */
                }
                res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_WRITE_ENABLE, 4,
                                               0x00000000, 0x00, 0x00,
                                               0x00000000, 0x00, 0x00,
                                               0x00, NULL, 0x00,
                                               NULL, 0x00, 0x00);                                           /* qspi write read */
                if (res != 0)                                                                               /* check result */
                {
                    handle->debug_print("w25qxx: write enable failed.\n");                                  /* write enable failed */
                   
                    return 1;                                                                               /* return error */
                }
            }
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_PAGE_PROGRAM, 4,
                                           addr, 4, 3,
                                           0x00000000, 0x00, 0x00,
                                           0, data, len,
                                           NULL, 0x00, 4);                                                  /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: page program failed.\n");                                      /* page program failed */
               
                  return 1;                                                                                 /* return error */
            }
        }
        else if ((handle->address_mode == W25QXX_ADDRESS_MODE_4_BYTE) && (handle->type >= W25Q256))
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_PAGE_PROGRAM, 4,
                                           addr, 4, 4,
                                           0x00000000, 0x00, 0x00,
                                           0, data, len,
                                           NULL, 0x00, 4);                                                  /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: page program failed.\n");                                      /* page program failed */
               
                return 1;                                                                                   /* return error */
            }
        }
        else
        {
            handle->debug_print("w25qxx: address mode is invalid.\n");                                      /* address mode is invalid */
           
            return 1;                                                                                       /* return error */
        }
        
        timeout = 3 * 100;                                                                                  /* max 3 ms */
        while (timeout != 0)                                                                                /* check timeout */
        {
            res = a_w25qxx_qspi_write_read(handle, W25QXX_COMMAND_READ_STATUS_REG1, 4,
                                           0x00000000, 0x00, 0x00,
                                           0x00000000, 0x00, 0x00,
                                           0, NULL, 0,
                                          (uint8_t *)&status, 1, 4);                                        /* spi write read */
            if (res != 0)                                                                                   /* check result */
            {
                handle->debug_print("w25qxx: page program failed.\n");                                      /* page program failed */
               
                return 1;                                                                                   /* return error */
            }
            if ((status & 0x01) == 0x00)                                                                    /* check status */
            {
                break;                                                                                      /* break */
            }
            timeout--;                                                                                      /* timeout-- */
            handle->delay_us(10);                                                                           /* delay 10 us */
        }
        if (timeout == 0)
        {
            handle->debug_print("w25qxx: page program timeout.\n");                                         /* page program timeout */
           
            return 1;                                                                                       /* return error */
        }
    }
    
    return 0;                                                                                               /* success return 0 */
}

/**
 * @brief     write with no check
 * @param[in] *handle points to a w25qxx handle structure
 * @param[in] addr is the write address
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_w25qxx_write_no_check(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len)
{
    uint8_t res;
    uint16_t page_remain;
    
    page_remain = 256 - addr % 256;                                         /* get remain */
    if (len <= page_remain)                                                 /* check length */
    {
        page_remain = (uint16_t)len;                                        /* page remain */
    }
    while( 1)                                                               /* loop */
    { 
        res = a_w25qxx_page_program(handle, addr, data, page_remain);       /* page program */
        if (res != 0)
        {
            handle->debug_print("w25qxx: page program failed.\n");          /* page program failed */
           
            return 1;                                                       /* return error */
        }
        if (len == page_remain)                                             /* check length */
        {
            break;                                                          /* break loop */
        }
        else
        {
            data += page_remain;                                            /* data + page_remain */
            addr += page_remain;                                            /* address + page_remain */
            len -= page_remain;                                             /* length - page_remain */
            if (len > 256)                                                  /* check length */
            {
                page_remain = 256;                                          /* set page remain */
            }
            else
            {
                page_remain = (uint16_t)len;                                /* set length */
            }
        }
    }
    
    return 0;                                                               /* success return 0 */
}

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
uint8_t w25qxx_write(w25qxx_handle_t *handle, uint32_t addr, uint8_t *data, uint32_t len)
{
    uint8_t res;
    uint32_t sec_pos;
    uint32_t sec_off;
    uint32_t sec_remain;
    uint32_t i;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }   
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }

    sec_pos = addr / 4096;                                                                     /* get sector position */
    sec_off = addr % 4096;                                                                     /* get sector offset */
    sec_remain = 4096 - sec_off;                                                               /* get sector remain */
    if (len <= sec_remain)                                                                     /* check length */
    {
        sec_remain = len;                                                                      /* set remain */
    }
    while(1)                                                                                   /* loop */
    {    
        res = a_w25qxx_read(handle, sec_pos * 4096, handle->buf_4k, 4096);                     /* read 4k data */
        if (res != 0)
        {
            handle->debug_print("w25qxx: read failed.\n");                                     /* read failed */
           
            return 4;                                                                          /* return error */
        }
        for (i = 0; i< sec_remain; i++)                                                        /* sec_remain length */
        {
            if (handle->buf_4k[sec_off + i] != 0xFF)                                           /* check 0xFF */
            {
                break;                                                                         /* break loop */
            }
        }
        if (i < sec_remain)                                                                    /* not all is 0xFF */
        {
            res = a_w25qxx_erase_sector(handle, sec_pos * 4096);                               /* erase sector */
            if (res != 0)
            {
                handle->debug_print("w25qxx: erase sector failed.\n");                         /* erase sector failed */
               
                return 5;                                                                      /* return error */
            }
            for (i = 0; i<sec_remain; i++)                                                     /* sec_remain length */
            {
                handle->buf_4k[i + sec_off] = data[i];                                         /* copy data */
            }
            res = a_w25qxx_write_no_check(handle, sec_pos * 4096, handle->buf_4k, 4096);       /* write data no check */
            if (res != 0)                                                                      /* check result */
            {
                handle->debug_print("w25qxx: write failed.\n");                                /* write failed */
               
                return 1;                                                                      /* return error */
            }
        }
        else
        {
            res = a_w25qxx_write_no_check(handle, addr, data, sec_remain);                     /* write data */
            if (res != 0)                                                                      /* check result */
            {
                handle->debug_print("w25qxx: write failed.\n");                                /* write failed */
               
                return 1;                                                                      /* return error */
            }
        }    
        if (len == sec_remain)                                                                 /* check length length*/
        {
            break;                                                                             /* break loop */
        }
        else
        {
            sec_pos++;                                                                         /* sector++ */
            sec_off = 0;                                                                       /* set offset */
            data += sec_remain;                                                                /* data + remain */
            addr += sec_remain;                                                                /* addr + remain */
            len -= sec_remain;                                                                 /* len - remain */
            if (len > 4096)                                                                    /* check length */
            {
                sec_remain = 4096;                                                             /* set 4096 */
            }
            else
            {
                sec_remain = len;                                                              /* set length */
            }
        }
    }
    
    return 0;                                                                                  /* success return 0 */
}

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
                              uint8_t *out_buf, uint32_t out_len, uint8_t data_line)
{
    
    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }   
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }
    
    return a_w25qxx_qspi_write_read(handle, instruction, instruction_line,
                                    address, address_line, address_len,
                                    alternate, alternate_line, alternate_len,
                                    dummy, in_buf, in_len,
                                    out_buf, out_len, data_line);                /* write and read */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to a w25qxx info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t w25qxx_info(w25qxx_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(w25qxx_info_t));                         /* initialize w25qxx info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "SPI QSPI", 16);                       /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
