/*
 * AirMobi BB1002 Board Support
 *
 * SoC: AR7161
 * RAM: 64MB
 * Flash: 16MB SPI NOR
 */

#include <common.h>
#include <command.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <config.h>
#include <version.h>

#include "ar7100_soc.h"

extern flash_info_t flash_info[];


/*-----------------------------------------------------------------------
 * GPIO configuration
 *
 * Reference:
 * BSAP1880 GPIO assignment
 *
 * GPIO4 : status yellow LED
 * GPIO5 : status green LED
 * GPIO7 : WiFi 2.4G LED
 * GPIO8 : WiFi 5G LED
 * GPIO6 : reset button, active low
 *
 * LEDs are active low.
 *-----------------------------------------------------------------------*/

#define BB1002_GPIO_LED_MASK \
    ((1U << 4) | (1U << 5) | (1U << 7) | (1U << 8))

#define BB1002_GPIO_UART_TX_MASK (1U << 10)
#define BB1002_GPIO_UART_RX_MASK (1U << 9)

int board_gpio_config(void)
{
    /* Enable UART on GPIO9 and GPIO10 */
    ar7100_reg_wr(AR7100_GPIO_FUNC, 1U << 8);

    /* Active-low LEDs: default OFF */
    ar7100_reg_rmw_set(AR7100_GPIO_OUT,
                       BB1002_GPIO_LED_MASK);

    /* Enable LED outputs and UART TX output */
    ar7100_reg_rmw_set(
        AR7100_GPIO_OE,
        BB1002_GPIO_LED_MASK |
        BB1002_GPIO_UART_TX_MASK
    );

    /* UART RX and reset button remain inputs */
    ar7100_reg_rmw_clear(
        AR7100_GPIO_OE,
        BB1002_GPIO_UART_RX_MASK | (1U << 6)
    );

    return 0;
}

/*-----------------------------------------------------------------------
 * DDR initialization
 *
 * Initial values copied from AP175 AR7161 implementation.
 * Must be checked against BB1002 original RedBoot register values.
 *-----------------------------------------------------------------------*/

int ar7100_mem_config(void)
{
    *(volatile unsigned int *)0xb8050004 = 0x50C0;
    udelay(10);

    *(volatile unsigned int *)0xb8050018 = 0x1313;
    udelay(10);

    *(volatile unsigned int *)0xb805001c = 0x67;
    udelay(10);

    *(volatile unsigned int *)0xb8050010 = 0x1099;
    udelay(10);

    /*
     * Detect actual installed RAM size.
     * Expected BB1002 RAM: 64MB.
     */
    return ar7100_ddr_find_size();
}


long int initdram(int board_type)
{
#ifdef DEBUG
    printf("BB1002 DDR register: 0x%08x\n",
           *(volatile unsigned long *)0xb8050000);
#endif

    return ar7100_mem_config();
}


/*-----------------------------------------------------------------------
 * Board identification
 *-----------------------------------------------------------------------*/

int checkboard(void)
{
    printf("AirMobi BB1002 (AR7161) U-Boot\n");
    return 0;
}


/*-----------------------------------------------------------------------
 * SPI NOR geometry
 *
 * 16MB / 64KB = 256 sectors
 *
 * S25FL128P is handled by the generic AR7100 SPI command path.
 * This driver uses 64KB sector erase and 256-byte page programming.
 *-----------------------------------------------------------------------*/

unsigned long flash_get_geom(flash_info_t *flash_info)
{
    int i;

    flash_info->flash_id = FLASH_UNKNOWN;

    flash_info->size = CFG_FLASH_SIZE;

    flash_info->sector_count =
        flash_info->size / CFG_FLASH_SECTOR_SIZE;

    for (i = 0; i < flash_info->sector_count; i++) {

        flash_info->start[i] =
            CFG_FLASH_BASE + (i * CFG_FLASH_SECTOR_SIZE);

        flash_info->protect[i] = 0;
    }

#ifdef DEBUG
    printf("BB1002 SPI NOR:\n");
    printf("Flash size: %lu bytes\n", flash_info->size);
    printf("Sector size: %d bytes\n", CFG_FLASH_SECTOR_SIZE);
    printf("Sector count: %d\n", flash_info->sector_count);
#endif

    return flash_info->size;
}

/*
 * BB1002 reset button handler
 *
 * GPIO6 is the active-low reset button.
 *
 * Do not execute AP175 resetcmd.
 * Keep reset-button TFTP/flash recovery disabled
 * until BB1002 recovery commands are implemented.
 */

int check_reset_button(void)
{
#ifdef DEBUG
    printf("BB1002: reset button handler disabled\n");
#endif

    return 0;
}
