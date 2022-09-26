### 1. Chip

#### 1.1 Chip Info

chip name : Raspberry Pi 4B.

spi pin: SCLK/MOSI/MISO/CS GPIO11/GPIO10/GPIO9/GPIO8.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(w25qxx REQUIRED)
```

### 3. W25QXX

#### 3.1 Command Instruction

​          w25qxx is a basic command which can test all w25qxx driver function:

​           -i        show w25qxx chip and driver information.

​           -h       show w25qxx help.

​           -p       show w25qxx pin connections of the current board.

​           -t (reg -type <type> (-spi| -qspi) | read -type <type> (-spi | -qspi)) 

​           -t reg -type <type> (-spi | -qspi)       run w25qxx register test and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -t read -type <type> (-spi | -qspi)        run w25qxx read test and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c (basic -type <type> power_down (-spi| -qspi) | basic -type <type> wake_up (-spi| -qspi) | basic -type <type> chip_erase (-spi| -qspi) | basic -type <type> get_id (-spi| -qspi) | basic -type <type> read <addr> (-spi| -qspi)  | basic -type <type> write <addr> <data> (-spi| -qspi) | advance -type <type> power_down (-spi| -qspi) | advance -type <type> wake_up (-spi| -qspi) | advance -type <type> chip_erase (-spi| -qspi) | advance -type <type> get_id (-spi| -qspi) | advance -type <type> read <addr> (-spi| -qspi)  | advance -type <type> write <addr> <data> (-spi| -qspi) | advance -type <type> page_program <addr> <data> (-spi| -qspi) | advance -type <type> erase_4k <addr> (-spi| -qspi) | advance -type <type> erase_32k <addr> (-spi| -qspi) | advance -type <type>  erase_64k <addr> (-spi| -qspi) | advance -type <type> fast_read <addr> (-spi| -qspi)  | advance -type <type> get_status1 (-spi| -qspi) | advance -type <type> get_status2 (-spi| -qspi) |  advance -type <type> get_status3 (-spi| -qspi) | advance -type <type> set_status1 <status> (-spi| -qspi) | advance -type <type> set_status2 <status> (-spi| -qspi) | advance -type <type>  set_status3 <status> (-spi| -qspi) | advance -type <type> get_jedec_id (-spi| -qspi) | advance -type <type> global_lock (-spi| -qspi) | advance -type <type> global_unlock (-spi| -qspi) |  advance -type <type> block_lock <addr> (-spi| -qspi) | advance -type <type> block_unlock <addr> (-spi| -qspi) | advance -type <type> read_block <addr> (-spi| -qspi) | advance -type <type> reset (-spi| -qspi) | advance -type <type> spi_read <addr> | advance  -type <type> spi_dual_output_read <addr> | advance -type <type> spi_quad_output_read <addr> | advance -type <type> spi_dual_io_read <addr> | advance -type <type>  spi_quad_io_read <addr> | advance -type <type> spi_word_quad_io_read <addr> | advance -type <type>   spi_octal_word_quad_io_read <addr> | advance -type <type> spi_page_program_quad_input <addr>  <data>| advance -type <type>   spi_get_id_dual_io | advance -type <type> spi_get_id_quad_io | advance -type <type> spi_get_sfdp |  advance -type <type>   spi_write_security_reg <num> <data> |   advance -type <type> spi_read_security_reg <num> | advance -type <type> qspi_set_read_parameters <dummy> <length> | advance -type <type>  spi_set_burst <wrap>)

​           -c basic -type <type> power_down (-spi| -qspi)        run w25qxx basic power down function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c basic -type <type> wake_up (-spi| -qspi)        run w25qxx basic wake up function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c basic -type <type> chip_erase (-spi| -qspi)        run w25qxx basic chip erase function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c basic -type <type> get_id (-spi| -qspi)        run w25qxx basic get id function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c basic -type <type> read <addr> (-spi| -qspi)        run w25qxx basic read function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.

​           -c basic -type <type> write <addr> <data> (-spi| -qspi)        run w25qxx basic write function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.

​           -c advance -type <type> power_down (-spi| -qspi)        run w25qxx advance power down function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c advance -type <type> wake_up (-spi| -qspi)        run w25qxx advance wake up function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c advance -type <type> chip_erase (-spi| -qspi)        run w25qxx advance chip erase function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c advance -type <type> get_id (-spi| -qspi)        run w25qxx advance get id function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c advance -type <type> read <addr> (-spi| -qspi)        run w25qxx advance read function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.

​           -c advance -type <type> write <addr> <data> (-spi| -qspi)        run w25qxx advance write function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address. data is the written data.

​           -c advance -type <type> page_program <addr> <data> (-spi| -qspi)        run w25qxx advance page program function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.data is the written data.

​           -c advance -type <type> erase_4k <addr> (-spi| -qspi)        run w25qxx advance erase 4k function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.

​           -c advance -type <type> erase_32k <addr> (-spi| -qspi)        run w25qxx advance erase 32k function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.

​           -c advance -type <type> erase_64k <addr> (-spi| -qspi)        run w25qxx advance erase 64k function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.

​           -c advance -type <type> fast_read <addr> (-spi| -qspi)        run w25qxx advance fast read function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address. 

​           -c advance -type <type> get_status1 (-spi| -qspi)         run w25qxx advance get status1 function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c advance -type <type> get_status2 (-spi| -qspi)          run w25qxx advance get status2 function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c advance -type <type> get_status3 (-spi| -qspi)          run w25qxx advance get status3 function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c advance -type <type> set_status1 <status> (-spi| -qspi)          run w25qxx advance set status1 function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.status is the set status.

​           -c advance -type <type> set_status2 <status> (-spi| -qspi)          run w25qxx advance set status2 function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.status is the set status.

​           -c advance -type <type> set_status3 <status> (-spi| -qspi)          run w25qxx advance set status3 function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.status is the set status.

​           -c advance -type <type> get_jedec_id (-spi| -qspi)         run w25qxx advance get jedec id function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c advance -type <type> global_lock (-spi| -qspi)          run w25qxx advance global lock function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c advance -type <type> global_unlock (-spi| -qspi)          run w25qxx advance global unlock function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c advance -type <type> block_lock <addr> (-spi| -qspi)        run w25qxx advance block lock function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.

​           -c advance -type <type> block_unlock <addr> (-spi| -qspi)        run w25qxx advance block unlock function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.

​           -c advance -type <type> read_block <addr> (-spi| -qspi)         run w25qxx advance read block function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.

​           -c advance -type <type> reset (-spi| -qspi)        run w25qxx advance reset function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c advance -type <type> spi_read <addr>        run w25qxx advance spi read function.  type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.

​           -c advance -type <type> spi_dual_output_read <addr>         run w25qxx advance spi dual output read function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.

​           -c advance -type <type> spi_quad_output_read <addr>        run w25qxx advance spi quad output read function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address. 

​           -c advance -type <type> spi_dual_io_read <addr>        run w25qxx advance spi dual io read function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.  

​           -c advance -type <type> spi_quad_io_read <addr>       run w25qxx advance spi quad io read function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.  

​           -c advance -type <type> spi_word_quad_io_read <addr>       run w25qxx advance spi word quad io read function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.   

​           -c advance -type <type> spi_octal_word_quad_io_read <addr>       run w25qxx advance spi octal word quad io read function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.    

​           -c advance -type <type> spi_page_program_quad_input <addr> <data>      run w25qxx advance page program quad input function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address. data is the written data.   

​           -c advance -type <type> spi_get_id_dual_io        run w25qxx advance get id dual io function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c advance -type <type> spi_get_id_quad_io        run w25qxx advance get id quad io function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c advance -type <type> spi_get_sfdp        run w25qxx advance get sfdp function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.

​           -c advance -type <type> spi_write_security_reg <num> <data>        run w25qxx advance write security register function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.num is the security register number. data is the written data.    

​           -c advance -type <type> spi_read_security_reg <num>         run w25qxx advance read security register function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.num is the security register number. num can be 1, 2, 3.

​           -c advance -type <type> qspi_set_read_parameters <dummy> <length>         run w25qxx advance set read parameters function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.dummy is the read dummy and it can be DUMMY_2_33MHZ, DUMMY_4_55MHZ, DUMMY_6_80MHZ, DUMMY_8_80MHZ. length is the wrap length and it can be 8, 16, 32 or 64.

​           -c advance -type <type>  spi_set_burst <wrap>        run w25qxx advance set burst function. type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.wrap is the burst wrap and it can be WRAP_NONE,WRAP_8_BYTE,WRAP_16_BYTE,WRAP_32_BYTE,WRAP_64_BYTE.

#### 3.2 Command Example

```shell
./w25qxx -i

