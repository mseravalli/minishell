################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/listManager.c \
../src/parser.c \
../src/processes.c \
../src/shellOS.c \
../src/signalHandlers.c 

OBJS += \
./src/listManager.o \
./src/parser.o \
./src/processes.o \
./src/shellOS.o \
./src/signalHandlers.o 

C_DEPS += \
./src/listManager.d \
./src/parser.d \
./src/processes.d \
./src/shellOS.d \
./src/signalHandlers.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


