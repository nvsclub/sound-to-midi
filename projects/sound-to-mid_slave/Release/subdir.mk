################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../i2c_library.c \
../main.c \
../mem_eeprom_test.c \
../mem_flash_test.c \
../mem_sram_test.c \
../microphone.c \
../spi_library.c 

OBJS += \
./i2c_library.o \
./main.o \
./mem_eeprom_test.o \
./mem_flash_test.o \
./mem_sram_test.o \
./microphone.o \
./spi_library.o 

C_DEPS += \
./i2c_library.d \
./main.d \
./mem_eeprom_test.d \
./mem_flash_test.d \
./mem_sram_test.d \
./microphone.d \
./spi_library.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


