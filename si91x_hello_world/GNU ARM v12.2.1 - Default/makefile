################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/unified_peripheral_drivers/src/subdir.mk
-include wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/unified_api/src/subdir.mk
-include wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/systemlevel/src/subdir.mk
-include wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/service/sleeptimer/src/subdir.mk
-include wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/service/clock_manager/src/subdir.mk
-include wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/rom_driver/src/subdir.mk
-include wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/peripheral_drivers/src/subdir.mk
-include wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/hardware_drivers/led/src/subdir.mk
-include wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/hardware_drivers/button/src/subdir.mk
-include wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/cmsis_driver/subdir.mk
-include wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/core/config/src/subdir.mk
-include wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/core/common/src/subdir.mk
-include wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/core/chip/src/iPMU_prog/iPMU_dotc/subdir.mk
-include wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/core/chip/src/subdir.mk
-include wiseconnect3_sdk_3.4.1/components/board/silabs/src/subdir.mk
-include simplicity_sdk_2024.12.2/util/third_party/freertos/kernel/portable/MemMang/subdir.mk
-include simplicity_sdk_2024.12.2/util/third_party/freertos/kernel/portable/GCC/ARM_CM4F/subdir.mk
-include simplicity_sdk_2024.12.2/util/third_party/freertos/kernel/subdir.mk
-include simplicity_sdk_2024.12.2/util/third_party/freertos/cmsis/Source/subdir.mk
-include simplicity_sdk_2024.12.2/platform/service/system/src/subdir.mk
-include simplicity_sdk_2024.12.2/platform/service/sleeptimer/src/subdir.mk
-include simplicity_sdk_2024.12.2/platform/common/src/subdir.mk
-include simplicity_sdk_2024.12.2/platform/CMSIS/RTOS2/Source/subdir.mk
-include autogen/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := si91x_hello_world
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: si91x_hello_world.axf

