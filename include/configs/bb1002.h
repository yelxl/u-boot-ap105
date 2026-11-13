/*
 * AirMobi BB1002
 *
 * SoC:       Atheros AR7161
 * RAM:       64MB
 * SPI NOR:   16MB
 * Ethernet:  GE0 / RGMII
 * PHY:       Vitesse VSC8601 Rev.B
 * PHY Addr:  0
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <configs/ar7100.h>

/*-----------------------------------------------------------------------
 * Serial
 *-----------------------------------------------------------------------*/

#undef CONFIG_BOOTDELAY
#define CONFIG_BOOTDELAY        3

#undef CONFIG_BAUDRATE
#define CONFIG_BAUDRATE         115200


/*-----------------------------------------------------------------------
 * SPI NOR Flash
 *
 * 16MB = 256 sectors * 64KB
 *
 * Layout:
 * 0x000000 - 0x03ffff : U-Boot       256KB
 * 0x040000 - 0xfdffff : Firmware   16000KB
 * 0xfe0000 - 0xfeffff : HWINFO        64KB
 * 0xff0000 - 0xffffff : U-Boot ENV    64KB
 *-----------------------------------------------------------------------*/

#define CFG_MAX_FLASH_BANKS     1
#define CFG_MAX_FLASH_SECT      256
#define CFG_FLASH_SECTOR_SIZE   (64 * 1024)
#define CFG_FLASH_SIZE          0x01000000

#define CFG_FLASH_WORD_SIZE     unsigned short
#define CFG_FLASH_ADDR0         0x5555
#define CFG_FLASH_ADDR1         0x2AAA

#define CFG_FLASH_BASE          0xbf000000


/*-----------------------------------------------------------------------
 * Kernel boot arguments
 *-----------------------------------------------------------------------*/

#undef CONFIG_ROOTFS_RD
#undef CONFIG_ROOTFS_FLASH

#undef CONFIG_BOOTARGS_FL
#undef CONFIG_BOOTARGS_RD
#undef CONFIG_BOOTARGS

#undef MTDPARTS_DEFAULT
#undef MTDIDS_DEFAULT

#define CONFIG_ROOTFS_FLASH

#define CONFIG_BOOTARGS_FL \
    "console=ttyS0,115200 " \
    "mtdparts=ar7100-nor0:256k(uboot),16000k(firmware),64k(hwinfo),64k(u-boot-env)"

#define CONFIG_BOOTARGS       CONFIG_BOOTARGS_FL

#define MTDPARTS_DEFAULT \
    "mtdparts=ar7100-nor0:256k(uboot),16000k(firmware),64k(hwinfo),64k(u-boot-env)"

#define MTDIDS_DEFAULT        "nor0=ar7100-nor0"


/*-----------------------------------------------------------------------
 * U-Boot Flash area and environment
 *-----------------------------------------------------------------------*/

#define CFG_MONITOR_BASE      TEXT_BASE
#define CFG_MONITOR_LEN       (256 << 10)

#undef CFG_HZ
#define CFG_HZ                (680000000 / 2)

#define CFG_FLASH_ERASE_TOUT  (2 * CFG_HZ)
#define CFG_FLASH_WRITE_TOUT  (2 * CFG_HZ)

#define CFG_INIT_SP_OFFSET    0x1000

#define CFG_ENV_IS_IN_FLASH   1
#undef CFG_ENV_IS_NOWHERE

#define CFG_ENV_ADDR          0xbfff0000
#define CFG_ENV_SIZE          0x10000


/*-----------------------------------------------------------------------
 * Boot command
 *
 * 不采用 AP175 的 bootap 自动 TFTP 循环。
 * 正常启动只从 Firmware 分区启动。
 *-----------------------------------------------------------------------*/

#define CONFIG_BOOTCOMMAND    "bootm 0xbf040000"

#define CONFIG_EXTRA_ENV_SETTINGS \
    "autoload=n\0" \
    "ipaddr=192.168.1.2\0" \
    "serverip=192.168.1.101\0" \
    "bootfile=firmware.bin\0"


