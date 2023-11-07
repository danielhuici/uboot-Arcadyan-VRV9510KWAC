#!/bin/bash
# Generates a .asc image for UART Boot (Arcadyan VRV9510KWAC)

awk -f tools/lantiq_ram_init_uart -v soc=vr9 board/arcadyan/vrv9510kwac/ddr_settings.h > ddr_settings
tools/lantiq_uart_image.pl ddr_settings u-boot.srec u-boot.asc
