#
# Build all source files into one system image
#

# modify these next lines as necessary
IMAGE		=	system
LOADPORT	=	/dev/ttyACM0

# #############################################################################
# Don't touch anything below
# #############################################################################

OBJECTS		=	main.o serial.o
LDSCRIPT	=	atmega328p.ld

TOOLCHAIN	=	avr-
AS		=	$(TOOLCHAIN)as
LD		=	$(TOOLCHAIN)ld
OBJCOPY		=	$(TOOLCHAIN)objcopy

ASFLAGS		=	-mmcu=atmega328p -g
LDFLAGS		=	-T $(LDSCRIPT)
EMUFLAGS	=	-g -B 0 -d atmega128
LOADFLAGS	=	-q -q -c arduino -p m328p -P $(LOADPORT)

.PRECIOUS:	%.o %.elf
.PHONY:		all emu load console clean

# specific rules
all:	$(IMAGE).elf

emu:	$(IMAGE).bin
	simulavr $(EMUFLAGS) $(IMAGE).bin

load:	$(IMAGE).hex
	sudo avrdude $(LOADFLAGS) -U flash:w:$(IMAGE).hex

console:
	sudo chgrp root $(LOADPORT)
	sudo cu -l $(LOADPORT) -s 9600

clean:
	-rm *.elf *.bin *.hex *.o

$(IMAGE).elf:	$(LDSCRIPT) $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

# general rules
%.o:	%.s
	$(AS) $(ASFLAGS) -o $@ $^

%.bin:	%.elf
	$(OBJCOPY) --output-target binary $^ $@

%.hex:	%.elf
	$(OBJCOPY) --output-target ihex $^ $@

