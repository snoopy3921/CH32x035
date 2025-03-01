#include "task_list.h"

const task_t app_task_table[] = {
    /*************************************************************************/
    /* TASK */
    /* TASK_ID          task_func     arg     prio   msg_queue_size    stk_size */
    /*************************************************************************/
    {TASK_1_ID,   	    task_1,       NULL,   1,      8,                150},
    {TASK_2_ID,   	    task_2,       NULL,   1,      8,                150}, 
    {TASK_3_ID,   	    task_3,       NULL,   1,      8,                150}, 
};
