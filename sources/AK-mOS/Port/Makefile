##############################################################################
# OS_CPU_OPTION:
#   - ARM_CM3  
#   - ARM_CM4_NO_FPU (Same as ARM_CM3)
# 	- RISC-V4C
##############################################################################
OS_CPU_OPTION = RISC_V4C


##############################################################################
# 			ARM Cortex M3       			     #
##############################################################################
ifeq ($(OS_CPU_OPTION), ARM_CM3) 
    CFLAGS   += -I./sources/AK-mOS/Port/ARM_CM3
    CPPFLAGS += -I./sources/AK-mOS/Port/ARM_CM3
    VPATH    += sources/AK-mOS/Port/ARM_CM3
    SOURCES  += sources/AK-mOS/Port/ARM_CM3/os_cpu.c

##############################################################################
# 			ARM M4 with no FPU				     #
##############################################################################
else ifeq ($(OS_CPU_OPTION), ARM_CM4_NO_FPU) 
    CFLAGS   += -I./sources/AK-mOS/Port/ARM_CM4_NO_FPU
    CPPFLAGS += -I./sources/AK-mOS/Port/ARM_CM4_NO_FPU
    VPATH    += sources/AK-mOS/Port/ARM_CM4_NO_FPU
    SOURCES  += sources/AK-mOS/Port/ARM_CM4_NO_FPU/os_cpu.c

##############################################################################
# 					RISC-V4C				    		     #
##############################################################################
else ifeq ($(OS_CPU_OPTION), RISC_V4C) 
    CFLAGS   += -I./sources/AK-mOS/Port/RISC_V4C
    CPPFLAGS += -I./sources/AK-mOS/Port/RISC_V4C
    VPATH    += sources/AK-mOS/Port/RISC_V4C
    SOURCES  += sources/AK-mOS/Port/RISC_V4C/os_cpu.c
    SOURCES_ASM += sources/AK-mOS/Port/RISC_V4C/os_cpu_asm.S
endif