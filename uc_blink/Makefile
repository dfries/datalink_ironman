# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# WinAVR Makefile Template written by Eric B. Weddington, Jörg Wunsch, et al.
#
# Released to the Public Domain
#
# Additional material for this makefile was written by:
# Peter Fleury
# Tim Henigan
# Colin O'Flynn
# Reiner Patommel
# Markus Pfaff
# Sander Pool
# Frederik Rouleau
# Carlos Lamas
#
#----------------------------------------------------------------------------
# On command line:
#
# make all = Make software.
#
# make clean = Clean out built project files.
#
# make coff = Convert ELF to AVR COFF.
#
# make extcoff = Convert ELF to AVR Extended COFF.
#
# make program = Download the hex file to the device, using avrdude.
#                Please customize the avrdude settings below first!
#
# make debug = Start either simulavr or avarice as specified for debugging,
#              with avr-gdb or avr-insight as the front end for debugging.
#
# make filename.s = Just compile filename.c into the assembler code only.
#
# make filename.i = Create a preprocessed source file for use in submitting
#                   bug reports to the GCC project.
#
# To rebuild project do "make clean" then "make all".
#----------------------------------------------------------------------------

include mcu_type.txt

# make it lower case
LOWER_MCU := $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$(MCU)))))))))))))))))))))))))))

# Default target.
all: build

# Target file name (without extension).
TARGET = uc_blink


# List C source files here. (C dependencies are automatically generated.)
SRC =


# Output format. (can be srec, ihex, binary)
FORMAT = ihex


# Object files directory
#     To put object files in current directory, use a dot (.), do NOT make
#     this an empty or blank macro!
OBJDIR = .


# List C++ source files here. (C dependencies are automatically generated.)
CXXSRC = $(TARGET).cc
CPPSRC = \
cores/teensy3/EventResponder.cpp \
cores/teensy3/DMAChannel.cpp

SRC += cores/teensy3/pins_teensy.c \
cores/teensy3/analog.c \
cores/teensy3/mk20dx128.c \
cores/teensy3/usb_dev.c \
cores/teensy3/usb_serial.c \
cores/teensy3/usb_mem.c \
cores/teensy3/usb_desc.c \
cores/teensy3/nonstd.c


FLAGS=-DUSB_SERIAL

$(TARGET).elf:

# List Assembler source files here.
#     Make them always end in a capital .S.  Files ending in a lowercase .s
#     will not be considered source files but generated files (assembler
#     output from the compiler), and will be deleted upon "make clean"!
#     Even though the DOS/Win* filesystem matches both .s and .S the same,
#     it will preserve the spelling of the filenames, and gcc itself does
#     care about how the name is spelled on its command-line.
ASRC =


# Optimization level, can be [0, 1, 2, 3, s].
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s


# Debugging format.
#     Native formats for AVR-GCC's -g are dwarf-2 [default] or stabs.
#     AVR Studio 4.10 requires dwarf-2.
#     AVR [Extended] COFF format requires stabs, plus an avr-objcopy run.
DEBUG = dwarf-2


# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRAINCDIRS =


# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD = -std=gnu99
CPPSTANDARD = -std=c++11


# Place -D or -U options here for C sources
CDEFS = -DF_CPU=$(F_CPU)


# Place -D or -U options here for ASM sources
ADEFS = -DF_CPU=$(F_CPU)

# Place -D or -U options here for C++ sources
CPPDEFS = -DF_CPU=$(F_CPU)
#CPPDEFS += -D__STDC_LIMIT_MACROS
#CPPDEFS += -D__STDC_CONSTANT_MACROS


GENDEPFLAGS = -MMD -MP -MF .dep/$(@F).d
MCUOPTIONS += -mcpu=cortex-m4 -mthumb -D__$(MCU)__
# hardware single precision only, catch doubles
MCUOPTIONS += -Wdouble-promotion -fsingle-precision-constant
AFLAGS += $(MCUOPTIONS)
FLAGS += $(MCUOPTIONS) $(GENDEPFLAGS) -Wall -g -g$(DEBUG) -O$(OPT)
# FLAGS += -nostdlib
CFLAGS += $(FLAGS)
CPPFLAGS += $(FLAGS)
LDFLAGS += -O$(OPT) $(MCUOPTIONS) -Tcores/teensy3/$(LOWER_MCU).ld
LDLIBS +=

