################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
isolated_sram_test.lss \

FLASH_IMAGE += \
isolated_sram_test.hex \

EEPROM_IMAGE += \
isolated_sram_test.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: isolated_sram_test.elf secondary-outputs

# Tool invocations
isolated_sram_test.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,isolated_sram_test.map -Wl,--section-start=.classb_sram_buffer=0x800100,--section-start=.data=0x800240 -mmcu=atmega328p -o "isolated_sram_test.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

isolated_sram_test.lss: isolated_sram_test.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S isolated_sram_test.elf  >"isolated_sram_test.lss"
	@echo 'Finished building: $@'
	@echo ' '

isolated_sram_test.hex: isolated_sram_test.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -R .classb_sram_buffer -O ihex isolated_sram_test.elf  "isolated_sram_test.hex"
	@echo 'Finished building: $@'
	@echo ' '

isolated_sram_test.eep: isolated_sram_test.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex isolated_sram_test.elf  "isolated_sram_test.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: isolated_sram_test.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p isolated_sram_test.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) isolated_sram_test.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
