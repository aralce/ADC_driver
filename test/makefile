all:
	@echo "========================="
	@echo "| ADC_driver UNIT TESTS |"
	@echo "========================="
	@$(MAKE) --no-print-directory -C tests_ADC_ADS1115_driver/
	@$(MAKE) --no-print-directory -C tests_ADC_MCP342x_driver/

clean:
	@$(MAKE) clean --no-print-directory -C tests_ADC_ADS1115_driver/
	@$(MAKE) clean --no-print-directory -C tests_ADC_MCP342x_driver/
	@echo "All tests were successfully removed."