#---------------- Compiler Options C ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS += $(CDEFS)
#CFLAGS += -funsigned-char
#CFLAGS += -funsigned-bitfields
#CFLAGS += -ffunction-sections
#CFLAGS += -fpack-struct
#CFLAGS += -fshort-enums
CFLAGS += -Wstrict-prototypes
#CFLAGS += -mshort-calls
#CFLAGS += -fno-unit-at-a-time
#CFLAGS += -Wundef
#CFLAGS += -Wunreachable-code
#CFLAGS += -Wsign-compare
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)
CLST += -Wa,-adhlns=$(<:%.c=$(OBJDIR)/%.lst)


#---------------- Compiler Options C++ ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CPPFLAGS += $(CPPDEFS)
#CPPFLAGS += -funsigned-char
#CPPFLAGS += -funsigned-bitfields
#CPPFLAGS += -fpack-struct
#CPPFLAGS += -fshort-enums
#CPPFLAGS += -fno-exceptions
CPPFLAGS += -Wundef
CPPFLAGS += -felide-constructors
CPPFLAGS += -fno-exceptions
CPPFLAGS += -fno-rtti
#CPPFLAGS += -mshort-calls
#CPPFLAGS += -fno-unit-at-a-time
#CPPFLAGS += -Wstrict-prototypes
#CPPFLAGS += -Wunreachable-code
#CPPFLAGS += -Wsign-compare
CPPFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
#CPPFLAGS += $(CSTANDARD)
CPPFLAGS += $(CPPSTANDARD)
CPPLST += -Wa,-adhlns=$(patsubst %.cpp,$(OBJDIR)/%.lst,$(patsubst %.cc,$(OBJDIR)/%.lst,$<))


#---------------- Assembler Options ----------------
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -adhlns:   create listing
#  -gstabs:   have the assembler create line number information; note that
#             for use in COFF files, additional information about filenames
#             and function names needs to be present in the assembler source
#             files -- see avr-libc docs [FIXME: not yet described there]
#  -listing-cont-lines: Sets the maximum number of continuation lines of hex
#       dump that will be displayed for a given single line of source input.
ASFLAGS += $(ADEFS) -x assembler-with-cpp \
	-Wa,-adhlns=$(<:%.S=$(OBJDIR)/%.lst),-gstabs,--listing-cont-lines=100


#---------------- Library Options ----------------
# Minimalistic printf version
PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires MATH_LIB = -lm below)
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt

# If this is left blank, then it will use the Standard printf version.
PRINTF_LIB =
#PRINTF_LIB = $(PRINTF_LIB_MIN)
#PRINTF_LIB = $(PRINTF_LIB_FLOAT)


# Minimalistic scanf version
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point + %[ scanf version (requires MATH_LIB = -lm below)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt

# If this is left blank, then it will use the Standard scanf version.
SCANF_LIB =
#SCANF_LIB = $(SCANF_LIB_MIN)
#SCANF_LIB = $(SCANF_LIB_FLOAT)


MATH_LIB = -lm


# List any extra directories to look for libraries here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRALIBDIRS =



#---------------- External Memory Options ----------------

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS =



#---------------- Linker Options ----------------
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
#LDFLAGS += -Wl,-Map=$(TARGET).map,--cref
#LDFLAGS += -Wl,--relax
LDFLAGS += -Wl,--gc-sections
LDFLAGS += $(EXTMEMOPTS)
LDFLAGS += $(patsubst %,-L%,$(EXTRALIBDIRS))
#LDFLAGS += -T linker_script.x
LDLIBS += $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)
LDLIBS += --specs=nano.specs


#---------------- Programming Options --------------------------

LOADER = teensy_loader_cli
# strip off underscores
LOADER_OPTIONS = --mcu=$(LOWER_MCU) -s


