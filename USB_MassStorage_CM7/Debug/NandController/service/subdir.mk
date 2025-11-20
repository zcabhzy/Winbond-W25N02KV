################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../NandController/service/BBT_service.c \
../NandController/service/BlockErase_service.c \
../NandController/service/OTP_service.c \
../NandController/service/Program_service.c \
../NandController/service/Protect_service.c \
../NandController/service/ReadID_service.c \
../NandController/service/Read_service.c \
../NandController/service/Reset_service.c \
../NandController/service/StatusRegister_service.c 

OBJS += \
./NandController/service/BBT_service.o \
./NandController/service/BlockErase_service.o \
./NandController/service/OTP_service.o \
./NandController/service/Program_service.o \
./NandController/service/Protect_service.o \
./NandController/service/ReadID_service.o \
./NandController/service/Read_service.o \
./NandController/service/Reset_service.o \
./NandController/service/StatusRegister_service.o 

C_DEPS += \
./NandController/service/BBT_service.d \
./NandController/service/BlockErase_service.d \
./NandController/service/OTP_service.d \
./NandController/service/Program_service.d \
./NandController/service/Protect_service.d \
./NandController/service/ReadID_service.d \
./NandController/service/Read_service.d \
./NandController/service/Reset_service.d \
./NandController/service/StatusRegister_service.d 


# Each subdirectory must supply rules for building sources it contributes
NandController/service/%.o NandController/service/%.su NandController/service/%.cyclo: ../NandController/service/%.c NandController/service/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/driver" -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/service" -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/application" -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/hal" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-NandController-2f-service

clean-NandController-2f-service:
	-$(RM) ./NandController/service/BBT_service.cyclo ./NandController/service/BBT_service.d ./NandController/service/BBT_service.o ./NandController/service/BBT_service.su ./NandController/service/BlockErase_service.cyclo ./NandController/service/BlockErase_service.d ./NandController/service/BlockErase_service.o ./NandController/service/BlockErase_service.su ./NandController/service/OTP_service.cyclo ./NandController/service/OTP_service.d ./NandController/service/OTP_service.o ./NandController/service/OTP_service.su ./NandController/service/Program_service.cyclo ./NandController/service/Program_service.d ./NandController/service/Program_service.o ./NandController/service/Program_service.su ./NandController/service/Protect_service.cyclo ./NandController/service/Protect_service.d ./NandController/service/Protect_service.o ./NandController/service/Protect_service.su ./NandController/service/ReadID_service.cyclo ./NandController/service/ReadID_service.d ./NandController/service/ReadID_service.o ./NandController/service/ReadID_service.su ./NandController/service/Read_service.cyclo ./NandController/service/Read_service.d ./NandController/service/Read_service.o ./NandController/service/Read_service.su ./NandController/service/Reset_service.cyclo ./NandController/service/Reset_service.d ./NandController/service/Reset_service.o ./NandController/service/Reset_service.su ./NandController/service/StatusRegister_service.cyclo ./NandController/service/StatusRegister_service.d ./NandController/service/StatusRegister_service.o ./NandController/service/StatusRegister_service.su

.PHONY: clean-NandController-2f-service