w25qxx: chip is Winbond W25QXX.
w25qxx: manufacturer is Winbond.
w25qxx: interface is SPI QSPI.
w25qxx: driver version is 1.0.
w25qxx: min supply voltage is 2.7V.
w25qxx: max supply voltage is 3.6V.
w25qxx: max current is 25.00mA.
w25qxx: max temperature is 85.0C.
w25qxx: min temperature is -40.0C.
```

```shell
./w25qxx -p

w25qxx: SPI interface SCK connected to GPIO11(BCM).
w25qxx: SPI interface MISO connected to GPIO9(BCM).
w25qxx: SPI interface MOSI connected to GPIO10(BCM).
w25qxx: SPI interface CS connected to GPIO8(BCM).
```

```shell
./w25qxx -t reg -type W25Q128 -spi

w25qxx: chip is Winbond W25QXX.
w25qxx: manufacturer is Winbond.
w25qxx: interface is SPI QSPI.
w25qxx: driver version is 1.0.
w25qxx: min supply voltage is 2.7V.
w25qxx: max supply voltage is 3.6V.
w25qxx: max current is 25.00mA.
w25qxx: max temperature is 85.0C.
w25qxx: min temperature is -40.0C.
w25qxx: start register test.
w25qxx: w25qxx_set_type/w25qxx_get_type test.
w25qxx: set type W25Q80.
w25qxx: check chip type ok.
w25qxx: set type W25Q16.
w25qxx: check chip type ok.
w25qxx: set type W25Q32.
w25qxx: check chip type ok.
w25qxx: set type W25Q64.
w25qxx: check chip type ok.
w25qxx: set type W25Q128.
w25qxx: check chip type ok.
w25qxx: set type W25Q256.
w25qxx: check chip type ok.
w25qxx: w25qxx_set_interface/w25qxx_get_interface test.
w25qxx: set interface SPI.
w25qxx: check chip interface ok.
w25qxx: set interface QSPI.
w25qxx: check chip interface ok.
w25qxx: w25qxx_get_manufacturer_device_id test.
w25qxx: manufacturer is 0xEF, device id is 0x17.
w25qxx: w25qxx_get_jedec_id test.
w25qxx: manufacturer is 0xEF, device id is 0x40 0x18.
w25qxx: w25qxx_get_unique_id test.
w25qxx: unique id 0xD2 0x66 0x78 0xB2 0x5F 0x26 0x2D 0x24.
w25qxx: w25qxx_set_status1/w25qxx_get_status1 test.
w25qxx: status1 is 0x00.
w25qxx: w25qxx_set_status2/w25qxx_get_status2 test.
w25qxx: status2 is 0x02.
w25qxx: w25qxx_set_status3/w25qxx_get_status3 test.
w25qxx: status3 is 0x60.
w25qxx: w25qxx_enable_write test.
w25qxx: check enable write ok.
w25qxx: w25qxx_disable_write test.
w25qxx: check disable write ok.
w25qxx: w25qxx_enable_write test.
w25qxx: check enable volatile sr write ok.
w25qxx: w25qxx_erase_program_suspend test.
w25qxx: check erase program suspend ok.
w25qxx: w25qxx_erase_program_suspend test.
w25qxx: check erase program resume ok.
w25qxx: w25qxx_global_block_lock test.
w25qxx: check global block lock ok.
w25qxx: w25qxx_global_block_unlock test.
w25qxx: check global block unlock ok.
w25qxx: w25qxx_individual_block_lock test.
w25qxx: check individual block lock ok.
w25qxx: w25qxx_read_block_lock test.
w25qxx: check read block lock ok with 1.
w25qxx: w25qxx_individual_block_unlock test.
w25qxx: check individual block unlock ok.
w25qxx: w25qxx_set_burst_with_wrap test.
w25qxx: check set burst with wrap ok.
w25qxx: w25qxx_power_down test.
w25qxx: w25qxx_release_power_down test.
w25qxx: w25qxx_enable_reset test.
w25qxx: w25qxx_reset_device test.
w25qxx: finish register test.
```

```shell
./w25qxx -t read -type W25Q128 -spi

