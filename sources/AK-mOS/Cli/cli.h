#ifndef CLI_H
#define CLI_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "os_cfg.h"

#define CLI_PRINT(fmt, ...) USER_PRINT(fmt, ##__VA_ARGS__)

typedef struct  {
    /**
     * Name of the command.
     */
    const char *cmd_name;

    /**
     * Description string for this cmd is executed.
     * Can be NULL if no help is provided.
     */
    const char *help;

     /**
     * Pointer to any specific app context that is required for this binding.
     * It will be provided in binding callback.
     */
    void *context;
    
    /**
     * A function will be executed when command is received.
     * If null, default callback (onCommand) will be called.
     */
    void (*func_cb)(uint8_t argc, char *args, void *context);
}cli_cmd_t;

extern void cli_init(void);


#define cli_new_line_carriage_return()    USER_PRINT("\r\n");
#define cli_carriage_return()             USER_PRINT("\r");

#ifdef __cplusplus
}
#endif
#endif /* CLI_H */