# Tool invocations
si91x_hello_world.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "C:\Users\florin.mota\SimplicityStudio\v5_workspace\si91x_hello_world\autogen\linkerfile_SoC.ld" -u _printf_float -Wl,--no-warn-rwx-segments -fno-lto -Xlinker --gc-sections -Xlinker -Map="si91x_hello_world.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o si91x_hello_world.axf -Wl,--start-group "./app.o" "./button_baremetal.o" "./gpio_example.o" "./main.o" "./spi_app_layer.o" "./autogen/sl_event_handler.o" "./autogen/sl_si91x_button_instances.o" "./autogen/sl_si91x_led_instances.o" "./simplicity_sdk_2024.12.2/platform/CMSIS/RTOS2/Source/os_systick.o" "./simplicity_sdk_2024.12.2/platform/common/src/sl_assert.o" "./simplicity_sdk_2024.12.2/platform/common/src/sl_cmsis_os2_common.o" "./simplicity_sdk_2024.12.2/platform/common/src/sl_core_cortexm.o" "./simplicity_sdk_2024.12.2/platform/common/src/sl_syscalls.o" "./simplicity_sdk_2024.12.2/platform/service/sleeptimer/src/sl_sleeptimer.o" "./simplicity_sdk_2024.12.2/platform/service/system/src/sl_system_init.o" "./simplicity_sdk_2024.12.2/platform/service/system/src/sl_system_kernel.o" "./simplicity_sdk_2024.12.2/util/third_party/freertos/cmsis/Source/cmsis_os2.o" "./simplicity_sdk_2024.12.2/util/third_party/freertos/kernel/croutine.o" "./simplicity_sdk_2024.12.2/util/third_party/freertos/kernel/event_groups.o" "./simplicity_sdk_2024.12.2/util/third_party/freertos/kernel/list.o" "./simplicity_sdk_2024.12.2/util/third_party/freertos/kernel/queue.o" "./simplicity_sdk_2024.12.2/util/third_party/freertos/kernel/stream_buffer.o" "./simplicity_sdk_2024.12.2/util/third_party/freertos/kernel/tasks.o" "./simplicity_sdk_2024.12.2/util/third_party/freertos/kernel/timers.o" "./simplicity_sdk_2024.12.2/util/third_party/freertos/kernel/portable/GCC/ARM_CM4F/port.o" "./simplicity_sdk_2024.12.2/util/third_party/freertos/kernel/portable/MemMang/heap_3.o" "./wiseconnect3_sdk_3.4.1/components/board/silabs/src/rsi_board.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/core/chip/src/iPMU_prog/iPMU_dotc/ipmu_apis.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/core/chip/src/iPMU_prog/iPMU_dotc/rsi_system_config_917.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/core/chip/src/rsi_deepsleep_soc.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/core/chip/src/rsi_ps_ram_func.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/core/chip/src/startup_si91x.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/core/chip/src/system_si91x.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/core/common/src/rsi_debug.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/core/common/src/sl_si91x_stack_object_declare.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/core/common/src/syscalls.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/core/config/src/rsi_nvic_priorities_config.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/cmsis_driver/SPI.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/cmsis_driver/UDMA.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/cmsis_driver/USART.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/hardware_drivers/button/src/sl_si91x_button.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/hardware_drivers/led/src/sl_si91x_led.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/peripheral_drivers/src/aux_reference_volt_config.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/peripheral_drivers/src/clock_update.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/peripheral_drivers/src/rsi_adc.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/peripheral_drivers/src/rsi_crc.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/peripheral_drivers/src/rsi_dac.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/peripheral_drivers/src/rsi_egpio.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/peripheral_drivers/src/rsi_opamp.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/peripheral_drivers/src/rsi_spi.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/peripheral_drivers/src/rsi_sysrtc.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/peripheral_drivers/src/rsi_udma.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/peripheral_drivers/src/rsi_udma_wrapper.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/peripheral_drivers/src/rsi_usart.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/rom_driver/src/rsi_rom_table_si91x.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/service/clock_manager/src/sl_si91x_clock_manager.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/service/sleeptimer/src/sl_sleeptimer_hal_si91x_sysrtc.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/systemlevel/src/rsi_bod.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/systemlevel/src/rsi_ipmu.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/systemlevel/src/rsi_pll.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/systemlevel/src/rsi_rtc.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/systemlevel/src/rsi_temp_sensor.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/systemlevel/src/rsi_time_period.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/systemlevel/src/rsi_ulpss_clk.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/unified_api/src/sl_si91x_adc.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/unified_api/src/sl_si91x_bjt_temperature_sensor.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/unified_api/src/sl_si91x_dma.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/unified_api/src/sl_si91x_driver_gpio.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/unified_api/src/sl_si91x_ssi.o" "./wiseconnect3_sdk_3.4.1/components/device/silabs/si91x/mcu/drivers/unified_peripheral_drivers/src/sl_si91x_peripheral_gpio.o" -lgcc -lnosys -lc -lm -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building out file: si91x_hello_world.out'
	arm-none-eabi-objcopy "si91x_hello_world.axf" "si91x_hello_world.out"
	@echo ' '

	@echo 'Building hex file: si91x_hello_world.hex'
	arm-none-eabi-objcopy -O ihex "si91x_hello_world.axf" "si91x_hello_world.hex"
	@echo ' '

	@echo 'Building bin file: si91x_hello_world.bin'
	arm-none-eabi-objcopy -O binary "si91x_hello_world.axf" "si91x_hello_world.bin"
	@echo ' '

	@echo 'Building s37 file: si91x_hello_world.s37'
	arm-none-eabi-objcopy -O srec "si91x_hello_world.axf" "si91x_hello_world.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "si91x_hello_world.axf" -A
	@echo ' '

# Other Targets
clean:
	-@$(foreach FILE, $(EXECUTABLES), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(OBJS), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(C_DEPS), $(shell $(RM) $(FILE)) )
	-$(RM) si91x_hello_world.axf
	-@echo ' '

post-build:
	-"C:\SiliconLabs\SimplicityStudio\v5\developer\adapter_packs\commander\commander.exe" postbuild "C:\Users\florin.mota\SimplicityStudio\v5_workspace\si91x_hello_world/si91x_hello_world.slpb" --parameter build_dir:"C:\Users\florin.mota\SimplicityStudio\v5_workspace\si91x_hello_world\GNU ARM v12.2.1 - Default"
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