w25qxx: chip is Winbond W25QXX.
w25qxx: manufacturer is Winbond.
w25qxx: interface is SPI QSPI.
w25qxx: driver version is 1.0.
w25qxx: min supply voltage is 2.7V.
w25qxx: max supply voltage is 3.6V.
w25qxx: max current is 25.00mA.
w25qxx: max temperature is 85.0C.
w25qxx: min temperature is -40.0C.
w25qxx: start read test.
w25qxx: w25qxx_write/w25qxx_read test.
w25qxx: 0x00000000/0x01000000 successful.
w25qxx: 0x00100000/0x01000000 successful.
w25qxx: 0x00200000/0x01000000 successful.
w25qxx: 0x00300000/0x01000000 successful.
w25qxx: 0x00400000/0x01000000 successful.
w25qxx: 0x00500000/0x01000000 successful.
w25qxx: 0x00600000/0x01000000 successful.
w25qxx: 0x00700000/0x01000000 successful.
w25qxx: 0x00800000/0x01000000 successful.
w25qxx: 0x00900000/0x01000000 successful.
w25qxx: 0x00A00000/0x01000000 successful.
w25qxx: 0x00B00000/0x01000000 successful.
w25qxx: 0x00C00000/0x01000000 successful.
w25qxx: 0x00D00000/0x01000000 successful.
w25qxx: 0x00E00000/0x01000000 successful.
w25qxx: 0x00F00000/0x01000000 successful.
w25qxx: w25qxx_sector_erase_4k test with address 0x4000.
w25qxx: only spi read test passed.
w25qxx: fast read test passed.
w25qxx: w25qxx_block_erase_32k test with address 0x18000.
w25qxx: only spi read test passed.
w25qxx: fast read test passed.
w25qxx: w25qxx_block_erase_64k test with address 0x80000.
w25qxx: only spi read test passed.
w25qxx: fast read test passed.
w25qxx: get sfdp.
w25qxx: sdfp[0-7] is 0xC5 0xF7 0xDB 0x02 0x52 0x43 0xE1 0x94.
w25qxx: sdfp[8-15] is 0xDA 0x6C 0x5C 0x25 0xDE 0xE7 0xB3 0x51.
w25qxx: sdfp[16-23] is 0x49 0xE2 0xDB 0x68 0x16 0x0F 0x3D 0xA3.
w25qxx: sdfp[24-31] is 0x32 0xB8 0xE0 0xA3 0x41 0xD1 0x3C 0x65.
w25qxx: sdfp[32-39] is 0x51 0x25 0x55 0x34 0x76 0x5C 0x4A 0x6E.
w25qxx: sdfp[40-47] is 0x24 0x68 0x9F 0x7C 0x07 0xC7 0x40 0xD1.
w25qxx: sdfp[48-55] is 0x40 0x74 0x41 0xC6 0x11 0xC0 0x64 0x2A.
w25qxx: sdfp[56-63] is 0x48 0x00 0x57 0x86 0xB9 0x3D 0xDE 0x48.
w25qxx: sdfp[64-71] is 0x90 0xC4 0xC4 0x5A 0xEE 0x7A 0x91 0x1A.
w25qxx: sdfp[72-79] is 0x22 0x4F 0xA9 0xDC 0x20 0xFD 0x07 0x5C.
w25qxx: sdfp[80-87] is 0x00 0xE0 0xFA 0xC7 0x8A 0x79 0x43 0x99.
w25qxx: sdfp[88-95] is 0xB5 0x19 0xF8 0xD0 0x4A 0xC4 0xFF 0x3E.
w25qxx: sdfp[96-103] is 0x8A 0xEE 0x25 0xE3 0xE7 0x3D 0xD8 0x9C.
w25qxx: sdfp[104-111] is 0x74 0x21 0xC0 0xD8 0x4A 0xDD 0x6D 0xE1.
w25qxx: sdfp[112-119] is 0xB5 0x70 0x7E 0x89 0x87 0xA2 0x47 0xCE.
w25qxx: sdfp[120-127] is 0x4E 0xB2 0x7F 0xD1 0x61 0xCE 0xF2 0xBE.
w25qxx: sdfp[128-135] is 0xC3 0xCA 0x9C 0xF8 0x47 0xE4 0xC9 0xE1.
w25qxx: sdfp[136-143] is 0x95 0x6A 0x45 0x13 0x00 0xE5 0xE0 0x83.
w25qxx: sdfp[144-151] is 0x67 0xAA 0x4F 0x1B 0x25 0xCD 0x30 0x48.
w25qxx: sdfp[152-159] is 0xB1 0xEF 0x7F 0xDF 0x84 0xBC 0x65 0x55.
w25qxx: sdfp[160-167] is 0xB6 0x2A 0x1D 0x5D 0xDD 0x52 0xC1 0x38.
w25qxx: sdfp[168-175] is 0xD7 0x28 0xD8 0xA2 0xC7 0x14 0xFE 0x96.
w25qxx: sdfp[176-183] is 0x63 0x6E 0x50 0x40 0x52 0xAE 0x23 0x18.
w25qxx: sdfp[184-191] is 0x80 0xC6 0x15 0xA4 0xC1 0x1B 0xA2 0xCD.
w25qxx: sdfp[192-199] is 0x41 0x6D 0xEB 0xA2 0xAC 0xF4 0x81 0xFD.
w25qxx: sdfp[200-207] is 0x0D 0xBD 0x6B 0x65 0x1F 0xDE 0x6B 0xC9.
w25qxx: sdfp[208-215] is 0x6F 0x45 0xF4 0x28 0x7D 0x80 0xF7 0x83.
w25qxx: sdfp[216-223] is 0x6B 0x8A 0x48 0x7F 0xFE 0xB5 0xB9 0xD4.
w25qxx: sdfp[224-231] is 0x35 0x95 0x0D 0x2C 0x33 0xDC 0x01 0x2C.
w25qxx: sdfp[232-239] is 0xDD 0x95 0x34 0x7A 0x2B 0xA3 0x0F 0x03.
w25qxx: sdfp[240-247] is 0x31 0x9F 0xEC 0x40 0x19 0x57 0x86 0x15.
w25qxx: sdfp[248-255] is 0xA2 0x80 0xAF 0xD8 0x27 0x5D 0xFE 0x39.
w25qxx: security register1 write and read test.
w25qxx: security register1 check passed.
w25qxx: security register2 write and read test.
w25qxx: security register2 check passed.
w25qxx: security register3 write and read test.
w25qxx: security register3 check passed.
w25qxx: start chip erasing.
w25qxx: w25qxx_chip_erase test.
w25qxx: chip erase successful.
w25qxx: finish read test.
```

```shell
./w25qxx -c basic -type W25Q128 power_down -spi  

