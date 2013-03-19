# ustawienia

PROJECT = rtnet-uc
MCU  = cortex-m3
OBJPTH = ./obj 
LDSCRIPT = ./$(PROJECT).ld

SW_PTH = C:/StellarisWare
BD_PTH = $(SW_PTH)/boards/ek-lm3s6965
UTILS_PTH = $(SW_PTH)/utils

UINCDIR = $(SW_PTH) \
	$(BD_PTH) \
	$(BD_PTH)/drivers \
	$(SW_PTH)/driverlib \
	./inc
	
# List the user directory to look for the libraries here
ULIBDIR = C:/StellarisWare/driverlib/gcc-cm3 

# List all user libraries here
ULIBS = C:/StellarisWare/driverlib/gcc-cm3/libdriver-cm3.a

	

OBJS = obj/startup_gcc.o \
	obj/rtnet-uc.o \
	obj/stubs.o \
	obj/RIT128x96x4.o \
	obj/uRTnet/uRTnet.o \
	obj/uRTnet/rtmac.o \
	obj/uRTnet/rtcfg.o \
	obj/uRTnet/tdma.o \
	obj/hal/ethernet.o \
	obj/hal/system.o \
	obj/hal/timers.o \
	obj/debug/debug.o \
	obj/ustdlib.o \
	obj/debug/additional_functions.o 

# definicje

TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
SIZE = $(TRGT)size
BIN  = $(CP) -O ihex 


INCDIR  = $(patsubst %,-I%,$(DINCDIR) $(UINCDIR))
LIBDIR  = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))

DEFS    = $(DDEFS) $(UDEFS) -DRUN_FROM_FLASH=1

ADEFS   = $(DADEFS) $(UADEFS)
#OBJS    = $(ASRC:.s=.o) $(SRC:.c=.o)
LIBS    = $(DLIBS) $(ULIBS)
MCFLAGS = -mthumb -mcpu=$(MCU)

ASFLAGS = $(MCFLAGS) -g -gdwarf-2 -Wa,-amhls=$(<:.s=.lst) $(ADEFS)
CPFLAGS = $(MCFLAGS) $(OPT) $(FPU) -gdwarf-2 -Wall -Wstrict-prototypes -fverbose-asm -Wa, $(DEFS) 
LDFLAGS = $(MCFLAGS) -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(FULL_PRJ).map,--cref,--no-warn-mismatch $(LIBDIR)

# -ahlms=$(<:.c=.lst)
# reguly

all: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROJECT)_rom.elf

obj/rtnet-uc.o: rtnet-uc.c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) rtnet-uc.c -o obj/rtnet-uc.o

obj/startup_gcc.o: startup_gcc.c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) startup_gcc.c -o obj/startup_gcc.o
	
obj/stubs.o: src/stubs.c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) src/stubs.c -o obj/stubs.o
	
obj/RIT128x96x4.o: $(BD_PTH)/drivers/RIT128x96x4.c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) $(BD_PTH)/drivers/RIT128x96x4.c -o obj/RIT128x96x4.o
	
obj/ustdlib.o: $(UTILS_PTH)/ustdlib.c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) $(UTILS_PTH)/ustdlib.c -o obj/ustdlib.o
	
obj/uRTnet/uRTnet.o: src/uRTnet/uRTnet.c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) src/uRTnet/uRTnet.c -o obj/uRTnet/uRTnet.o	
	
obj/uRTnet/rtmac.o: src/uRTnet/rtmac.c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) src/uRTnet/rtmac.c -o obj/uRTnet/rtmac.o
	
obj/uRTnet/rtcfg.o: src/uRTnet/rtcfg.c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) src/uRTnet/rtcfg.c -o obj/uRTnet/rtcfg.o
	
obj/uRTnet/tdma.o: src/uRTnet/tdma.c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) src/uRTnet/tdma.c -o obj/uRTnet/tdma.o
	
obj/hal/ethernet.o: src/hal/ethernet.c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) src/hal/ethernet.c -o obj/hal/ethernet.o	
	
obj/hal/system.o: src/hal/system.c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) src/hal/system.c -o obj/hal/system.o
	
obj/hal/timers.o: src/hal/timers.c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) src/hal/timers.c -o obj/hal/timers.o
	
obj/debug/additional_functions.o: src/debug/additional_functions.c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) src/debug/additional_functions.c -o obj/debug/additional_functions.o
	
obj/debug/debug.o: src/debug/debug.c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) src/debug/debug.c -o obj/debug/debug.o
	
	
# rule for clean
clean: 
	rm -rf $(PROJECT)_rom.elf
	rm -rf $(OBJS)
