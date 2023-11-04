/*
 * Copyright (C) 2013 Luka Perkov <luka@openwrt.org>
 * Copyright (C) 2012-2015 Daniel Schwierzeck, daniel.schwierzeck@gmail.com
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_IDENT_STRING	"VRV9510KWAC"
#define CONFIG_BOARD_NAME	"Arcadyan VRV9510KWAC"

/* Configure SoC */
#define CONFIG_LTQ_SUPPORT_UART		/* Enable ASC and UART */

#define CONFIG_LTQ_SUPPORT_ETHERNET	/* Enable ethernet */
#define CONFIG_FW_VRX200_PHY11G_A1X

#define CONFIG_LTQ_SUPPORT_NAND_FLASH		/* Have a K9F1G08U0D NAND flash */
#define CONFIG_SYS_NAND_USE_FLASH_BBT

#define CONFIG_LTQ_SUPPORT_SPL_NAND_FLASH	/* Build NAND flash SPL */
#define CONFIG_LTQ_NAND_CS1
#define CONFIG_SYS_NAND_PAGE_COUNT	64
#define CONFIG_SYS_NAND_PAGE_SIZE	2048
#define CONFIG_SYS_NAND_OOBSIZE		64
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128 * 1024)
#define CONFIG_SYS_NAND_5_ADDR_CYCLE

#define CONFIG_LTQ_SPL_COMP_LZO
#define CONFIG_LTQ_SPL_CONSOLE
#define CONFIG_LTQ_SPL_MC_TUNE
#define CONFIG_MISC_INIT_R

/* MTD devices */
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_CMD_MTD
#define CONFIG_CMD_MTDPARTS
#define CONFIG_CMD_UBI
#define CONFIG_RBTREE

/* Environment */
#if defined(CONFIG_SYS_BOOT_NANDSPL)
#define CONFIG_SPL_TPL_OFFS		0x800
#define CONFIG_SPL_TPL_SIZE		0x5000
#define CONFIG_SPL_MC_TUNE_OFFS		0x5800
#define CONFIG_SPL_U_BOOT_OFFS		0x20000
#define CONFIG_SPL_U_BOOT_SIZE		0x44000

#define CONFIG_ENV_IS_IN_UBI
#define CONFIG_ENV_OVERWRITE
#define CONFIG_ENV_UBI_PART		"ubi"
#define CONFIG_ENV_UBI_VOLUME		"uboot_env"
#define CONFIG_ENV_UBI_VID_HDR_OFF	"2048"

#define MTDPARTS_DEFAULT		\
	"mtdparts=nand-xway:512k(uboot_fix),-(ubi)"
#else
#define CONFIG_ENV_IS_NOWHERE

#define MTDPARTS_DEFAULT		"mtdparts="
#endif

#define MTDIDS_DEFAULT			"nand0=nand-xway"
#define CONFIG_ENV_SIZE			(8 * 1024)

/* Console */
#define CONFIG_LTQ_ADVANCED_CONSOLE
#define CONFIG_BAUDRATE			115200
#define CONFIG_CONSOLE_ASC		1

/* Commands */
#define CONFIG_CMD_PING
#define CONFIG_CMD_MISC
#define CONFIG_CMD_ECHO

/* Boot */
#define CONFIG_MIPS_BOOT_FDT
#define CONFIG_FIT
#define CONFIG_OF_LIBFDT
#define CONFIG_LZMA
#define CONFIG_LZO

/* Environment */
#define CONFIG_LOADADDR			CONFIG_SYS_LOAD_ADDR

#define CONFIG_ENV_MTDPARTS			\
	"mtdids="MTDIDS_DEFAULT"\0"		\
	"mtdparts="MTDPARTS_DEFAULT"\0"

/* Pull in default board configs for Lantiq XWAY VRX200 */
#include <asm/lantiq/config.h>
#include <asm/arch/config.h>

#define CONFIG_EXTRA_ENV_SETTINGS	\
	CONFIG_ENV_LANTIQ_DEFAULTS	\
	CONFIG_ENV_MTDPARTS

#endif /* __CONFIG_H */