#---------------- Debugging Options ----------------

# For simulavr only - target MCU frequency.
DEBUG_MFREQ = $(F_CPU)

# Set the DEBUG_UI to either gdb or insight.
# DEBUG_UI = gdb
DEBUG_UI = insight

# Set the debugging back-end to either avarice, simulavr.
DEBUG_BACKEND = avarice
#DEBUG_BACKEND = simulavr

# GDB Init Filename.
GDBINIT_FILE = __avr_gdbinit

# When using avarice settings for the JTAG
JTAG_DEV = /dev/com1

# Debugging port used to communicate between GDB / avarice / simulavr.
DEBUG_PORT = 4242

# Debugging host used to communicate between GDB / avarice / simulavr, normally
#     just set to localhost unless doing some sort of crazy debugging when
#     avarice is running on a different computer.
DEBUG_HOST = localhost



#============================================================================


# Define programs and commands.
CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size
AR = arm-none-eabi-ar rcs
NM = arm-none-eabi-nm
AVRDUDE = avrdude
REMOVE = rm -f
REMOVEDIR = rm -rf
COPY = cp


# Define Messages
# English
MSG_SIZE_BEFORE = Size before:
MSG_SIZE_AFTER = Size after:
MSG_SIZE_MATCH = file is identical
MSG_CLEANING = Cleaning project:




# Define all object files.
OBJ = $(SRC:%.c=$(OBJDIR)/%.o) $(CXXSRC:%.cc=$(OBJDIR)/%.o)  $(CPPSRC:%.cpp=$(OBJDIR)/%.o) $(ASRC:%.S=$(OBJDIR)/%.o)

# Define all listing files.
LST = $(SRC:%.c=$(OBJDIR)/%.lst) $(CXXSRC:%.cc=$(OBJDIR)/%.lst)  $(CPPSRC:%.cpp=$(OBJDIR)/%.lst) $(ASRC:%.S=$(OBJDIR)/%.lst)



# Change the build target to build a HEX file or a library.
build: sizebefore elf hex lss sizeafter
#build: lib


elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss
sym: $(TARGET).sym
LIBNAME=lib$(TARGET).a
lib: $(LIBNAME)



# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT)
#ELFSIZE = $(SIZE) --mcu=$(MCU) --format=avr
ELFSIZE = $(SIZE)

# This leaves $(TARGET).elf.old alone if $(TARGET).elf doesn't exist, instead
# of deleting it because it is still useful to compare with an earlier version.
sizebefore:
	@cp $(TARGET).elf $(TARGET).elf.old 2> /dev/null || true

sizeafter: $(TARGET).elf
	@if test -f $(TARGET).elf; then \
	if test -f $(TARGET).elf.old; then \
		echo $(MSG_SIZE_BEFORE); $(ELFSIZE) $(TARGET).elf.old; fi; \
	if cmp --quiet $(TARGET).elf.old $(TARGET).elf; then \
		echo $(MSG_SIZE_AFTER) $(MSG_SIZE_MATCH); else \
		echo $(MSG_SIZE_AFTER); $(ELFSIZE) $(TARGET).elf; \
	fi 2>/dev/null; fi



# Program the device.
program: sizebefore $(TARGET).hex sizeafter program_no_size

program_no_size: $(TARGET).hex
	$(LOADER) $(LOADER_OPTIONS) $<


# Generate avr-gdb config/init file which does the following:
#     define the reset signal, load the target file, connect to target, and set
#     a breakpoint at main().
gdb-config:
	@$(REMOVE) $(GDBINIT_FILE)
	@echo define reset >> $(GDBINIT_FILE)
	@echo SIGNAL SIGHUP >> $(GDBINIT_FILE)
	@echo end >> $(GDBINIT_FILE)
	@echo file $(TARGET).elf >> $(GDBINIT_FILE)
	@echo target remote $(DEBUG_HOST):$(DEBUG_PORT)  >> $(GDBINIT_FILE)
ifeq ($(DEBUG_BACKEND),simulavr)
	@echo load  >> $(GDBINIT_FILE)