w25qxx: power down successful.
```

```shell
./w25qxx -c basic -type W25Q128 wake_up -spi

w25qxx: wake up successful.
```

```shell
./w25qxx -c basic -type W25Q128 chip_erase -spi

w25qxx: start chip erase.
w25qxx: chip erase successful.
```

```shell
./w25qxx -c basic -type W25Q128 get_id -spi

w25qxx: manufacturer is 0xEF device id is 0x17.
```

```shell
./w25qxx -c basic -type W25Q128 read 00 -spi

w25qxx: addr 0 is 8.
```

```shell
./w25qxx -c basic -type W25Q128 write 00 8 -spi

w25qxx: addr 0 write data 8.
```

```shell
./w25qxx -c advance -type W25Q128 power_down -spi  

w25qxx: power down successful.
```

```shell
./w25qxx -c advance -type W25Q128 wake_up -spi

w25qxx: wake up successful.
```

```shell
./w25qxx -c advance -type W25Q128 chip_erase -spi

w25qxx: start chip erase.
w25qxx: chip erase successful.
```

```shell
./w25qxx -c advance -type W25Q128 get_id -spi

w25qxx: manufacturer is 0xEF device id is 0x17.
```

```shell
./w25qxx -c advance -type W25Q128 read 00 -spi

w25qxx: addr 0 is 2.
```

```shell
./w25qxx -c advance -type W25Q128 write 00 2 -spi

