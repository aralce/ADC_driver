#THIS MAKEFILE RUNS THE TESTS OF THE MODULE

all:
	@$(MAKE) --no-print-directory -C tests_HAL_ADC/
	@$(MAKE) --no-print-directory -C tests_HAL_EEPROM/
	@$(MAKE) --no-print-directory -C tests_HAL_GPIO/
	@$(MAKE) --no-print-directory -C tests_HAL_PWM/
	@$(MAKE) --no-print-directory -C tests_HAL_system/

clean:
	@echo Hardware_abstraction_layer: Build folders were removed.

	@$(MAKE) clean -s --no-print-directory -C tests_HAL_ADC/
	@$(MAKE) clean -s --no-print-directory -C tests_HAL_EEPROM/
	@$(MAKE) clean -s --no-print-directory -C tests_HAL_GPIO/
	@$(MAKE) clean -s --no-print-directory -C tests_HAL_PWM/
	@$(MAKE) clean -s --no-print-directory -C tests_HAL_system/