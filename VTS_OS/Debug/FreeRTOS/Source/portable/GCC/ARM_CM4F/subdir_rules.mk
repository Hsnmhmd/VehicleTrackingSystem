################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/Source/portable/GCC/ARM_CM4F/%.o: ../FreeRTOS/Source/portable/GCC/ARM_CM4F/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"D:/Online_Emb_Diploma/Installations/xpack-arm-none-eabi-gcc-10.2.1-1.1-win32-x64/xpack-arm-none-eabi-gcc-10.2.1-1.1/bin/arm-none-eabi-gcc-10.2.1.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DPART_TM4C123GH6PM -DTARGET_IS_TM4C123_RB1 -Dgcc -I"C:/Users/admin/Downloads/NTI_4_Months/TIVAC/VTS_OS" -I"C:/Users/admin/Downloads/NTI_4_Months/TIVAC/VTS_OS/driverlib" -I"C:/Users/admin/Downloads/NTI_4_Months/TIVAC/VTS_OS/FreeRTOS" -I"C:/Users/admin/Downloads/NTI_4_Months/TIVAC/VTS_OS/FreeRTOS/Source/include" -I"C:/Users/admin/Downloads/NTI_4_Months/TIVAC/VTS_OS/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/Users/admin/Downloads/NTI_4_Months/TIVAC/VTS_OS/FreeRTOS/Source/portable/MemMang" -I"C:/Users/admin/Downloads/NTI_4_Months/TIVAC/VTS_OS/HAL" -I"C:/Users/admin/Downloads/NTI_4_Months/TIVAC/VTS_OS/inc" -I"D:/Online_Emb_Diploma/Installations/xpack-arm-none-eabi-gcc-10.2.1-1.1-win32-x64/xpack-arm-none-eabi-gcc-10.2.1-1.1/arm-none-eabi/include" -O0 -ffunction-sections -fdata-sections -fsingle-precision-constant -g -gdwarf-3 -gstrict-dwarf -Wall -specs="nosys.specs" -MMD -MP -MF"FreeRTOS/Source/portable/GCC/ARM_CM4F/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


