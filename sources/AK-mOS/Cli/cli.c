#include "cli.h"

typedef struct
{
      char *buf;
      /**
       * Position of first element in buffer. From this position elements are
       * taken
       */
      uint16_t front;
      /**
       * Position after last element. At this position new elements are inserted
       */
      uint16_t back;
      /**
       * Size of buffer
       */
      uint16_t size;
} buff_queue_t;


void cli_init(void)
{
	CLI_PRINT("\n");
	CLI_PRINT("    ___    __ __              ____  _____\n");
	CLI_PRINT("   /   |  / //_/   ____ ___  / __ \\/ ___/\n");
	CLI_PRINT("  / /| | / .<     / __ `__ \\/ / / /\\__ \\\n");
	CLI_PRINT(" / ___ |/ /| |   / / / / / / /_/ /___/ /\n");
      CLI_PRINT("/_/  |_/_/ |_|  /_/ /_/ /_/\\____//____/\n");
      CLI_PRINT("\n");
	CLI_PRINT(" Welcome to Active Kernel mini RTOS!\n");
}