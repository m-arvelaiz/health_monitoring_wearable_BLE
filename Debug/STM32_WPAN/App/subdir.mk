################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32_WPAN/App/app_ble.c \
../STM32_WPAN/App/ble_custom_utils.c \
../STM32_WPAN/App/ble_hm_data.c \
../STM32_WPAN/App/custom_app.c \
../STM32_WPAN/App/custom_stm.c \
../STM32_WPAN/App/data_handler.c 

OBJS += \
./STM32_WPAN/App/app_ble.o \
./STM32_WPAN/App/ble_custom_utils.o \
./STM32_WPAN/App/ble_hm_data.o \
./STM32_WPAN/App/custom_app.o \
./STM32_WPAN/App/custom_stm.o \
./STM32_WPAN/App/data_handler.o 

C_DEPS += \
./STM32_WPAN/App/app_ble.d \
./STM32_WPAN/App/ble_custom_utils.d \
./STM32_WPAN/App/ble_hm_data.d \
./STM32_WPAN/App/custom_app.d \
./STM32_WPAN/App/custom_stm.d \
./STM32_WPAN/App/data_handler.d 


# Each subdirectory must supply rules for building sources it contributes
STM32_WPAN/App/%.o STM32_WPAN/App/%.su STM32_WPAN/App/%.cyclo: ../STM32_WPAN/App/%.c STM32_WPAN/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB5Mxx -c -I../Core/Inc -I../STM32_WPAN/App -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Utilities/lpm/tiny_lpm -I../Middlewares/ST/STM32_WPAN -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../Middlewares/ST/STM32_WPAN/utilities -I../Middlewares/ST/STM32_WPAN/ble/core -I../Middlewares/ST/STM32_WPAN/ble/core/auto -I../Middlewares/ST/STM32_WPAN/ble/core/template -I../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Utilities/sequencer -I../Middlewares/ST/STM32_WPAN/ble -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-STM32_WPAN-2f-App

clean-STM32_WPAN-2f-App:
	-$(RM) ./STM32_WPAN/App/app_ble.cyclo ./STM32_WPAN/App/app_ble.d ./STM32_WPAN/App/app_ble.o ./STM32_WPAN/App/app_ble.su ./STM32_WPAN/App/ble_custom_utils.cyclo ./STM32_WPAN/App/ble_custom_utils.d ./STM32_WPAN/App/ble_custom_utils.o ./STM32_WPAN/App/ble_custom_utils.su ./STM32_WPAN/App/ble_hm_data.cyclo ./STM32_WPAN/App/ble_hm_data.d ./STM32_WPAN/App/ble_hm_data.o ./STM32_WPAN/App/ble_hm_data.su ./STM32_WPAN/App/custom_app.cyclo ./STM32_WPAN/App/custom_app.d ./STM32_WPAN/App/custom_app.o ./STM32_WPAN/App/custom_app.su ./STM32_WPAN/App/custom_stm.cyclo ./STM32_WPAN/App/custom_stm.d ./STM32_WPAN/App/custom_stm.o ./STM32_WPAN/App/custom_stm.su ./STM32_WPAN/App/data_handler.cyclo ./STM32_WPAN/App/data_handler.d ./STM32_WPAN/App/data_handler.o ./STM32_WPAN/App/data_handler.su

.PHONY: clean-STM32_WPAN-2f-App

