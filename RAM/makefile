################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /Applications/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Example_2802xHRPWM.obj" \
"/Users/jdelouche/ti/controlSUITE/development_kits/C2000_LaunchPad/f2802x_headers/cmd/F2802x_Headers_nonBIOS.cmd" \
"/Users/jdelouche/ti/controlSUITE/development_kits/C2000_LaunchPad/f2802x_common/lib/driverlib.lib" \
$(GEN_CMDS__FLAG) \
-l"/Users/jdelouche/ti/controlSUITE/development_kits/C2000_LaunchPad/F2802x_common/cmd/F2802x_generic_ram.cmd" \
-lrts2800_ml.lib \
-lIQmath.lib \
-llibc.a \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Example_2802xHRPWM.out \

EXE_OUTPUTS__QUOTED += \
"Example_2802xHRPWM.out" \

BIN_OUTPUTS += \
Example_2802xHRPWM.hex \

BIN_OUTPUTS__QUOTED += \
"Example_2802xHRPWM.hex" \


# All Target
all: Example_2802xHRPWM.out

# Tool invocations
Example_2802xHRPWM.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C2000 Linker'
	"/Applications/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt -g --define="_DEBUG" --define="LARGE_MODEL" --quiet --issue_remarks --verbose_diagnostics --diag_warning=225 --output_all_syms --cdebug_asm_data -z -m"Example_2802xHRPWM.map" --stack_size=0x200 --warn_sections -i"/Applications/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/lib" -i"/Applications/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" -i"/Users/jdelouche/ti/controlSUITE/development_kits/C2000_LaunchPad/f2802x_examples/whrpwm" -i"/Users/jdelouche/ti/controlSUITE/development_kits/C2000_LaunchPad/f2802x_common/lib" -i"/Users/jdelouche/ti/libs/math/IQmath/v15c/lib" --reread_libs --verbose_diagnostics --issue_remarks --xml_link_info="Example_2802xHRPWM_linkInfo.xml" --entry_point=code_start --rom_model -o "Example_2802xHRPWM.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

Example_2802xHRPWM.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: C2000 Hex Utility'
	"/Applications/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/hex2000"  -o "Example_2802xHRPWM.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "Example_2802xHRPWM.obj" 
	-$(RM) "Example_2802xHRPWM.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

