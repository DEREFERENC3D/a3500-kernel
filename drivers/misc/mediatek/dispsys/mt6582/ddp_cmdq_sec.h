#ifndef __DDP_CMDQ_SEC_H__
#define __DDP_CMDQ_SEC_H__

#include "ddp_cmdq.h"

/**
 * Callback to fill message buffer for secure task
 *
 * Params:
 *     init32_t command id
 *     void*    pIwc, the inter-world communication buffer
 * Return:
 *     >=0 for success;
 */ 
typedef int32_t (*CmdqSecFillIwcCB)(int32_t, void*);


int32_t cmdq_exec_task_secure_with_retry(TaskStruct *pTask, int32_t thread, const uint32_t retry); 

// TODO: refactor sw copy by using atomic variable
void cmdq_debug_set_sw_copy(int32_t value);
int32_t cmdq_debug_get_sw_copy(void);

// TODO: move cmdqSubmitTaskSecure to ddp_cmdq_sec
#endif //__DDP_CMDQ_SEC_H__
