# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# compile ASM with /Users/user/.espressif/tools/xtensa-esp32s2-elf/esp-2021r2-8.4.0/xtensa-esp32s2-elf/bin/xtensa-esp32s2-elf-gcc
# compile C with /Users/user/.espressif/tools/xtensa-esp32s2-elf/esp-2021r2-8.4.0/xtensa-esp32s2-elf/bin/xtensa-esp32s2-elf-gcc
ASM_DEFINES = -DMBEDTLS_CONFIG_FILE=\"mbedtls/esp_config.h\"

ASM_INCLUDES = -I/Users/user/esp/CECS490/led_blinky/build/config -I/Users/user/esp/esp-idf/components/mbedtls/port/include -I/Users/user/esp/esp-idf/components/mbedtls/mbedtls/include -I/Users/user/esp/esp-idf/components/mbedtls/esp_crt_bundle/include -I/Users/user/esp/esp-idf/components/newlib/platform_include -I/Users/user/esp/esp-idf/components/freertos/FreeRTOS-Kernel/include -I/Users/user/esp/esp-idf/components/freertos/esp_additions/include/freertos -I/Users/user/esp/esp-idf/components/freertos/FreeRTOS-Kernel/portable/xtensa/include -I/Users/user/esp/esp-idf/components/freertos/esp_additions/include -I/Users/user/esp/esp-idf/components/esp_hw_support/include -I/Users/user/esp/esp-idf/components/esp_hw_support/include/soc -I/Users/user/esp/esp-idf/components/esp_hw_support/include/soc/esp32s2 -I/Users/user/esp/esp-idf/components/esp_hw_support/port/esp32s2/. -I/Users/user/esp/esp-idf/components/esp_hw_support/port/esp32s2/private_include -I/Users/user/esp/esp-idf/components/heap/include -I/Users/user/esp/esp-idf/components/log/include -I/Users/user/esp/esp-idf/components/lwip/include/apps -I/Users/user/esp/esp-idf/components/lwip/include/apps/sntp -I/Users/user/esp/esp-idf/components/lwip/lwip/src/include -I/Users/user/esp/esp-idf/components/lwip/port/esp32/include -I/Users/user/esp/esp-idf/components/lwip/port/esp32/include/arch -I/Users/user/esp/esp-idf/components/soc/include -I/Users/user/esp/esp-idf/components/soc/esp32s2/. -I/Users/user/esp/esp-idf/components/soc/esp32s2/include -I/Users/user/esp/esp-idf/components/hal/esp32s2/include -I/Users/user/esp/esp-idf/components/hal/include -I/Users/user/esp/esp-idf/components/hal/platform_port/include -I/Users/user/esp/esp-idf/components/esp_rom/include -I/Users/user/esp/esp-idf/components/esp_rom/include/esp32s2 -I/Users/user/esp/esp-idf/components/esp_rom/esp32s2 -I/Users/user/esp/esp-idf/components/esp_common/include -I/Users/user/esp/esp-idf/components/esp_system/include -I/Users/user/esp/esp-idf/components/esp_system/port/soc -I/Users/user/esp/esp-idf/components/esp_system/port/include/private -I/Users/user/esp/esp-idf/components/xtensa/include -I/Users/user/esp/esp-idf/components/xtensa/esp32s2/include -I/Users/user/esp/esp-idf/components/driver/include -I/Users/user/esp/esp-idf/components/driver/esp32s2/include -I/Users/user/esp/esp-idf/components/driver/deprecated -I/Users/user/esp/esp-idf/components/esp_pm/include -I/Users/user/esp/esp-idf/components/esp_ringbuf/include -I/Users/user/esp/esp-idf/components/efuse/include -I/Users/user/esp/esp-idf/components/efuse/esp32s2/include -I/Users/user/esp/esp-idf/components/vfs/include -I/Users/user/esp/esp-idf/components/esp_wifi/include -I/Users/user/esp/esp-idf/components/esp_event/include -I/Users/user/esp/esp-idf/components/esp_netif/include -I/Users/user/esp/esp-idf/components/esp_eth/include -I/Users/user/esp/esp-idf/components/tcpip_adapter/include -I/Users/user/esp/esp-idf/components/esp_phy/include -I/Users/user/esp/esp-idf/components/esp_phy/esp32s2/include -I/Users/user/esp/esp-idf/components/esp_timer/include

ASM_FLAGS = -ffunction-sections -fdata-sections -Wall -Werror=all -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wextra -Wno-unused-parameter -Wno-sign-compare -ggdb -Og -fmacro-prefix-map=/Users/user/esp/CECS490/led_blinky=. -fmacro-prefix-map=/Users/user/esp/esp-idf=/IDF -fstrict-volatile-bitfields -Wno-error=unused-but-set-variable -fno-jump-tables -fno-tree-switch-conversion -D_GNU_SOURCE -DIDF_VER=\"v5.0-dev-1478-g6895a662aa\" -DESP_PLATFORM -D_POSIX_READER_WRITER_LOCKS

