include sources/AK-mOS/Cli/Makefile
include sources/AK-mOS/Port/Makefile

CFLAGS	+= -I./sources/AK-mOS/Inc
CPPFLAGS	+= -I./sources/AK-mOS/Inc

CFLAGS	+= -I./sources/AK-mOS
CPPFLAGS	+= -I./sources/AK-mOS

VPATH += sources/AK-mOS
VPATH += sources/AK-mOS/Src

SOURCES_CPP += sources/AK-mOS/task_list.cpp
SOURCES_CPP += sources/AK-mOS/cli_cmd_list.cpp

SOURCES += sources/AK-mOS/Src/os_kernel.c
SOURCES += sources/AK-mOS/Src/os_list.c
SOURCES += sources/AK-mOS/Src/os_mem.c
SOURCES += sources/AK-mOS/Src/os_msg.c
SOURCES += sources/AK-mOS/Src/os_prio.c
SOURCES += sources/AK-mOS/Src/os_task.c
SOURCES += sources/AK-mOS/Src/os_timer.c