w25qxx: addr 0 write data 2.
```

```shell
./w25qxx -c advance -type W25Q128 page_program 00 6 -spi

w25qxx: addr 0 page program data 6.
```

```shell
./w25qxx -c advance -type W25Q128 erase_4k 00 -spi

w25qxx: erase 4k addr 0 successful.
```

```shell
./w25qxx -c advance -type W25Q128 erase_32k 00 -spi

w25qxx: erase 32k addr 0 successful.
```

```shell
./w25qxx -c advance -type W25Q128 erase_64k 00 -spi

w25qxx: erase 64k addr 0 successful.
```

```shell
./w25qxx -c advance -type W25Q128 fast_read 00 -spi 

w25qxx: addr 0 is 7.
```

```shell
./w25qxx -c advance -type W25Q128 get_status1 -spi 

w25qxx: get status1 is 0x00.
```

```shell
./w25qxx -c advance -type W25Q128 get_status2 -spi 

w25qxx: get status2 is 0x02.
```

```shell
./w25qxx -c advance -type W25Q128 get_status3 -spi 

w25qxx: get status3 is 0x01.
```

```shell
./w25qxx -c advance -type W25Q128 set_status1 00 -spi

w25qxx: set status1 0x00.
```

```shell
./w25qxx -c advance -type W25Q128 set_status2 02 -spi

w25qxx: set status2 0x02.
```

```shell
./w25qxx -c advance -type W25Q128 set_status3 96 -spi

w25qxx: set status3 0x60.
```

```shell
./w25qxx -c advance -type W25Q128 get_jedec_id -spi

w25qxx: manufacturer is 0xEF device id is 0x40 0x18.
```

```shell
./w25qxx -c advance -type W25Q128 global_lock -spi 

w25qxx: global block lock successful.  
```

```shell
./w25qxx -c advance -type W25Q128 global_unlock -spi  

w25qxx: global block unlock successful.
```

```shell
./w25qxx -c advance -type W25Q128 block_lock 00 -spi

w25qxx: block lock addr 0.
```

```shell
./w25qxx -c advance -type W25Q128 block_unlock 00 -spi

w25qxx: block unlock addr 0.
```

```shell
./w25qxx -c advance -type W25Q128 read_block 00 -spi

w25qxx: read block addr 0 is 1. 
```

```shell
./w25qxx -c advance -type W25Q128 reset -spi

w25qxx: reset successful.
```

```shell
./w25qxx -c advance -type W25Q128 spi_read 00

w25qxx: add 0 spi read is 0x07.
```

```shell
./w25qxx -c advance -type W25Q128 spi_dual_output_read 00

w25qxx: this chip can't use this function.
```

```shell
./w25qxx -c advance -type W25Q128 spi_quad_output_read 00

w25qxx: this chip can't use this function.
```

```shell
./w25qxx -c advance -type W25Q128 spi_dual_io_read 00

w25qxx: this chip can't use this function.
```

```shell
./w25qxx -c advance -type W25Q128 spi_quad_io_read 00

w25qxx: this chip can't use this function.
```

```shell
./w25qxx -c advance -type W25Q128 spi_word_quad_io_read 00

w25qxx: this chip can't use this function.
```

```shell
./w25qxx -c advance -type W25Q128 spi_octal_word_quad_io_read 00

w25qxx: this chip can't use this function.
```

```shell
./w25qxx -c advance -type W25Q128 spi_read_security_reg 1

