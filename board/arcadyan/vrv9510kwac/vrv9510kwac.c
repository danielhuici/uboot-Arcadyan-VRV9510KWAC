/*
 * Copyright (C) 2013 Luka Perkov <luka@openwrt.org>
 * Copyright (C) 2011-2015 Daniel Schwierzeck <daniel.schwierzeck@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/gpio.h>
#include <asm/arch/gpio.h>
#include <asm/lantiq/eth.h>
#include <asm/lantiq/chipid.h>
#include <asm/lantiq/cpu.h>
#include <asm/lantiq/mem.h>
#include <asm/arch/gphy.h>

#define GPIO_SERVICE_BUTTON 22
#define GPIO_POWER_LED 38
#define GPIO_SERVICE_LED 41

#if defined(CONFIG_SPL_BUILD)
#define do_gpio_init	1
#define do_pll_init	1
#define do_dcdc_init	0
#elif defined(CONFIG_SYS_BOOT_RAM)
#define do_gpio_init	1
#define do_pll_init	0
#define do_dcdc_init	1
#else
#define do_gpio_init	0
#define do_pll_init	0
#define do_dcdc_init	1
#endif

static void gpio_init(void)
{
	/* EBU.FL_CS1 as output for NAND CE */
	gpio_set_altfunc(23, GPIO_ALTSEL_SET, GPIO_ALTSEL_CLR, GPIO_DIR_OUT);
	/* EBU.FL_A23 as output for NAND CLE */
	gpio_set_altfunc(24, GPIO_ALTSEL_SET, GPIO_ALTSEL_CLR, GPIO_DIR_OUT);
	/* EBU.FL_A24 as output for NAND ALE */
	gpio_set_altfunc(13, GPIO_ALTSEL_SET, GPIO_ALTSEL_CLR, GPIO_DIR_OUT);
	/* GPIO 3.0 as input for NAND Ready Busy */
	gpio_set_altfunc(48, GPIO_ALTSEL_SET, GPIO_ALTSEL_CLR, GPIO_DIR_IN);
	/* GPIO 3.1 as output for NAND Read */
	gpio_set_altfunc(49, GPIO_ALTSEL_SET, GPIO_ALTSEL_CLR, GPIO_DIR_OUT);

	gpio_request(GPIO_SERVICE_LED, "");
	gpio_request(GPIO_POWER_LED, "");
	gpio_direction_output(GPIO_POWER_LED, 0);
}

int board_early_init_f(void)
{
	if (do_gpio_init) {
		ltq_gpio_init();
		gpio_init();
	}

	if (do_pll_init)
		ltq_pll_init();

	if (do_dcdc_init)
		ltq_dcdc_init(0x7F);


	return 0;
}


int board_late_init(void)
{
#if defined(CONFIG_SYS_BOOT_NANDSPL)
	char *first_boot = getenv("first-boot");
	int service_button_pressed = !gpio_get_value(GPIO_SERVICE_BUTTON);
	
	if (strcmp(first_boot, "1") == 0) {
		gpio_direction_output(GPIO_SERVICE_LED, 0);
		setenv("bootcmd", "run move-boardconfig rescue-tftp");
	} else if (service_button_pressed) {
		gpio_direction_output(GPIO_SERVICE_LED, 0);
		printf("Service button pressed! Launching firmware via tftpboot...\n");
		setenv("bootcmd", "run rescue-tftp");
	} else {
		setenv("bootcmd", "run sysboot");
	}
#else 
	
#endif

	return 0;
}


int checkboard(void)
{
	puts("Board: " CONFIG_BOARD_NAME "\n");
	ltq_chip_print_info();

	return 0;
}

int misc_init_r(void)
{
	return mc_tune_store_flash();
}

/* WAN and LAN1 ports (phy internal) not working */
static const struct ltq_eth_port_config eth_port_config[] = {
	/* GMAC0: external Lantiq PEF7071 10/100/1000 PHY for LAN port 3 */
	{ 0, 0x0, LTQ_ETH_PORT_PHY, PHY_INTERFACE_MODE_RGMII },
	/* GMAC1: external Lantiq PEF7071 10/100/1000 PHY for LAN port 2 */
	{ 1, 0x1, LTQ_ETH_PORT_PHY, PHY_INTERFACE_MODE_RGMII },
	/* GMAC2: internal GPHY0 with 10/100/1000 firmware for LAN port 1 */
	{ 2, 0x11, LTQ_ETH_PORT_PHY, PHY_INTERFACE_MODE_GMII },
	/* GMAC3: unused */
	{ 3, 0x0, LTQ_ETH_PORT_NONE, PHY_INTERFACE_MODE_NONE },
	/* GMAC4: internal GPHY1 with 10/100/1000 firmware for WAN port */
	{ 4, 0x13, LTQ_ETH_PORT_PHY, PHY_INTERFACE_MODE_GMII },
	/* GMAC5: external Lantiq PEF7071 10/100/1000 PHY for LAN 4 */
	{ 5, 0x5, LTQ_ETH_PORT_PHY, PHY_INTERFACE_MODE_RGMII },
};

static const struct ltq_eth_board_config eth_board_config = {
	.ports = eth_port_config,
	.num_ports = ARRAY_SIZE(eth_port_config),
};

int board_eth_init(bd_t * bis)
{
	const enum ltq_gphy_clk clk = LTQ_GPHY_CLK_25MHZ_PLL0;
	const ulong fw_addr = 0x80FF0000;

	ltq_gphy_phy11g_a1x_load(fw_addr);

	ltq_cgu_gphy_clk_src(clk);

	ltq_rcu_gphy_boot(0, fw_addr);
	ltq_rcu_gphy_boot(1, fw_addr);

	return ltq_eth_initialize(&eth_board_config);
}
