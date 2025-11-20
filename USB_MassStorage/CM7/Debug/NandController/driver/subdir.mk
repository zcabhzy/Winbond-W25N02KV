################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../NandController/driver/nand_dri_BlockErase.c \
../NandController/driver/nand_dri_Program.c \
../NandController/driver/nand_dri_Protect.c \
../NandController/driver/nand_dri_Read.c \
../NandController/driver/nand_dri_ReadID.c \
../NandController/driver/nand_dri_Reset.c \
../NandController/driver/nand_dri_StatusRegister.c 

OBJS += \
./NandController/driver/nand_dri_BlockErase.o \
./NandController/driver/nand_dri_Program.o \
./NandController/driver/nand_dri_Protect.o \
./NandController/driver/nand_dri_Read.o \
./NandController/driver/nand_dri_ReadID.o \
./NandController/driver/nand_dri_Reset.o \
./NandController/driver/nand_dri_StatusRegister.o 

C_DEPS += \
./NandController/driver/nand_dri_BlockErase.d \
./NandController/driver/nand_dri_Program.d \
./NandController/driver/nand_dri_Protect.d \
./NandController/driver/nand_dri_Read.d \
./NandController/driver/nand_dri_ReadID.d \
./NandController/driver/nand_dri_Reset.d \
./NandController/driver/nand_dri_StatusRegister.d 


# Each subdirectory must supply rules for building sources it contributes
NandController/driver/%.o NandController/driver/%.su NandController/driver/%.cyclo: ../NandController/driver/%.c NandController/driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/driver" -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/service" -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/application" -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/hal" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-NandController-2f-driver

clean-NandController-2f-driver:
	-$(RM) ./NandController/driver/nand_dri_BlockErase.cyclo ./NandController/driver/nand_dri_BlockErase.d ./NandController/driver/nand_dri_BlockErase.o ./NandController/driver/nand_dri_BlockErase.su ./NandController/driver/nand_dri_Program.cyclo ./NandController/driver/nand_dri_Program.d ./NandController/driver/nand_dri_Program.o ./NandController/driver/nand_dri_Program.su ./NandController/driver/nand_dri_Protect.cyclo ./NandController/driver/nand_dri_Protect.d ./NandController/driver/nand_dri_Protect.o ./NandController/driver/nand_dri_Protect.su ./NandController/driver/nand_dri_Read.cyclo ./NandController/driver/nand_dri_Read.d ./NandController/driver/nand_dri_Read.o ./NandController/driver/nand_dri_Read.su ./NandController/driver/nand_dri_ReadID.cyclo ./NandController/driver/nand_dri_ReadID.d ./NandController/driver/nand_dri_ReadID.o ./NandController/driver/nand_dri_ReadID.su ./NandController/driver/nand_dri_Reset.cyclo ./NandController/driver/nand_dri_Reset.d ./NandController/driver/nand_dri_Reset.o ./NandController/driver/nand_dri_Reset.su ./NandController/driver/nand_dri_StatusRegister.cyclo ./NandController/driver/nand_dri_StatusRegister.d ./NandController/driver/nand_dri_StatusRegister.o ./NandController/driver/nand_dri_StatusRegister.su

.PHONY: clean-NandController-2f-driver