w25qxx: read 1 security register.
w25qxx: 0xC5 0xF7 0xDB 0x02 0x52 0x43 0xE1 0x94.
w25qxx: 0xDA 0x6C 0x5C 0x25 0xDE 0xE7 0xB3 0x51.
w25qxx: 0x49 0xE2 0xDB 0x68 0x16 0x0F 0x3D 0xA3.
w25qxx: 0x32 0xB8 0xE0 0xA3 0x41 0xD1 0x3C 0x65.
w25qxx: 0x51 0x25 0x55 0x34 0x76 0x5C 0x4A 0x6E.
w25qxx: 0x24 0x68 0x9F 0x7C 0x07 0xC7 0x40 0xD1.
w25qxx: 0x40 0x74 0x41 0xC6 0x11 0xC0 0x64 0x2A.
w25qxx: 0x48 0x00 0x57 0x86 0xB9 0x3D 0xDE 0x48.
w25qxx: 0x90 0xC4 0xC4 0x5A 0xEE 0x7A 0x91 0x1A.
w25qxx: 0x22 0x4F 0xA9 0xDC 0x20 0xFD 0x07 0x5C.
w25qxx: 0x00 0xE0 0xFA 0xC7 0x8A 0x79 0x43 0x99.
w25qxx: 0xB5 0x19 0xF8 0xD0 0x4A 0xC4 0xFF 0x3E.
w25qxx: 0x8A 0xEE 0x25 0xE3 0xE7 0x3D 0xD8 0x9C.
w25qxx: 0x74 0x21 0xC0 0xD8 0x4A 0xDD 0x6D 0xE1.
w25qxx: 0xB5 0x70 0x7E 0x89 0x87 0xA2 0x47 0xCE.
w25qxx: 0x4E 0xB2 0x7F 0xD1 0x61 0xCE 0xF2 0xBE.
w25qxx: 0xC3 0xCA 0x9C 0xF8 0x47 0xE4 0xC9 0xE1.
w25qxx: 0x95 0x6A 0x45 0x13 0x00 0xE5 0xE0 0x83.
w25qxx: 0x67 0xAA 0x4F 0x1B 0x25 0xCD 0x30 0x48.
w25qxx: 0xB1 0xEF 0x7F 0xDF 0x84 0xBC 0x65 0x55.
w25qxx: 0xB6 0x2A 0x1D 0x5D 0xDD 0x52 0xC1 0x38.
w25qxx: 0xD7 0x28 0xD8 0xA2 0xC7 0x14 0xFE 0x96.
w25qxx: 0x63 0x6E 0x50 0x40 0x52 0xAE 0x23 0x18.
w25qxx: 0x80 0xC6 0x15 0xA4 0xC1 0x1B 0xA2 0xCD.
w25qxx: 0x41 0x6D 0xEB 0xA2 0xAC 0xF4 0x81 0xFD.
w25qxx: 0x0D 0xBD 0x6B 0x65 0x1F 0xDE 0x6B 0xC9.
w25qxx: 0x6F 0x45 0xF4 0x28 0x7D 0x80 0xF7 0x83.
w25qxx: 0x6B 0x8A 0x48 0x7F 0xFE 0xB5 0xB9 0xD4.
w25qxx: 0x35 0x95 0x0D 0x2C 0x33 0xDC 0x01 0x2C.
w25qxx: 0xDD 0x95 0x34 0x7A 0x2B 0xA3 0x0F 0x03.
w25qxx: 0x31 0x9F 0xEC 0x40 0x19 0x57 0x86 0x15.
w25qxx: 0xA2 0x80 0xAF 0xD8 0x27 0x5D 0xFE 0x39.
```

```shell
./w25qxx -c advance -type W25Q128 spi_get_id_dual_io

w25qxx: this chip can't use this function.
```

```shell
./w25qxx -c advance -type W25Q128 spi_get_id_quad_io 

w25qxx: this chip can't use this function.
```

```shell
./w25qxx -c advance -type W25Q128 spi_get_sfdp

