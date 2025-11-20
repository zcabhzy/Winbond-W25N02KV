################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FTLController/Src/Cache.c \
../FTLController/Src/FlashTranslationLayer.c \
../FTLController/Src/GarbageCollection.c \
../FTLController/Src/Invalidata.c \
../FTLController/Src/MappingTable.c \
../FTLController/Src/WearLeveling.c 

OBJS += \
./FTLController/Src/Cache.o \
./FTLController/Src/FlashTranslationLayer.o \
./FTLController/Src/GarbageCollection.o \
./FTLController/Src/Invalidata.o \
./FTLController/Src/MappingTable.o \
./FTLController/Src/WearLeveling.o 

C_DEPS += \
./FTLController/Src/Cache.d \
./FTLController/Src/FlashTranslationLayer.d \
./FTLController/Src/GarbageCollection.d \
./FTLController/Src/Invalidata.d \
./FTLController/Src/MappingTable.d \
./FTLController/Src/WearLeveling.d 


# Each subdirectory must supply rules for building sources it contributes
FTLController/Src/%.o FTLController/Src/%.su FTLController/Src/%.cyclo: ../FTLController/Src/%.c FTLController/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/User/STM32CubeIDE/workspace_1.18.1/USB_MassStorage/CM7/NandController/Inc" -I"C:/Users/User/STM32CubeIDE/workspace_1.18.1/USB_MassStorage/CM7/FTLController/Inc" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/User/STM32CubeIDE/workspace_1.18.1/USB_MassStorage/CM7/NandController/driver" -I"C:/Users/User/STM32CubeIDE/workspace_1.18.1/USB_MassStorage/CM7/NandController/service" -I"C:/Users/User/STM32CubeIDE/workspace_1.18.1/USB_MassStorage/CM7/NandController/application" -I"C:/Users/User/STM32CubeIDE/workspace_1.18.1/USB_MassStorage/CM7/NandController/hal" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FTLController-2f-Src

clean-FTLController-2f-Src:
	-$(RM) ./FTLController/Src/Cache.cyclo ./FTLController/Src/Cache.d ./FTLController/Src/Cache.o ./FTLController/Src/Cache.su ./FTLController/Src/FlashTranslationLayer.cyclo ./FTLController/Src/FlashTranslationLayer.d ./FTLController/Src/FlashTranslationLayer.o ./FTLController/Src/FlashTranslationLayer.su ./FTLController/Src/GarbageCollection.cyclo ./FTLController/Src/GarbageCollection.d ./FTLController/Src/GarbageCollection.o ./FTLController/Src/GarbageCollection.su ./FTLController/Src/Invalidata.cyclo ./FTLController/Src/Invalidata.d ./FTLController/Src/Invalidata.o ./FTLController/Src/Invalidata.su ./FTLController/Src/MappingTable.cyclo ./FTLController/Src/MappingTable.d ./FTLController/Src/MappingTable.o ./FTLController/Src/MappingTable.su ./FTLController/Src/WearLeveling.cyclo ./FTLController/Src/WearLeveling.d ./FTLController/Src/WearLeveling.o ./FTLController/Src/WearLeveling.su

.PHONY: clean-FTLController-2f-Src

