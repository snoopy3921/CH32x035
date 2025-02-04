Print = @echo

NAME_MODULE = CH32x035_Sample
PROJECT = $(NAME_MODULE)

OBJECTS_DIR = build_$(NAME_MODULE)
TARGET = $(OBJECTS_DIR)/$(NAME_MODULE).elf

##############################################################################
# Project files    #
##############################################################################
include sources/device/Makefile
include sources/application/Makefile

SHARED_DIR = ./platform/mik32v2-shared
HAL_DIR = ./platform/mik32-hal

SOURCES_ 		= $(notdir $(SOURCES))
OBJECTS 		+= $(addprefix $(OBJECTS_DIR)/, $(SOURCES_:.c=.o))

SOURCES_CPP_ 	= $(notdir $(SOURCES_CPP))
OBJECTS 		+= $(addprefix $(OBJECTS_DIR)/, $(SOURCES_CPP_:.cpp=.o))

SOURCES_ASM_ 	= $(notdir $(SOURCES_ASM))
OBJECTS		   += $(addprefix $(OBJECTS_DIR)/, $(SOURCES_ASM_:.S=.o))

##############################################################################


#|---------------------------------------------------------------------------------------------------|
#| OPTIMIZE LEVELS                                                                                   |
#|------------|----------------------------------|--------------|---------|------------|-------------|
#|   option   | optimization levels              |execution time|code size|memory usage|complile time|
#|------------|----------------------------------|--------------|---------|------------|-------------|
#|   -O0      | compilation time                 |     (+)      |   (+)   |     (-)    |    (-)      |
#| -O1 || -O  | code size && execution time      |     (-)      |   (-)   |     (+)    |    (+)      |
#|   -O2      | more code size && execution time |     (--)     |         |     (+)    |    (++)     |
#|   -O3      | more code size && execution time |     (---)    |         |     (+)    |    (+++)    |
#|   -Os      | code size                        |              |   (--)  |            |    (++)     |
#|  -Ofast    | O3 with none math cals           |     (---)    |         |     (+)    |    (+++)    |
#|------------|----------------------------------|--------------|---------|------------|-------------|
OPTIMIZE_OPTION = -Os

COMPILER_PATH	   = /opt/riscv

PROGRAMMER_PATH	   = /home/giahuy/tools/WCHISPTool_CMD/Linux/bin/x64/WCHISPTool_CMD

# The command for calling the compiler.
CC 		= $(COMPILER_PATH)/bin/riscv64-unknown-elf-gcc
CPP 	= $(COMPILER_PATH)/bin/riscv64-unknown-elf-g++
LD 		= $(COMPILER_PATH)/bin/riscv64-unknown-elf-ld
STRIP 	= $(COMPILER_PATH)/bin/riscv64-unknown-elf-strip
OBJCOPY = $(COMPILER_PATH)/bin/riscv64-unknown-elf-objcopy
OBJDUMP = $(COMPILER_PATH)/bin/riscv64-unknown-elf-objdump
OBJSIZE = $(COMPILER_PATH)/bin/riscv64-unknown-elf-size

LIBS += -lc -lm

# Set the compiler CPU/FPU options.
MARCH = -march=rv32imc_zicsr
MABI = 	-mabi=ilp32

GENERAL_FLAGS += $(OPTIMIZE_OPTION)

COMPILER_FLAGS += $(MARCH) $(MABI) -MD -fstrict-volatile-bitfields -fno-strict-aliasing -fno-common -fno-builtin-printf		

# C compiler flags
CFLAGS += $(GENERAL_FLAGS) $(COMPILER_FLAGS)

# C++ compiler flags
CPPFLAGS +=					\
		$(GENERAL_FLAGS)	\
		$(COMPILER_FLAGS)	\
		-fno-rtti			\
		-fno-exceptions		\
		-fno-use-cxa-atexit	\
		-std=gnu++14		\

# linker flags
LDFLAGS += 	-lgcc \
			-mcmodel=medlow \
			-nostartfiles \
			-ffreestanding \
			-Wl,-Bstatic,-T,$(LDFILE),-Map,$(OBJECTS_DIR)/$(PROJECT).map,--print-memory-usage \
			$(MARCH) $(MABI) \
			-lnosys \
			-lm \


# all : $(SOURCES) $(LDFILE) $(SOURCES_ASM)
# 	$(Print) $^ | tr ' ' '\n'
	
all: create $(TARGET)

create:
	$(Print) "           )\     *****************************"
	$(Print) "  c=======<||)    " CREATE $(OBJECTS_DIR) folder
	$(Print) "           )(     *****************************"			
	@mkdir -p $(OBJECTS_DIR) $(OBJECTS_DIR_C) $(OBJECTS_DIR_CPP) $(OBJECTS_DIR_ASM)

$(TARGET): $(OBJECTS)
	$(Print) LD $@
	@$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS) $(LIBS)
	$(Print) OBJCOPY $(@)
	@$(OBJCOPY) -O binary $(@) $(@:.elf=.bin)
	@$(OBJCOPY) -O ihex $(@) $(@:.elf=.hex)
	@$(OBJDUMP) -S -d $(@) > $(@:.elf=.asm)
	@$(OBJSIZE) $(TARGET)

$(OBJECTS_DIR)/%.o: %.c
	$(Print) CC $(notdir $@)
	@$(CC) -c $(CFLAGS) -o $@ $<
	@$(CC) -S $(CFLAGS) -o $@.disasm $<

$(OBJECTS_DIR)/%.o: %.cpp
	$(Print) CXX $(notdir $@)
	@$(CPP) -c $(CPPFLAGS) -o $@ $<

$(OBJECTS_DIR)/%.o: %.S
	$(Print) AS $(notdir $@)
	@$(CC) -c $(CFLAGS) -o $@ $< -D__ASSEMBLY__=1



.PHONY: clean
clean:
	$(Print) "        __         *****************************"
	$(Print) "      __\ \___     " CLEAN $(OBJECTS_DIR) folder
	$(Print) "      \ _ _ _ \    "	
	$(Print) "       \_\_\_\_\   *****************************"	
	@rm -rf $(OBJECTS_DIR)

.PHONY: com
com:
	minicom -D /dev/ttyUSB0 -b 9600 -c on 

.PHONY: flash
flash:
	chprog $(TARGET:.axf=.bin)