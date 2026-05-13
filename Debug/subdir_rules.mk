################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Clock.o: /Users/rachitkakkar/Documents/Archive/College\ Work/ECE\ 319H/MSPM0_ValvanoWare/inc/Clock.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs2040/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H" -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug" -I"/Applications/ti/mspm0_sdk_2_09_00_01/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_09_00_01/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"Clock.d_raw" -MT"Clock.o" -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug/syscfg" -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DAC5.o: /Users/rachitkakkar/Documents/Archive/College\ Work/ECE\ 319H/MSPM0_ValvanoWare/inc/DAC5.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs2040/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H" -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug" -I"/Applications/ti/mspm0_sdk_2_09_00_01/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_09_00_01/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"DAC5.d_raw" -MT"DAC5.o" -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug/syscfg" -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.o: ../%.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs2040/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H" -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug" -I"/Applications/ti/mspm0_sdk_2_09_00_01/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_09_00_01/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

LaunchPad.o: /Users/rachitkakkar/Documents/Archive/College\ Work/ECE\ 319H/MSPM0_ValvanoWare/inc/LaunchPad.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs2040/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H" -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug" -I"/Applications/ti/mspm0_sdk_2_09_00_01/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_09_00_01/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"LaunchPad.d_raw" -MT"LaunchPad.o" -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

SPI.o: /Users/rachitkakkar/Documents/Archive/College\ Work/ECE\ 319H/MSPM0_ValvanoWare/inc/SPI.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs2040/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H" -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug" -I"/Applications/ti/mspm0_sdk_2_09_00_01/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_09_00_01/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"SPI.d_raw" -MT"SPI.o" -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

ST7735.o: /Users/rachitkakkar/Documents/Archive/College\ Work/ECE\ 319H/MSPM0_ValvanoWare/inc/ST7735.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs2040/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H" -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug" -I"/Applications/ti/mspm0_sdk_2_09_00_01/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_09_00_01/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"ST7735.d_raw" -MT"ST7735.o" -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

SlidePot.o: /Users/rachitkakkar/Documents/Archive/College\ Work/ECE\ 319H/MSPM0_ValvanoWare/inc/SlidePot.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs2040/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H" -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug" -I"/Applications/ti/mspm0_sdk_2_09_00_01/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_09_00_01/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"SlidePot.d_raw" -MT"SlidePot.o" -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

TExaS.o: /Users/rachitkakkar/Documents/Archive/College\ Work/ECE\ 319H/MSPM0_ValvanoWare/inc/TExaS.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs2040/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H" -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug" -I"/Applications/ti/mspm0_sdk_2_09_00_01/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_09_00_01/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"TExaS.d_raw" -MT"TExaS.o" -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Timer.o: /Users/rachitkakkar/Documents/Archive/College\ Work/ECE\ 319H/MSPM0_ValvanoWare/inc/Timer.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs2040/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H" -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug" -I"/Applications/ti/mspm0_sdk_2_09_00_01/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_09_00_01/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"Timer.d_raw" -MT"Timer.o" -I"/Users/rachitkakkar/Documents/Archive/College Work/ECE 319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


