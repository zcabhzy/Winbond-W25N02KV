################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../NandController/Src/nand_Erase.c \
../NandController/Src/nand_Program.c \
../NandController/Src/nand_Protect.c \
../NandController/Src/nand_Read.c 

OBJS += \
./NandController/Src/nand_Erase.o \
./NandController/Src/nand_Program.o \
./NandController/Src/nand_Protect.o \
./NandController/Src/nand_Read.o 

C_DEPS += \
./NandController/Src/nand_Erase.d \
./NandController/Src/nand_Program.d \
./NandController/Src/nand_Protect.d \
./NandController/Src/nand_Read.d 


# Each subdirectory must supply rules for building sources it contributes
NandController/Src/%.o NandController/Src/%.su NandController/Src/%.cyclo: ../NandController/Src/%.c NandController/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/User/STM32CubeIDE/workspace_1.18.1/USB_MassStorage/CM7/NandController/Inc" -I"C:/Users/User/STM32CubeIDE/workspace_1.18.1/USB_MassStorage/CM7/FTLController/Inc" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/User/STM32CubeIDE/workspace_1.18.1/USB_MassStorage/CM7/NandController/driver" -I"C:/Users/User/STM32CubeIDE/workspace_1.18.1/USB_MassStorage/CM7/NandController/service" -I"C:/Users/User/STM32CubeIDE/workspace_1.18.1/USB_MassStorage/CM7/NandController/application" -I"C:/Users/User/STM32CubeIDE/workspace_1.18.1/USB_MassStorage/CM7/NandController/hal" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-NandController-2f-Src

clean-NandController-2f-Src:
	-$(RM) ./NandController/Src/nand_Erase.cyclo ./NandController/Src/nand_Erase.d ./NandController/Src/nand_Erase.o ./NandController/Src/nand_Erase.su ./NandController/Src/nand_Program.cyclo ./NandController/Src/nand_Program.d ./NandController/Src/nand_Program.o ./NandController/Src/nand_Program.su ./NandController/Src/nand_Protect.cyclo ./NandController/Src/nand_Protect.d ./NandController/Src/nand_Protect.o ./NandController/Src/nand_Protect.su ./NandController/Src/nand_Read.cyclo ./NandController/Src/nand_Read.d ./NandController/Src/nand_Read.o ./NandController/Src/nand_Read.su

.PHONY: clean-NandController-2f-Src

