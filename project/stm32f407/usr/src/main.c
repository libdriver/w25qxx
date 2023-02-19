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
 * @file      main.c
 * @brief     main source file
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

#include "driver_w25qxx_basic.h"
#include "driver_w25qxx_advance.h"
#include "driver_w25qxx_read_test.h"
#include "driver_w25qxx_register_test.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "getopt.h"
#include <math.h>
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
volatile uint16_t g_len;                   /**< uart buffer length */

/**
 * @brief     w25qxx full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t w25qxx(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"data", required_argument, NULL, 2},
        {"dummy", required_argument, NULL, 3},
        {"length", required_argument, NULL, 4},
        {"interface", required_argument, NULL, 5},
        {"num", required_argument, NULL, 6},
        {"status", required_argument, NULL, 7},
        {"type", required_argument, NULL, 8},
        {"wrap", required_argument, NULL, 9},
        {NULL, 0, NULL, 0},
    };
    char type[49] = "unknown";
    uint32_t addr = 0;
    uint32_t data = 0;
    w25qxx_qspi_read_dummy_t dummy = W25QXX_QSPI_READ_DUMMY_8_80MHZ;
    w25qxx_qspi_read_wrap_length_t length = W25QXX_QSPI_READ_WRAP_LENGTH_8_BYTE;
    w25qxx_interface_t interface = W25QXX_INTERFACE_SPI;
    w25qxx_security_register_t num = W25QXX_SECURITY_REGISTER_1;
    w25qxx_type_t chip_type = W25Q128;
    w25qxx_burst_wrap_t wrap = W25QXX_BURST_WRAP_NONE;
    uint8_t status;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }

    /* init 0 */
    optind = 0;

    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);

        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 49);
                snprintf(type, 48, "h");

                break;
            }

            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 49);
                snprintf(type, 48, "i");

                break;
            }

            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 49);
                snprintf(type, 48, "p");

                break;
            }

            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 49);
                snprintf(type, 48, "e_%s", optarg);

                break;
            }

            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 49);
                snprintf(type, 48, "t_%s", optarg);

                break;
            }

            /* addr */
            case 1 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }

                /* set the addr */
                addr = hex_data & 0xFFFFFFFF;

                break;
            }

            /* data */
            case 2 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }

                /* set the data */
                data = hex_data & 0xFFFFFFFF;

                break;
            }

            /* dummy */
            case 3 :
            {
                /* set the dummy */
                if (strcmp("DUMMY_2_33MHZ", optarg) == 0)
                {
                    dummy = W25QXX_QSPI_READ_DUMMY_2_33MHZ;
                }
                else if (strcmp("DUMMY_4_55MHZ", optarg) == 0)
                {
                    dummy = W25QXX_QSPI_READ_DUMMY_4_55MHZ;
                }
                else if (strcmp("DUMMY_6_80MHZ", optarg) == 0)
                {
                    dummy = W25QXX_QSPI_READ_DUMMY_6_80MHZ;
                }
                else if (strcmp("DUMMY_8_80MHZ", optarg) == 0)
                {
                    dummy = W25QXX_QSPI_READ_DUMMY_8_80MHZ;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* length */
            case 4 :
            {
                /* set the length */
                if (strcmp("8", optarg) == 0)
                {
                    length = W25QXX_QSPI_READ_WRAP_LENGTH_8_BYTE;
                }
                else if (strcmp("16", optarg) == 0)
                {
                    length = W25QXX_QSPI_READ_WRAP_LENGTH_16_BYTE;
                }
                else if (strcmp("32", optarg) == 0)
                {
                    length = W25QXX_QSPI_READ_WRAP_LENGTH_32_BYTE;
                }
                else if (strcmp("64", optarg) == 0)
                {
                    length = W25QXX_QSPI_READ_WRAP_LENGTH_64_BYTE;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* interface */
            case 5 :
            {
                /* set the interface */
                if (strcmp("spi", optarg) == 0)
                {
                    interface = W25QXX_INTERFACE_SPI;
                }
                else if (strcmp("qspi", optarg) == 0)
                {
                    interface = W25QXX_INTERFACE_QSPI;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* num */
            case 6 :
            {
                /* set the num */
                if (strcmp("1", optarg) == 0)
                {
                    num = W25QXX_SECURITY_REGISTER_1;
                }
                else if (strcmp("2", optarg) == 0)
                {
                    num = W25QXX_SECURITY_REGISTER_2;
                }
                else if (strcmp("3", optarg) == 0)
                {
                    num = W25QXX_SECURITY_REGISTER_3;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* status */
            case 7 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }

                /* set the status */
                status = hex_data & 0xFF;

                break;
            }

            /* type */
            case 8 :
            {
                /* set the type */
                if (strcmp("W25Q80", optarg) == 0)
                {
                    chip_type = W25Q80;
                }
                else if (strcmp("W25Q16", optarg) == 0)
                {
                    chip_type = W25Q16;
                }
                else if (strcmp("W25Q32", optarg) == 0)
                {
                    chip_type = W25Q32;
                }
                else if (strcmp("W25Q64", optarg) == 0)
                {
                    chip_type = W25Q64;
                }
                else if (strcmp("W25Q128", optarg) == 0)
                {
                    chip_type = W25Q128;
                }
                else if (strcmp("W25Q256", optarg) == 0)
                {
                    chip_type = W25Q256;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* wrap */
            case 9 :
            {
                /* set the wrap */
                if (strcmp("WRAP_NONE", optarg) == 0)
                {
                    wrap = W25QXX_BURST_WRAP_NONE;
                }
                else if (strcmp("WRAP_8_BYTE", optarg) == 0)
                {
                    wrap = W25QXX_BURST_WRAP_8_BYTE;
                }
                else if (strcmp("WRAP_16_BYTE", optarg) == 0)
                {
                    wrap = W25QXX_BURST_WRAP_16_BYTE;
                }
                else if (strcmp("WRAP_32_BYTE", optarg) == 0)
                {
                    wrap = W25QXX_BURST_WRAP_32_BYTE;
                }
                else if (strcmp("WRAP_64_BYTE", optarg) == 0)
                {
                    wrap = W25QXX_BURST_WRAP_64_BYTE;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* the end */
            case -1 :
            {
                break;
            }

            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* run reg test */
        res = w25qxx_register_test(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_read", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* run read test */
        res = w25qxx_read_test(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("e_power-down", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* basic init */
        res = w25qxx_basic_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* power down */
        res = w25qxx_basic_power_down();
        if (res != 0)
        {
            (void)w25qxx_basic_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: power down successful.\n");
        }

        /* basic deinit */
        (void)w25qxx_basic_deinit();

        return 0;
    }
    else if (strcmp("e_wake-up", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* basic init */
        res = w25qxx_basic_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* wake up */
        res = w25qxx_basic_wake_up();
        if (res != 0)
        {
            (void)w25qxx_basic_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: wake up successful.\n");
        }

        /* basic deinit */
        (void)w25qxx_basic_deinit();

        return 0;
    }
    else if (strcmp("e_chip-erase", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* basic init */
        res = w25qxx_basic_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* output */
        w25qxx_interface_debug_print("w25qxx: start chip erase.\n");

        /* chip erase */
        res = w25qxx_basic_chip_erase();
        if (res != 0)
        {
            (void)w25qxx_basic_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: chip erase successful.\n");
        }

        /* basic deinit */
        (void)w25qxx_basic_deinit();

        return 0;
    }
    else if (strcmp("e_get-id", type) == 0)
    {
        uint8_t res;
        uint8_t manufacturer;
        uint8_t device_id;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* basic init */
        res = w25qxx_basic_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* get id */
        res = w25qxx_basic_get_id((uint8_t *)&manufacturer, (uint8_t *)&device_id);
        if (res != 0)
        {
            (void)w25qxx_basic_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X device id is 0x%02X.\n",
                                         manufacturer, device_id);
        }

        /* basic deinit */
        (void)w25qxx_basic_deinit();

        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint8_t dat;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* basic init */
        res = w25qxx_basic_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* read */
        res = w25qxx_basic_read(addr, (uint8_t *)&dat, 1);
        if (res != 0)
        {
            (void)w25qxx_basic_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: addr %d is %d.\n", addr, dat);
        }

        /* basic deinit */
        (void)w25qxx_basic_deinit();

        return 0;
    }
    else if (strcmp("e_write", type) == 0)
    {
        uint8_t res;
        uint8_t dat;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* basic init */
        res = w25qxx_basic_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* write */
        dat = data & 0xFF;
        res = w25qxx_basic_write(addr, (uint8_t *)&dat, 1);
        if (res != 0)
        {
            (void)w25qxx_basic_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: addr %d write data %d.\n", addr, dat);
        }

        /* basic deinit */
        (void)w25qxx_basic_deinit();

        return 0;
    }
    else if (strcmp("e_advance-power-down", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* power down */
        res = w25qxx_advance_power_down();
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: power down successful.\n");
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-wake-up", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* wake up */
        res = w25qxx_advance_wake_up();
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: wake up successful.\n");
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-chip-erase", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* output */
        w25qxx_interface_debug_print("w25qxx: start chip erase.\n");

        /* chip erase */
        res = w25qxx_advance_chip_erase();
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: chip erase successful.\n");
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-get-id", type) == 0)
    {
        uint8_t res;
        uint8_t manufacturer;
        uint8_t device_id;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* get id */
        res = w25qxx_advance_get_id((uint8_t *)&manufacturer, (uint8_t *)&device_id);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X device id is 0x%02X.\n",
                                         manufacturer, device_id);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-read", type) == 0)
    {
        uint8_t res;
        uint8_t dat;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* read */
        res = w25qxx_advance_read(addr, (uint8_t *)&dat, 1);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: addr %d is %d.\n", addr, dat);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-write", type) == 0)
    {
        uint8_t res;
        uint8_t dat;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance write */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* write */
        dat = data & 0xFF;
        res = w25qxx_advance_write(addr, (uint8_t *)&dat, 1);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: addr %d write data %d.\n", addr, dat);
        }

        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-page-program", type) == 0)
    {
        uint8_t res;
        uint8_t dat;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* page program */
        dat = data & 0xFF;
        res = w25qxx_advance_page_program(addr, (uint8_t *)&dat, 1);
        if (res != 0)
        {
            (void)w25qxx_basic_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: addr %d page program data %d.\n", addr, dat);
        }

        (void)w25qxx_basic_deinit();

        return 0;
    }
    else if (strcmp("e_advance-erase-4k", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* erase 4k */
        res = w25qxx_advance_sector_erase_4k(addr);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: erase 4k addr %d successful.\n", addr);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-erase-32k", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* erase 32k */
        res = w25qxx_advance_block_erase_32k(addr);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: erase 32k addr %d successful.\n", addr);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-erase-64k", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* erase 64k */
        res = w25qxx_advance_block_erase_64k(addr);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: erase 64k addr %d successful.\n", addr);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-fast-read", type) == 0)
    {
        uint8_t res;
        uint8_t dat;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* fast read */
        res = w25qxx_advance_fast_read(addr, (uint8_t *)&dat, 1);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: addr %d is %d.\n", addr, dat);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-get-status1", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* get status1 */
        res = w25qxx_advance_get_status1((uint8_t *)&status);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: get status1 is 0x%02X.\n", status);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-get-status2", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* get status2 */
        res = w25qxx_advance_get_status2((uint8_t *)&status);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: get status2 is 0x%02X.\n", status);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-get-status3", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* get status3 */
        res = w25qxx_advance_get_status3((uint8_t *)&status);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: get status3 is 0x%02X.\n", status);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-set-status1", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* set status1 */
        res = w25qxx_advance_set_status1(status);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: set status1 0x%02X.\n", status);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-set-status2", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* set status2 */
        res = w25qxx_advance_set_status2(status);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: set status2 0x%02X.\n", status);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-set-status3", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* set status3 */
        res = w25qxx_advance_set_status3(status);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: set status3 0x%02X.\n", status);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-get-jedec-id", type) == 0)
    {
        uint8_t res;
        uint8_t manufacturer;
        uint8_t device_id[2];

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* get jedec id */
        res = w25qxx_advance_get_get_jedec_id((uint8_t *)&manufacturer, (uint8_t *)device_id);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X device id is 0x%02X 0x%02X.\n",
                                         manufacturer, device_id[0], device_id[1]);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-global-lock", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* global lock */
        res = w25qxx_advance_global_block_lock();
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: global block lock successful.\n");
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-global-unlock", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* global unlock */
        res = w25qxx_advance_global_block_unlock();
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: global block unlock successful.\n");
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-block-lock", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* block lock */
        res = w25qxx_advance_individual_block_lock(addr);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: block lock addr %d.\n", addr);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-block-unlock", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* block unlock */
        res = w25qxx_advance_individual_block_unlock(addr);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: block unlock addr %d.\n", addr);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-read-block", type) == 0)
    {
        uint8_t res;
        uint8_t value;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* read block */
        res = w25qxx_advance_read_block_lock(addr, (uint8_t *)&value);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: read block addr %d is %d.\n", addr, value);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-reset", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* reset */
        res = w25qxx_advance_reset();
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: reset successful.\n");
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-spi-read", type) == 0)
    {
        uint8_t res;
        uint8_t dat;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* spi read */
        res = w25qxx_advance_only_spi_read(addr, (uint8_t *)&dat, 1);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: add %d spi read is 0x%02X.\n", addr, dat);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-spi-dual-output-read", type) == 0)
    {
        w25qxx_interface_debug_print("w25qxx: this chip can't use this function.\n");

        return 5;
    }
    else if (strcmp("e_advance-spi-quad-output-read", type) == 0)
    {
        w25qxx_interface_debug_print("w25qxx: this chip can't use this function.\n");

        return 5;
    }
    else if (strcmp("e_advance-spi-dual-io-read", type) == 0)
    {
        w25qxx_interface_debug_print("w25qxx: this chip can't use this function.\n");

        return 5;
    }
    else if (strcmp("e_advance-spi-quad-io-read", type) == 0)
    {
        w25qxx_interface_debug_print("w25qxx: this chip can't use this function.\n");

        return 5;
    }
    else if (strcmp("e_advance-spi-word-quad-io-read", type) == 0)
    {
        w25qxx_interface_debug_print("w25qxx: this chip can't use this function.\n");

        return 5;
    }
    else if (strcmp("e_advance-spi-octal-word-quad-io-read", type) == 0)
    {
        w25qxx_interface_debug_print("w25qxx: this chip can't use this function.\n");

        return 5;
    }
    else if (strcmp("e_advance-spi-read-security", type) == 0)
    {
        uint8_t res;
        uint8_t dat[256];

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* read security */
        res = w25qxx_advance_only_spi_read_security_register(num, (uint8_t *)dat);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: read %d security register.\n", num);
            for (uint16_t i = 0; i < 256; i += 8)
            {
                w25qxx_interface_debug_print("w25qxx: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\n",
                                             dat[i + 0], dat[i + 1], dat[i + 2], dat[i + 3],
                                             dat[i + 4], dat[i + 5], dat[i + 6], dat[i + 7]
                                            );
            }
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-spi-get-id-dual-io", type) == 0)
    {
        w25qxx_interface_debug_print("w25qxx: this chip can't use this function.\n");

        return 5;
    }
    else if (strcmp("e_advance-spi-get-id-quad-io", type) == 0)
    {
        w25qxx_interface_debug_print("w25qxx: this chip can't use this function.\n");

        return 5;
    }
    else if (strcmp("e_advance-spi-get-sfdp", type) == 0)
    {
        uint8_t res;
        uint8_t sfdp[256];

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* get sfdp */
        res = w25qxx_advance_only_spi_get_sfdp((uint8_t *)sfdp);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }

        /* output*/
        for (uint16_t j = 0; j < 256; j += 8)
        {
            w25qxx_interface_debug_print("w25qxx: sdfp[%d-%d] is 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\n",
                                         j , j + 7,
                                         sfdp[j + 0], sfdp[j + 1], sfdp[j + 2],
                                         sfdp[j + 3], sfdp[j + 4], sfdp[j + 5],
                                         sfdp[j + 6], sfdp[j + 7]);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-spi-page-program-quad-input", type) == 0)
    {
        w25qxx_interface_debug_print("w25qxx: this chip can't use this function.\n");

        return 5;
    }
    else if (strcmp("e_advance-spi-write-security", type) == 0)
    {
        uint8_t res;
        uint8_t buf[256];

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* write security */
        memset((uint8_t *)buf, data, 256);
        res = w25qxx_advance_only_spi_write_security_register(num, (uint8_t *)buf);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: write %d security register successful.\n", num);
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-qspi-set-read-parameters", type) == 0)
    {
        w25qxx_interface_debug_print("w25qxx: this chip can't use this function.\n");

        return 5;
    }
    else if (strcmp("e_advance-spi-set-burst", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* advance init */
        res = w25qxx_advance_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* set burst */
        res = w25qxx_advance_only_spi_set_burst_with_wrap(wrap);
        if (res != 0)
        {
            (void)w25qxx_advance_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: set burst with wrap successful.\n");
        }

        /* advance deinit */
        (void)w25qxx_advance_deinit();

        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        w25qxx_interface_debug_print("Usage:\n");
        w25qxx_interface_debug_print("  w25qxx (-i | --information)\n");
        w25qxx_interface_debug_print("  w25qxx (-h | --help)\n");
        w25qxx_interface_debug_print("  w25qxx (-p | --port)\n");
        w25qxx_interface_debug_print("  w25qxx (-t reg | --test=reg) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-t read | --test=read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e power-down | --example=power-down) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e wake-up | --example=wake-up) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e chip-erase | --example=chip-erase) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e get-id | --example=get-id) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e read | --example=read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e write | --example=write) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address> --data=<hex>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-power-down | --example=advance-power-down) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-wake-up | --example=advance-wake-up) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-chip-erase | --example=advance-chip-erase) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-get-id | --example=advance-get-id) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-read | --example=advance-read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-write | --example=advance-write) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address> --data=<hex>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-page-program | --example=advance-page-program) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address> --data=<hex>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-erase-4k | --example=advance-erase-4k) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-erase-32k | --example=advance-erase-32k) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-erase-64k | --example=advance-erase-64k) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-fast-read | --example=advance-fast-read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-get-status1 | --example=advance-get-status1) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-get-status2 | --example=advance-get-status2) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-get-status3 | --example=advance-get-status3) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-set-status1 | --example=advance-set-status1) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --status=<hex>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-set-status2 | --example=advance-set-status2) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --status=<hex>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-set-status3 | --example=advance-set-status3) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --status=<hex>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-get-jedec-id | --example=advance-get-jedec-id) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-global-lock | --example=advance-global-lock) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-global-unlock | --example=advance-global-unlock) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-block-lock | --example=advance-block-lock) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-block-unlock | --example=advance-block-unlock) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-read-block | --example=advance-read-block) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-reset | --example=advance-reset) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-read | --example=advance-spi-read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-dual-output-read | --example=advance-spi-dual-output-read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-quad-output-read | --example=advance-spi-quad-output-read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-dual-io-read | --example=advance-spi-dual-io-read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-quad-io-read | --example=advance-spi-quad-io-read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-word-quad-io-read | --example=advance-spi-word-quad-io-read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-octal-word-quad-io-read | --example=advance-spi-octal-word-quad-io-read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-page-program-quad-input | --example=advance-spi-page-program-quad-input) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --addr=<address> --data=<hex>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-get-id-dual-io | --example=advance-spi-get-id-dual-io) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-get-id-quad-io | --example=advance-spi-get-id-quad-io) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-get-sfdp | --example=advance-spi-get-sfdp) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-write-security | --example=advance-spi-write-security) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --num=<1 | 2 | 3> --data=<hex>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-read-security | --example=advance-spi-read-security) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --num=<1 | 2 | 3>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-qspi-set-read-parameters | --example=advance-qspi-set-read-parameters) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --dummy=<DUMMY_2_33MHZ | DUMMY_4_55MHZ | DUMMY_6_80MHZ | DUMMY_8_80MHZ> --length=<8 | 16 | 32 | 64>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-set-burst | --example=advance-spi-set-burst) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --wrap=<WRAP_NONE | WRAP_8_BYTE | WRAP_16_BYTE | WRAP_32_BYTE | WRAP_64_BYTE>\n");
        w25qxx_interface_debug_print("\n");
        w25qxx_interface_debug_print("Options:\n");
        w25qxx_interface_debug_print("      --addr=<address>               Set the operator address and it is hexadecimal.([default: 0x00000000])\n");
        w25qxx_interface_debug_print("      --data=<hex>                   Set the input data and it is hexadecimal.([default: 0x00000000])\n");
        w25qxx_interface_debug_print("      --dummy=<DUMMY_2_33MHZ | DUMMY_4_55MHZ | DUMMY_6_80MHZ | DUMMY_8_80MHZ>\n");
        w25qxx_interface_debug_print("                                     Set the dummy.([default: DUMMY_8_80MHZ])\n");
        w25qxx_interface_debug_print("  -e <power-down | wake-up | chip-erase | get-id | read | write | advance-power-down | advance-wake-up | advance-chip-erase\n");
        w25qxx_interface_debug_print("     | advance-get-id | advance-read | advance-write | advance-page-program | advance-erase-4k | advance-erase-32k | advance-erase-64k\n");
        w25qxx_interface_debug_print("     | advance-fast-read | advance-get-status1 | advance-get-status2 | advance-get-status3 | advance-set-status1 | advance-set-status2\n");
        w25qxx_interface_debug_print("     | advance-set-status3 | advance-get-jedec-id | advance-global-lock | advance-global-unlock | advance-block-lock | advance-block-unlock\n");
        w25qxx_interface_debug_print("     | advance-read-block | advance-reset | advance-spi-read | advance-spi-dual-output-read | advance-spi-quad-output-read\n");
        w25qxx_interface_debug_print("     | advance-spi-dual-io-read | advance-spi-quad-io-read | advance-spi-word-quad-io-read | advance-spi-octal-word-quad-io-read\n");
        w25qxx_interface_debug_print("     | advance-spi-page-program-quad-input | advance-spi-get-id-dual-io | advance-spi-get-id-quad-io | advance-spi-get-sfdp\n");
        w25qxx_interface_debug_print("     | advance-spi-write-security | advance-spi-read-security | advance-qspi-set-read-parameters | advance-spi-set-burst>, --example=<power-down\n");
        w25qxx_interface_debug_print("     | wake-up | chip-erase | get-id | read | write | advance-power-down | advance-wake-up | advance-chip-erase\n");
        w25qxx_interface_debug_print("     | advance-get-id | advance-read | advance-write | advance-page-program | advance-erase-4k | advance-erase-32k | advance-erase-64k\n");
        w25qxx_interface_debug_print("     | advance-fast-read | advance-get-status1 | advance-get-status2 | advance-get-status3 | advance-set-status1 | advance-set-status2\n");
        w25qxx_interface_debug_print("     | advance-set-status3 | advance-get-jedec-id | advance-global-lock | advance-global-unlock | advance-block-lock | advance-block-unlock\n");
        w25qxx_interface_debug_print("     | advance-read-block | advance-reset | advance-spi-read | advance-spi-dual-output-read | advance-spi-quad-output-read\n");
        w25qxx_interface_debug_print("     | advance-spi-dual-io-read | advance-spi-quad-io-read | advance-spi-word-quad-io-read | advance-spi-octal-word-quad-io-read\n");
        w25qxx_interface_debug_print("     | advance-spi-page-program-quad-input | advance-spi-get-id-dual-io | advance-spi-get-id-quad-io | advance-spi-get-sfdp\n");
        w25qxx_interface_debug_print("     | advance-spi-write-security | advance-spi-read-security | advance-qspi-set-read-parameters | advance-spi-set-burst>\n");
        w25qxx_interface_debug_print("                                     Run the driver example.\n");
        w25qxx_interface_debug_print("  -h, --help                         Show the help.\n");
        w25qxx_interface_debug_print("      --length=<8 | 16 | 32 | 64>    Set the dummy length.([default: 8])\n");
        w25qxx_interface_debug_print("  -i, --information                  Show the chip information.\n");
        w25qxx_interface_debug_print("      --interface=<spi | qspi>       Set the chip interface.([default: spi])\n");
        w25qxx_interface_debug_print("      --num=<1 | 2 | 3>              Set the security number.([default: 1])\n");
        w25qxx_interface_debug_print("  -p, --port                         Display the pin connections of the current board.\n");
        w25qxx_interface_debug_print("      --status=<hex>                 Set the status and it is hexadecimal.([default: 0x00])\n");
        w25qxx_interface_debug_print("  -t <reg | read>, --test=<reg | read>\n");
        w25qxx_interface_debug_print("                                     Run the driver test.\n");
        w25qxx_interface_debug_print("      --type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>\n");
        w25qxx_interface_debug_print("                                     Set the chip type.([default: W25Q128])\n");
        w25qxx_interface_debug_print("      --wrap=<WRAP_NONE | WRAP_8_BYTE | WRAP_16_BYTE | WRAP_32_BYTE | WRAP_64_BYTE>\n");
        w25qxx_interface_debug_print("                                     Set the wrap.([default: WRAP_8_BYTE])\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        w25qxx_info_t info;

        /* print w25qxx info */
        w25qxx_info(&info);
        w25qxx_interface_debug_print("w25qxx: chip is %s.\n", info.chip_name);
        w25qxx_interface_debug_print("w25qxx: manufacturer is %s.\n", info.manufacturer_name);
        w25qxx_interface_debug_print("w25qxx: interface is %s.\n", info.interface);
        w25qxx_interface_debug_print("w25qxx: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        w25qxx_interface_debug_print("w25qxx: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        w25qxx_interface_debug_print("w25qxx: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        w25qxx_interface_debug_print("w25qxx: max current is %0.2fmA.\n", info.max_current_ma);
        w25qxx_interface_debug_print("w25qxx: max temperature is %0.1fC.\n", info.temperature_max);
        w25qxx_interface_debug_print("w25qxx: min temperature is %0.1fC.\n", info.temperature_min);

        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        w25qxx_interface_debug_print("w25qxx: SPI interface SCK connected to GPIOA PIN5.\n");
        w25qxx_interface_debug_print("w25qxx: SPI interface MISO connected to GPIOA PIN6.\n");
        w25qxx_interface_debug_print("w25qxx: SPI interface MOSI connected to GPIOA PIN7.\n");
        w25qxx_interface_debug_print("w25qxx: SPI interface CS connected to GPIOA PIN4.\n");

        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;

    /* stm32f407 clock init and hal init */
    clock_init();

    /* delay init */
    delay_init();

    /* uart init */
    uart_init(115200);

    /* shell init && register w25qxx function */
    shell_init();
    shell_register("w25qxx", w25qxx);
    uart_print("w25qxx: welcome to libdriver w25qxx.\n");

    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("w25qxx: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("w25qxx: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("w25qxx: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("w25qxx: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("w25qxx: param is invalid.\n");
            }
            else
            {
                uart_print("w25qxx: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
