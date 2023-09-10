################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/Online_Emb_Diploma/Installations/xpack-arm-none-eabi-gcc-10.2.1-1.1-win32-x64/xpack-arm-none-eabi-gcc-10.2.1-1.1

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./SIM800.o" \
"./main.o" \
"./main1.o" \
"./tm4c123gh6pm_startup_ccs_gcc.o" \
"./FreeRTOS/Source/croutine.o" \
"./FreeRTOS/Source/event_groups.o" \
"./FreeRTOS/Source/list.o" \
"./FreeRTOS/Source/queue.o" \
"./FreeRTOS/Source/stream_buffer.o" \
"./FreeRTOS/Source/tasks.o" \
"./FreeRTOS/Source/timers.o" \
"./FreeRTOS/Source/portable/GCC/ARM_CM4F/port.o" \
"./FreeRTOS/Source/portable/MemMang/heap_4.o" \
"./HAL/gps.o" \
"./HAL/gsm_hw.o" \
"./driverlib/cpu.o" \
"./driverlib/gpio.o" \
"./driverlib/interrupt.o" \
"./driverlib/sysctl.o" \
"./driverlib/uart.o" \
"./driverlib/udma.o" \
-Wl,-T"../tm4c123gh6pm.lds" \
$(GEN_CMDS__FLAG) \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include FreeRTOS/Source/subdir_vars.mk
-include FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir_vars.mk
-include FreeRTOS/Source/portable/MemMang/subdir_vars.mk
-include HAL/subdir_vars.mk
-include driverlib/subdir_vars.mk
-include subdir_rules.mk
-include FreeRTOS/Source/subdir_rules.mk
-include FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir_rules.mk
-include FreeRTOS/Source/portable/MemMang/subdir_rules.mk
-include HAL/subdir_rules.mk
-include driverlib/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
VTS_OS.out \

EXE_OUTPUTS__QUOTED += \
"VTS_OS.out" \


# All Target
all: $(OBJS) $(LDS_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "VTS_OS.out"

# Tool invocations
VTS_OS.out: $(OBJS) $(LDS_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: GNU Linker'
	"D:/Online_Emb_Diploma/Installations/xpack-arm-none-eabi-gcc-10.2.1-1.1-win32-x64/xpack-arm-none-eabi-gcc-10.2.1-1.1/bin/arm-none-eabi-gcc-10.2.1.exe" -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mfpu=fpv4-sp-d16 -DPART_TM4C123GH6PM -DTARGET_IS_TM4C123_RB1 -Dgcc -O0 -ffunction-sections -fdata-sections -fsingle-precision-constant -g -gdwarf-3 -gstrict-dwarf -Wall -specs="nosys.specs" -mcpu=cortex-m4 -Wl,-Map,"VTS_OS.map" -Wl,--gc-sections -o"VTS_OS.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "SIM800.o" "main.o" "main1.o" "tm4c123gh6pm_startup_ccs_gcc.o" "FreeRTOS\Source\croutine.o" "FreeRTOS\Source\event_groups.o" "FreeRTOS\Source\list.o" "FreeRTOS\Source\queue.o" "FreeRTOS\Source\stream_buffer.o" "FreeRTOS\Source\tasks.o" "FreeRTOS\Source\timers.o" "FreeRTOS\Source\portable\GCC\ARM_CM4F\port.o" "FreeRTOS\Source\portable\MemMang\heap_4.o" "HAL\gps.o" "HAL\gsm_hw.o" "driverlib\cpu.o" "driverlib\gpio.o" "driverlib\interrupt.o" "driverlib\sysctl.o" "driverlib\uart.o" "driverlib\udma.o" 
	-$(RM) "SIM800.d" "main.d" "main1.d" "tm4c123gh6pm_startup_ccs_gcc.d" "FreeRTOS\Source\croutine.d" "FreeRTOS\Source\event_groups.d" "FreeRTOS\Source\list.d" "FreeRTOS\Source\queue.d" "FreeRTOS\Source\stream_buffer.d" "FreeRTOS\Source\tasks.d" "FreeRTOS\Source\timers.d" "FreeRTOS\Source\portable\GCC\ARM_CM4F\port.d" "FreeRTOS\Source\portable\MemMang\heap_4.d" "HAL\gps.d" "HAL\gsm_hw.d" "driverlib\cpu.d" "driverlib\gpio.d" "driverlib\interrupt.d" "driverlib\sysctl.d" "driverlib\uart.d" "driverlib\udma.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

