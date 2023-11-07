/*
 * Copyright (C) 2013 Luka Perkov <luka@openwrt.org>
 * Copyright (C) 2012-2015 Daniel Schwierzeck, <daniel.schwierzeck@gmail.com>
 * Copyright (C) 2022-2023 Daniel Huici, <danielhuici@hotmail.com>
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

#define CONFIG_LTQ_SUPPORT_NAND_FLASH		/* Have a ZENTEL A5U1GA31ATS 8G NAND flash */
#define CONFIG_SYS_NAND_USE_FLASH_BBT

#define CONFIG_LTQ_SUPPORT_SPL_NAND_FLASH	/* Build NAND flash SPL */
#define CONFIG_LTQ_NAND_CS1
#define CONFIG_SYS_NAND_PAGE_COUNT	64
#define CONFIG_SYS_NAND_PAGE_SIZE	2048
#define CONFIG_SYS_NAND_OOBSIZE		64
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128 * 1024) /* 128 KB */
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_BOOTM_LEN          0x1000000       /* 16 MB */

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
#define CONFIG_SPL_U_BOOT_SIZE		0x60000
#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_ENV_OVERWRITE
#define CONFIG_ENV_OFFSET		0x80000
#define CONFIG_ENV_SECT_SIZE		(128 * 1024)
#define MTDPARTS_DEFAULT		\
	"mtdparts=nand-xway:512k(uboot),128k(uboot_env),4m(kernel),122m(ubi),4k(board_config)"
#define CONFIG_ENV_FIRSTBOOT \
	"first-boot=1\0"

#else
#define CONFIG_ENV_IS_NOWHERE
#define MTDPARTS_DEFAULT		"mtdparts="
#define CONFIG_ENV_FIRSTBOOT \
	"first-boot=0\0"
#endif

#define CONFIG_ENV_SIZE			(128 * 1024)
#define MTDIDS_DEFAULT			"nand0=nand-xway"

/* Console */
#define CONFIG_LTQ_ADVANCED_CONSOLE
#define CONFIG_BAUDRATE			115200
#define CONFIG_CONSOLE_ASC		1
#define CONFIG_BOOTDELAY 3 

/* Commands */
#define CONFIG_CMD_PING
#define CONFIG_CMD_MISC
#define CONFIG_CMD_ECHO
#define CONFIG_CMD_TFTPPUT
#define CONFIG_CMD_MOVE_BOARDCONFIG		 \
	"move-boardconfig=nand read 0x81000000 0x0 0x16e1000 && " \
	"nand erase 0x7ea0000 0x1000 && " \
	"nand write 0x826e0000 0x7ea0000 0x1000 && " \
	"setenv first-boot 0 && " \
	"saveenv\0" 
#define CONFIG_CMD_LOAD_TFTP_IMAGE		 \
	"rescue-tftp=sleep 5;" \
	"tftpboot rescue.bin &&" \
	"bootm $fileaddr\0" 
#define CONFIG_CMD_BOOTNAND \
	"sysboot=nand read 0x81000000 0xa0000 0x400000 && "\
	"bootm 0x81000000\0"

/* Boot */
#define CONFIG_MIPS_BOOT_FDT
#define CONFIG_FIT
#define CONFIG_OF_LIBFDT
#define CONFIG_LZMA
#define CONFIG_LZO
#define CONFIG_BOARD_LATE_INIT

/* Environment */
#define CONFIG_LOADADDR			CONFIG_SYS_LOAD_ADDR

#define CONFIG_ENV_MTDPARTS			\
	"mtdids="MTDIDS_DEFAULT"\0"		\
	"mtdparts="MTDPARTS_DEFAULT"\0" 

/* Ethernet */
#if defined(CONFIG_LTQ_SUPPORT_ETHERNET)
#define CONFIG_ETHADDR		00:01:02:03:04:05
#define CONFIG_SERVERIP		192.168.1.2
#define CONFIG_IPADDR		192.168.1.1
#endif

/* Pull in default board configs for Lantiq XWAY VRX200 */
#include <asm/lantiq/config.h>
#include <asm/arch/config.h>

#define CONFIG_EXTRA_ENV_SETTINGS	\
	CONFIG_ENV_LANTIQ_DEFAULTS	\
	CONFIG_ENV_MTDPARTS \
	CONFIG_ENV_FIRSTBOOT \
	CONFIG_CMD_MOVE_BOARDCONFIG \
	CONFIG_CMD_LOAD_TFTP_IMAGE \
	CONFIG_CMD_BOOTNAND \
	CONFIG_ENV_FIRSTBOOT

#endif /* __CONFIG_H */