/*-----------------------------------------------------------------------
 * DDR / PLL
 *
 * 暂时沿用 AP175 的 AR7161 初始化参数作为编译和首轮调试基线。
 * 这些数值必须再与 BB1002 原厂 RedBoot 的 DDR/PLL 初始化结果核对。
 *-----------------------------------------------------------------------*/

#define CONFIG_NR_DRAM_BANKS            2
#define CFG_PLL_FREQ                    CFG_PLL_680_340_170

#define CFG_DDR_REFRESH_VAL             0x461b
#define CFG_DDR_CONFIG_VAL              0x77b8884e
#define CFG_DDR_CONFIG2_VAL             0x812cd6a8

#define CFG_DDR_MODE_VAL_INIT           0x131
#define CFG_DDR_EXT_MODE_VAL            0x0
#define CFG_DDR_MODE_VAL                0x33

#define CFG_DDR_TRTW_VAL                0x1f
#define CFG_DDR_TWTR_VAL                0x1e
#define CFG_DDR_RD_DATA_THIS_CYCLE_VAL  0xff

#define CONFIG_MEMSIZE_IN_BYTES


/*-----------------------------------------------------------------------
 * BB1002 board identifier
 *-----------------------------------------------------------------------*/

#define CFG_BOARD_BB1002                1


/*
 * 可选：如果已经从 RedBoot FIS 中确认了 Board data 分区的物理偏移，
 * 可以在此配置中定义：
 *
 * #define CFG_BB1002_BOARD_DATA_OFFSET  0x??????
 *
 * 单位为字节，起点相对于 Flash 基地址。
 *
 * OpenWrt 的 BSAP 系列代码从 Board data 分区偏移 2 读取 MAC。
 * 所以 BOARDCAL = Flash base + Board data offset + 2。
 *
 * 未确认实际 FIS 地址前不要填写猜测值。
 */

#ifdef CFG_BB1002_BOARD_DATA_OFFSET
#define BOARDCAL \
    (CFG_FLASH_BASE + CFG_BB1002_BOARD_DATA_OFFSET + 2)
#endif


/*-----------------------------------------------------------------------
 * Network commands
 *-----------------------------------------------------------------------*/

#define CONFIG_NET_MULTI

#define CONFIG_COMMANDS ( \
    CONFIG_CMD_DFL       | \
    CFG_CMD_MII          | \
    CFG_CMD_PING         | \
    CFG_CMD_NET          | \
    CFG_CMD_ENV          | \
    CFG_CMD_FLASH        | \
    CFG_CMD_RUN          | \
    CFG_CMD_DHCP         \
)

/* Default IP configuration */

#define CONFIG_IPADDR      192.168.1.2
#define CONFIG_SERVERIP    192.168.1.101


/*-----------------------------------------------------------------------
 * Ethernet hardware
 *
 * BB1002:
 *   GE0
 *   RGMII
 *   VSC8601
 *   MDIO address 0
 *-----------------------------------------------------------------------*/

#define CFG_FAULT_ECHO_LINK_DOWN     1

#define CONFIG_PHY_GIGE              1

#define CFG_VSC8601_PHY              1

#define VERBOSE                      10

#define CFG_AG7100_NMACS             1
#define CFG_PHY_ADDR                 0

#define CFG_GMII                     0
#define CFG_MII0_RGMII               1
#define CFG_AG7100_GE0_RGMII         1


/*-----------------------------------------------------------------------
 * Boot image
 *-----------------------------------------------------------------------*/

#define CFG_BOOTM_LEN                (16 << 20)


/*-----------------------------------------------------------------------
 * Hush parser
 *-----------------------------------------------------------------------*/

#define CFG_HUSH_PARSER
#define CFG_PROMPT_HUSH_PS2           "hush>"

#include <cmd_confdefs.h>

#endif /* __CONFIG_H */
