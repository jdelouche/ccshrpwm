################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Example_2802xHRPWM.obj: ../Example_2802xHRPWM.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/Applications/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt -g --include_path="/Applications/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" --include_path="/packages/ti/xdais" --include_path="/Users/jdelouche/ti/controlSUITE/development_kits/C2000_LaunchPad" --include_path="/Users/jdelouche/ti/libs/math/IQmath/v15c/include" --define="_DEBUG" --define="_FLASH" --define="LARGE_MODEL" --quiet --verbose_diagnostics --diag_warning=225 --issue_remarks --output_all_syms --cdebug_asm_data --preproc_with_compile --preproc_dependency="Example_2802xHRPWM.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