w25qxx: sdfp[0-7] is 0xC5 0xF7 0xDB 0x02 0x52 0x43 0xE1 0x94.
w25qxx: sdfp[8-15] is 0xDA 0x6C 0x5C 0x25 0xDE 0xE7 0xB3 0x51.
w25qxx: sdfp[16-23] is 0x49 0xE2 0xDB 0x68 0x16 0x0F 0x3D 0xA3.
w25qxx: sdfp[24-31] is 0x32 0xB8 0xE0 0xA3 0x41 0xD1 0x3C 0x65.
w25qxx: sdfp[32-39] is 0x51 0x25 0x55 0x34 0x76 0x5C 0x4A 0x6E.
w25qxx: sdfp[40-47] is 0x24 0x68 0x9F 0x7C 0x07 0xC7 0x40 0xD1.
w25qxx: sdfp[48-55] is 0x40 0x74 0x41 0xC6 0x11 0xC0 0x64 0x2A.
w25qxx: sdfp[56-63] is 0x48 0x00 0x57 0x86 0xB9 0x3D 0xDE 0x48.
w25qxx: sdfp[64-71] is 0x90 0xC4 0xC4 0x5A 0xEE 0x7A 0x91 0x1A.
w25qxx: sdfp[72-79] is 0x22 0x4F 0xA9 0xDC 0x20 0xFD 0x07 0x5C.
w25qxx: sdfp[80-87] is 0x00 0xE0 0xFA 0xC7 0x8A 0x79 0x43 0x99.
w25qxx: sdfp[88-95] is 0xB5 0x19 0xF8 0xD0 0x4A 0xC4 0xFF 0x3E.
w25qxx: sdfp[96-103] is 0x8A 0xEE 0x25 0xE3 0xE7 0x3D 0xD8 0x9C.
w25qxx: sdfp[104-111] is 0x74 0x21 0xC0 0xD8 0x4A 0xDD 0x6D 0xE1.
w25qxx: sdfp[112-119] is 0xB5 0x70 0x7E 0x89 0x87 0xA2 0x47 0xCE.
w25qxx: sdfp[120-127] is 0x4E 0xB2 0x7F 0xD1 0x61 0xCE 0xF2 0xBE.
w25qxx: sdfp[128-135] is 0xC3 0xCA 0x9C 0xF8 0x47 0xE4 0xC9 0xE1.
w25qxx: sdfp[136-143] is 0x95 0x6A 0x45 0x13 0x00 0xE5 0xE0 0x83.
w25qxx: sdfp[144-151] is 0x67 0xAA 0x4F 0x1B 0x25 0xCD 0x30 0x48.
w25qxx: sdfp[152-159] is 0xB1 0xEF 0x7F 0xDF 0x84 0xBC 0x65 0x55.
w25qxx: sdfp[160-167] is 0xB6 0x2A 0x1D 0x5D 0xDD 0x52 0xC1 0x38.
w25qxx: sdfp[168-175] is 0xD7 0x28 0xD8 0xA2 0xC7 0x14 0xFE 0x96.
w25qxx: sdfp[176-183] is 0x63 0x6E 0x50 0x40 0x52 0xAE 0x23 0x18.
w25qxx: sdfp[184-191] is 0x80 0xC6 0x15 0xA4 0xC1 0x1B 0xA2 0xCD.
w25qxx: sdfp[192-199] is 0x41 0x6D 0xEB 0xA2 0xAC 0xF4 0x81 0xFD.
w25qxx: sdfp[200-207] is 0x0D 0xBD 0x6B 0x65 0x1F 0xDE 0x6B 0xC9.
w25qxx: sdfp[208-215] is 0x6F 0x45 0xF4 0x28 0x7D 0x80 0xF7 0x83.
w25qxx: sdfp[216-223] is 0x6B 0x8A 0x48 0x7F 0xFE 0xB5 0xB9 0xD4.
w25qxx: sdfp[224-231] is 0x35 0x95 0x0D 0x2C 0x33 0xDC 0x01 0x2C.
w25qxx: sdfp[232-239] is 0xDD 0x95 0x34 0x7A 0x2B 0xA3 0x0F 0x03.
w25qxx: sdfp[240-247] is 0x31 0x9F 0xEC 0x40 0x19 0x57 0x86 0x15.
w25qxx: sdfp[248-255] is 0xA2 0x80 0xAF 0xD8 0x27 0x5D 0xFE 0x39.
```

```shell
./w25qxx -c advance -type W25Q128 spi_page_program_quad_input 00 8

w25qxx: this chip can't use this function.
```

```shell
./w25qxx -c advance -type W25Q128 spi_write_security_reg 1 8

w25qxx: write 1 security register successful.
```

```shell
./w25qxx -c advance -type W25Q128 qspi_set_read_parameters DUMMY_8_80MHZ 8 

w25qxx: this chip can't use qspi interface.
```

```shell
./w25qxx -c advance -type W25Q128 spi_set_burst WRAP_8_BYTE

w25qxx: set burst with wrap successful.
```

```shell
./w25qxx -h

w25qxx -i
	show w25qxx chip and driver information.
w25qxx -h
	show w25qxx help.
w25qxx -p
	show w25qxx pin connections of the current board.
w25qxx -c basic -type <type> power_down (-spi| -qspi)
	run w25qxx basic power down function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.
w25qxx -c basic -type <type> wake_up (-spi| -qspi)
	run w25qxx basic wake up function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.
w25qxx -c basic -type <type> chip_erase (-spi| -qspi)
	run w25qxx basic chip erase function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.
w25qxx -c basic -type <type> get_id (-spi| -qspi)
	run w25qxx basic get id function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.
w25qxx -c basic -type <type> read <addr> (-spi| -qspi)
	run w25qxx basic read function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.
w25qxx -c basic -type <type> write <addr> <data> (-spi| -qspi)
	run w25qxx basic write function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.
