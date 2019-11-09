CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
GDB = arm-none-eabi-gdb
SIZE = arm-none-eabi-size

# Project settings
exec := bloom.elf
arch := -mcpu=cortex-m0 -mthumb
spec := -nostartfiles --specs=nosys.specs --specs=nano.specs
opt  := -Os
misc := -Wall -g2
libs :=
link := -Tmcu/stm32f030.ld

# Source files
objects += $(addprefix mcu/, cortex.o rcc.o gpio.o i2c.o timer.o)
objects += $(addprefix emu/, onewire.o)
objects += $(addprefix core/, main.o screens.o settings.o actions.o)
objects += $(addprefix drivers/, ssd1306.o ds18b20.o pcf8563.o)
objects += $(addprefix board/, board.o)
objects += $(addprefix res/, font_8x16.o font_12x20_digits.o font_7x9.o)
objects += $(addprefix utils/, time.o tasks.o)

# Header directories
inc += -I. -Ivendor

include Makefile.arm-generic

