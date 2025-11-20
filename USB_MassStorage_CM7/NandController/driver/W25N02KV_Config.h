/*
 *  W25N02KV_Config.h
 *
 *  Created on: Sep 24, 2025
 *  Author: Henry
 *  Folder: NandController/driver
 */

#ifndef DRIVER_W25N02KV_CONFIG_H_
#define DRIVER_W25N02KV_CONFIG_H_

/// ---------------------------------------------------------------------------
/// Device Information (From FEATURES)
/// ---------------------------------------------------------------------------
#define W25N02KV_CAPACITY_BITS     (2UL * 1024UL * 1024UL * 1024UL) // 2G-bit
#define W25N02KV_CAPACITY_BYTES    (256UL * 1024UL * 1024UL)        // 256MB (Main area)
#define W25N02KV_CAPACITY_MB       (W25N02KV_CAPACITY_BYTES / (1024UL * 1024UL))

/// ---------------------------------------------------------------------------
/// Organization
/// ---------------------------------------------------------------------------
#define PAGE_MAIN_SIZE             2048    							   // 2048 Bytes(2KB)
#define PAGE_SPARE_SIZE            128     							   // 128  Bytes(128B)
#define PAGE_TOTAL_SIZE            (PAGE_MAIN_SIZE + PAGE_SPARE_SIZE)  // 2176 Bytes
#define PAGES_PER_BLOCK            64      							   // 64 pages per block
#define BLOCK_SIZE_BYTES           (PAGES_PER_BLOCK * PAGE_TOTAL_SIZE) // 64 * 2176 Bytes = 139264 Bytes (136KB)

#define TOTAL_BLOCKS               2048    							   // 2048 blocks
#define TOTAL_PAGES                (TOTAL_BLOCKS * PAGES_PER_BLOCK)    // 2048 * 64 Bytes = 128KB

/// ---------------------------------------------------------------------------
/// Performance & Endurance
/// ---------------------------------------------------------------------------
#define W25N02KV_MAX_SPI_FREQ      104000000   // 104MHz Standard/Dual/Quad
#define W25N02KV_EQUIV_DUAL_IO     (208000000) // 104MHz x 2
#define W25N02KV_EQUIV_QUAD_IO     (416000000) // 104MHz x 4

#define W25N02KV_SEQ_TRANSFER_MBPS 50          // 50 MB/s sequential data rate

#define W25N02KV_ENDURANCE         100000      // 100k P/E cycles
#define W25N02KV_DATA_RETENTION    10          // 10 years

/// ---------------------------------------------------------------------------
/// Power
/// ---------------------------------------------------------------------------
#define W25N02KV_VCC_MIN           2.7f
#define W25N02KV_VCC_MAX           3.6f

#define W25N02KV_ACTIVE_CURRENT    25000   // 25 mA
#define W25N02KV_STANDBY_CURRENT   10      // 10 uA
#define W25N02KV_DEEP_POWERDOWN    1       // 1 uA

#define W25N02KV_TEMP_MIN          (-40)
#define W25N02KV_TEMP_MAX          (85)    // up to 105°C for industrial plus grade

/// ---------------------------------------------------------------------------
/// Special Features
/// ---------------------------------------------------------------------------
/// Built-in ECC: On-chip 8-bit ECC
#define W25N02KV_ECC_ENABLE        1

/// ECC-Protected Factory Invalid block info: Block 0-7 and Block 2044-2047
#define W25N02KV_FACTORY_BADBLOCK_PROTECTED 1
#define FACTORY_INFO_BLOCK_START   0
#define FACTORY_INFO_BLOCK_END     8
#define FACTORY_INFO_BLOCK2_START  2043
#define FACTORY_INFO_BLOCK2_END    2047

/// OTP pages
#define W25N02KV_OTP_PAGES         2

/// ---------------------------------------------------------------------------
/// JEDEC ID check
/// ---------------------------------------------------------------------------
#define W25N02KV_JEDEC_ID0         W25N02KV_MANUFACTURER_ID
#define W25N02KV_JEDEC_ID1         W25N02KV_MEMORY_TYPE
#define W25N02KV_JEDEC_ID2         W25N02KV_DEVICE_ID

/// ---------------------------------------------------------------------------
/// Address Calculation Macros
/// ---------------------------------------------------------------------------
#define PAGE_ADDR(block, page)     ((block) * PAGES_PER_BLOCK + (page))
#define BLOCK_ADDR(page_addr)      ((page_addr) / PAGES_PER_BLOCK)

/// ---------------------------------------------------------------------------
/// Data Pattern
/// ---------------------------------------------------------------------------

/// Status after erase block
#define NAND_ERASED_STATE          0xFF

#endif /* DRIVER_W25N02KV_CONFIG_H_ */
