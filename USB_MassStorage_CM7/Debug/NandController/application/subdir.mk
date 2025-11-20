################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../NandController/application/Endurance_Test.c \
../NandController/application/FactoryInvalidBlockScan_Test.c \
../NandController/application/Pattern.c \
../NandController/application/SinglePage_Test(Multiple\ Pattern).c \
../NandController/application/SinglePage_Test(RandomLoadProgram).c \
../NandController/application/SinglePage_Test.c 

OBJS += \
./NandController/application/Endurance_Test.o \
./NandController/application/FactoryInvalidBlockScan_Test.o \
./NandController/application/Pattern.o \
./NandController/application/SinglePage_Test(Multiple\ Pattern).o \
./NandController/application/SinglePage_Test(RandomLoadProgram).o \
./NandController/application/SinglePage_Test.o 

C_DEPS += \
./NandController/application/Endurance_Test.d \
./NandController/application/FactoryInvalidBlockScan_Test.d \
./NandController/application/Pattern.d \
./NandController/application/SinglePage_Test(Multiple\ Pattern).d \
./NandController/application/SinglePage_Test(RandomLoadProgram).d \
./NandController/application/SinglePage_Test.d 


# Each subdirectory must supply rules for building sources it contributes
NandController/application/%.o NandController/application/%.su NandController/application/%.cyclo: ../NandController/application/%.c NandController/application/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/driver" -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/service" -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/application" -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/hal" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
NandController/application/SinglePage_Test(Multiple\ Pattern).o: ../NandController/application/SinglePage_Test(Multiple\ Pattern).c NandController/application/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/driver" -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/service" -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/application" -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/hal" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"NandController/application/SinglePage_Test(Multiple Pattern).d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
NandController/application/SinglePage_Test(RandomLoadProgram).o: ../NandController/application/SinglePage_Test(RandomLoadProgram).c NandController/application/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/driver" -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/service" -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/application" -I"C:/Users/CPC/STM32CubeIDE/workspace_1.19.0/USB_MassStorage/CM7/NandController/hal" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"NandController/application/SinglePage_Test(RandomLoadProgram).d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-NandController-2f-application

clean-NandController-2f-application:
	-$(RM) ./NandController/application/Endurance_Test.cyclo ./NandController/application/Endurance_Test.d ./NandController/application/Endurance_Test.o ./NandController/application/Endurance_Test.su ./NandController/application/FactoryInvalidBlockScan_Test.cyclo ./NandController/application/FactoryInvalidBlockScan_Test.d ./NandController/application/FactoryInvalidBlockScan_Test.o ./NandController/application/FactoryInvalidBlockScan_Test.su ./NandController/application/Pattern.cyclo ./NandController/application/Pattern.d ./NandController/application/Pattern.o ./NandController/application/Pattern.su ./NandController/application/SinglePage_Test(Multiple\ Pattern).cyclo ./NandController/application/SinglePage_Test(Multiple\ Pattern).d ./NandController/application/SinglePage_Test(Multiple\ Pattern).o ./NandController/application/SinglePage_Test(Multiple\ Pattern).su ./NandController/application/SinglePage_Test(RandomLoadProgram).cyclo ./NandController/application/SinglePage_Test(RandomLoadProgram).d ./NandController/application/SinglePage_Test(RandomLoadProgram).o ./NandController/application/SinglePage_Test(RandomLoadProgram).su ./NandController/application/SinglePage_Test.cyclo ./NandController/application/SinglePage_Test.d ./NandController/application/SinglePage_Test.o ./NandController/application/SinglePage_Test.su

.PHONY: clean-NandController-2f-application