endif
	@echo break main >> $(GDBINIT_FILE)



# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT = $(OBJCOPY) --debugging
COFFCONVERT += --change-section-address .data-0x800000
COFFCONVERT += --change-section-address .bss-0x800000
COFFCONVERT += --change-section-address .noinit-0x800000
COFFCONVERT += --change-section-address .eeprom-0x810000



coff: $(TARGET).elf
	$(COFFCONVERT) -O coff-avr $< $(TARGET).cof


extcoff: $(TARGET).elf
	$(COFFCONVERT) -O coff-ext-avr $< $(TARGET).cof



# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.eep: %.elf
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0

# Create extended listing file from ELF output file.
%.lss: %.elf
	$(OBJDUMP) -h -S -z $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	$(NM) -n $< > $@



# Create library from object files.
.SECONDARY : $(TARGET).a
.PRECIOUS : $(OBJ)
%.a: $(OBJ)
	@#echo
	@#echo $(MSG_CREATING_LIBRARY) $@
	$(AR) $@ $(OBJ)


# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

# Compile: create object files from C source files.
$(OBJDIR)/%.o : %.c
	$(CC) -c $(CLST) $(CFLAGS) $< -o $@

# Compile: create assembler files from C source files.
%.s : %.c
	$(CC) -S $(CLST) $(CFLAGS) $< -o $@

# Compile: create object files from C++ source files.
$(OBJDIR)/%.o : %.cc
	$(CXX) -c $(CPPLST) $(CPPFLAGS) $< -o $@

# Compile: create object files from C++ source files.
$(OBJDIR)/%.o : %.cpp
	$(CXX) -c $(CPPLST) $(CPPFLAGS) $< -o $@

# Compile: create assembler files from C++ source files.
%.s : %.cc
	$(CC) -S $(CPPFLAGS) $< -o $@

# Compile: create assembler files from C++ source files.
%.s : %.cpp
	$(CC) -S $(CPPFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
$(OBJDIR)/%.o : %.S
	$(CC) -c $(ASFLAGS) $< -o $@


# Create preprocessed source for use in sending a bug report.
%.i : %.c
	$(CC) -E -mmcu=$(MCU) $(CFLAGS) $< -o $@


# Target: clean project.
clean: clean_list

clean_list :
	@#echo
	@#echo $(MSG_CLEANING)
	$(REMOVE) $(TARGET).hex \
	 $(TARGET).eep \
	 $(TARGET).cof \
	 $(TARGET).elf $(TARGET).elf.old \
	 $(TARGET).map \
	 $(TARGET).sym \
	 $(TARGET).lss \
	 $(LIBNAME);
	if [ -n "$(SRC)" ] ; then \
	$(REMOVE) $(SRC:%.c=$(OBJDIR)/%.o) \
	 $(SRC:%.c=$(OBJDIR)/%.lst) \
	 $(SRC:.c=.s) \
	 $(SRC:.c=.d) \
	 $(SRC:.c=.i); \
	fi
	if [ -n "$(CXXSRC)" ] ; then \
	$(REMOVE) $(CXXSRC:%.cc=$(OBJDIR)/%.o) \
	 $(CXXSRC:%.cc=$(OBJDIR)/%.lst) \
	 $(CXXSRC:.cc=.s) \
	 $(CXXSRC:.cc=.d) \
	 $(CXXSRC:.cc=.i); \
	fi
	if [ -n "$(CPPSRC)" ] ; then \
	$(REMOVE) $(CPPSRC:%.cpp=$(OBJDIR)/%.o) \
	 $(CPPSRC:%.cpp=$(OBJDIR)/%.lst) \
	 $(CPPSRC:.cpp=.s) \
	 $(CPPSRC:.cpp=.d) \
	 $(CPPSRC:.cpp=.i); \
	fi
	$(REMOVEDIR) .dep


# Create object files directory
$(shell mkdir $(OBJDIR) 2>/dev/null)


# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


# Listing of phony targets.
.PHONY : all sizebefore sizeafter \
build elf hex eep lss sym coff extcoff \
clean clean_list program program_no_size debug gdb-config