C_DEFINES = -DMBEDTLS_CONFIG_FILE=\"mbedtls/esp_config.h\"

C_INCLUDES = -I/Users/user/esp/CECS490/led_blinky/build/config -I/Users/user/esp/esp-idf/components/mbedtls/port/include -I/Users/user/esp/esp-idf/components/mbedtls/mbedtls/include -I/Users/user/esp/esp-idf/components/mbedtls/esp_crt_bundle/include -I/Users/user/esp/esp-idf/components/newlib/platform_include -I/Users/user/esp/esp-idf/components/freertos/FreeRTOS-Kernel/include -I/Users/user/esp/esp-idf/components/freertos/esp_additions/include/freertos -I/Users/user/esp/esp-idf/components/freertos/FreeRTOS-Kernel/portable/xtensa/include -I/Users/user/esp/esp-idf/components/freertos/esp_additions/include -I/Users/user/esp/esp-idf/components/esp_hw_support/include -I/Users/user/esp/esp-idf/components/esp_hw_support/include/soc -I/Users/user/esp/esp-idf/components/esp_hw_support/include/soc/esp32s2 -I/Users/user/esp/esp-idf/components/esp_hw_support/port/esp32s2/. -I/Users/user/esp/esp-idf/components/esp_hw_support/port/esp32s2/private_include -I/Users/user/esp/esp-idf/components/heap/include -I/Users/user/esp/esp-idf/components/log/include -I/Users/user/esp/esp-idf/components/lwip/include/apps -I/Users/user/esp/esp-idf/components/lwip/include/apps/sntp -I/Users/user/esp/esp-idf/components/lwip/lwip/src/include -I/Users/user/esp/esp-idf/components/lwip/port/esp32/include -I/Users/user/esp/esp-idf/components/lwip/port/esp32/include/arch -I/Users/user/esp/esp-idf/components/soc/include -I/Users/user/esp/esp-idf/components/soc/esp32s2/. -I/Users/user/esp/esp-idf/components/soc/esp32s2/include -I/Users/user/esp/esp-idf/components/hal/esp32s2/include -I/Users/user/esp/esp-idf/components/hal/include -I/Users/user/esp/esp-idf/components/hal/platform_port/include -I/Users/user/esp/esp-idf/components/esp_rom/include -I/Users/user/esp/esp-idf/components/esp_rom/include/esp32s2 -I/Users/user/esp/esp-idf/components/esp_rom/esp32s2 -I/Users/user/esp/esp-idf/components/esp_common/include -I/Users/user/esp/esp-idf/components/esp_system/include -I/Users/user/esp/esp-idf/components/esp_system/port/soc -I/Users/user/esp/esp-idf/components/esp_system/port/include/private -I/Users/user/esp/esp-idf/components/xtensa/include -I/Users/user/esp/esp-idf/components/xtensa/esp32s2/include -I/Users/user/esp/esp-idf/components/driver/include -I/Users/user/esp/esp-idf/components/driver/esp32s2/include -I/Users/user/esp/esp-idf/components/driver/deprecated -I/Users/user/esp/esp-idf/components/esp_pm/include -I/Users/user/esp/esp-idf/components/esp_ringbuf/include -I/Users/user/esp/esp-idf/components/efuse/include -I/Users/user/esp/esp-idf/components/efuse/esp32s2/include -I/Users/user/esp/esp-idf/components/vfs/include -I/Users/user/esp/esp-idf/components/esp_wifi/include -I/Users/user/esp/esp-idf/components/esp_event/include -I/Users/user/esp/esp-idf/components/esp_netif/include -I/Users/user/esp/esp-idf/components/esp_eth/include -I/Users/user/esp/esp-idf/components/tcpip_adapter/include -I/Users/user/esp/esp-idf/components/esp_phy/include -I/Users/user/esp/esp-idf/components/esp_phy/esp32s2/include -I/Users/user/esp/esp-idf/components/esp_timer/include

C_FLAGS = -mlongcalls  -ffunction-sections -fdata-sections -Wall -Werror=all -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wextra -Wno-unused-parameter -Wno-sign-compare -ggdb -Og -fmacro-prefix-map=/Users/user/esp/CECS490/led_blinky=. -fmacro-prefix-map=/Users/user/esp/esp-idf=/IDF -fstrict-volatile-bitfields -Wno-error=unused-but-set-variable -fno-jump-tables -fno-tree-switch-conversion -std=gnu99 -Wno-old-style-declaration -D_GNU_SOURCE -DIDF_VER=\"v5.0-dev-1478-g6895a662aa\" -DESP_PLATFORM -D_POSIX_READER_WRITER_LOCKS

