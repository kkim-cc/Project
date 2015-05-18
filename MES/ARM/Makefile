OBJECTS		=	main.o serial.o

TOOLCHAIN	=	arm-linux-gnueabi-
AS		=	$(TOOLCHAIN)as
LD		=	$(TOOLCHAIN)ld
OBJCOPY		=	$(TOOLCHAIN)objcopy
EMU		=	qemu-system-arm
DEBUGGER	=	gdb-multiarch

ASFLAGS		=	-march=armv4t
EMUFLAGS_DBG	=	-s -S
EMUFLAGS	=	-M integratorcp -nographic


.PHONY:	all clean emu run debug

all:	system.elf

clean:
	-rm *.o *.elf *.bin *.gdb

emu:	system.bin
	$(EMU) $(EMUFLAGS) $(EMUFLAGS_DBG) -kernel $<

run:	system.bin
	$(EMU) $(EMUFLAGS) -kernel $<

# Setup for debugging: load symbols, connect to target, step past the bootrom
debug:
	@echo "set arch arm" > system.gdb
	@echo "sym system.elf" >> system.gdb
	@echo "target remote localhost:1234" >> system.gdb
	@echo "si 4" >> system.gdb
	$(DEBUGGER) -q -x system.gdb

system.elf:	qemu.ld $(OBJECTS)
	$(LD) -T qemu.ld -o $@ $(OBJECTS)

%.o:	%.s
	$(AS) $(ASFLAGS) -g -o $@ $<

%.bin:	%.elf
	$(OBJCOPY) -O binary $< $@