w25qxx -c advance -type <type> power_down (-spi| -qspi)
	run w25qxx advance power down function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.
w25qxx -c advance -type <type> wake_up (-spi| -qspi)
	run w25qxx advance wake up function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.
w25qxx -c advance -type <type> chip_erase (-spi| -qspi)
	run w25qxx advance chip erase function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.
w25qxx -c advance -type <type> get_id (-spi| -qspi)
	run w25qxx advance get id function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.
w25qxx -c advance -type <type> read <addr> (-spi| -qspi)
	run w25qxx advance read function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.
w25qxx -c advance -type <type> write <addr> <data> (-spi| -qspi)
	run w25qxx advance write function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.
w25qxx -c advance -type <type> page_program <addr> <data> (-spi| -qspi)
	run w25qxx advance page program function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.
w25qxx -c advance -type <type> erase_4k <addr> (-spi| -qspi)
	run w25qxx advance erase 4k function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.
w25qxx -c advance -type <type> erase_32k <addr> (-spi| -qspi)
	run w25qxx advance erase 32k function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.
w25qxx -c advance -type <type> erase_64k <addr> (-spi| -qspi)
	run w25qxx advance erase 64k function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.
w25qxx -c advance -type <type> fast_read <addr> (-spi| -qspi)
	run w25qxx advance fast read function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.
w25qxx -c advance -type <type> get_status1 (-spi| -qspi)
	run w25qxx advance get status1 function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.
w25qxx -c advance -type <type> get_status2 (-spi| -qspi)
	run w25qxx advance get status2 function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.
w25qxx -c advance -type <type> get_status3 (-spi| -qspi)
	run w25qxx advance get status3 function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.
w25qxx -c advance -type <type> set_status1 <status> (-spi| -qspi)
	run w25qxx advance set status1 function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.status is the set status.
w25qxx -c advance -type <type> set_status2 <status> (-spi| -qspi)
	run w25qxx advance set status2 function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.status is the set status.
w25qxx -c advance -type <type> set_status3 <status> (-spi| -qspi)
	run w25qxx advance set status3 function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.status is the set status.
w25qxx -c advance -type <type> get_jedec_id (-spi| -qspi)
	run w25qxx advance get jedec id function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.
w25qxx -c advance -type <type> global_lock (-spi| -qspi)
	run w25qxx advance global lock function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.
w25qxx -c advance -type <type> global_unlock (-spi| -qspi)
	run w25qxx advance global unlock function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.
w25qxx -c advance -type <type> read_block <addr> (-spi| -qspi)
	run w25qxx advance read block function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.
w25qxx -c advance -type <type> reset (-spi| -qspi)
	run w25qxx advance reset function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.
w25qxx -c advance -type <type> spi_read <addr>
	run w25qxx advance spi read function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.
w25qxx -c advance -type <type> spi_dual_output_read <addr>
	run w25qxx advance spi dual output read function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.
w25qxx -c advance -type <type> spi_quad_output_read <addr>
	run w25qxx advance spi quad output read function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.
w25qxx -c advance -type <type> spi_dual_io_read <addr>
	run w25qxx advance spi dual io read function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.
w25qxx -c advance -type <type> spi_quad_io_read <addr>
	run w25qxx advance spi quad io read function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.
w25qxx -c advance -type <type> spi_word_quad_io_read <addr>
	run w25qxx advance spi word quad io read function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.
w25qxx -c advance -type <type> spi_octal_word_quad_io_read <addr>
	run w25qxx advance spi octal word quad io read function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.
w25qxx -c advance -type <type> spi_page_program_quad_input <addr> <data>
	run w25qxx advance page program quad input function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.data is the written data.
w25qxx -c advance -type <type> spi_get_id_dual_io
	run w25qxx advance get id dual io function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.
w25qxx -c advance -type <type> spi_get_id_quad_io
	run w25qxx advance get id quad io function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.
w25qxx -c advance -type <type> spi_get_sfdp
	run w25qxx advance get sfdp function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.
w25qxx -c advance -type <type> spi_write_security_reg <num> <data>
	run w25qxx advance write security register function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.addr is the flash address.num is the security register number.data is the written data.
w25qxx -c advance -type <type> spi_read_security_reg <num>
	run w25qxx advance read security register function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.num is the security register number.num can be 1, 2, 3.
w25qxx -c advance -type <type> qspi_set_read_parameters <dummy> <length>
	run w25qxx advance set read parameters function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.dummy is the read dummy and it can be DUMMY_2_33MHZ, DUMMY_4_55MHZ, DUMMY_6_80MHZ, DUMMY_8_80MHZ.length is the wrap length and it can be 8, 16, 32 or 64.
w25qxx -c advance -type <type> spi_set_burst <wrap>
	run w25qxx advance set burst function.type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.wrap is the burst wrap and it can be WRAP_NONE,WRAP_8_BYTE,WRAP_16_BYTE,WRAP_32_BYTE,WRAP_64_BYTE.
```

