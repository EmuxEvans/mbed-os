/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_Task.h"
#include "rt_List.h"
#include "rt_MemBox.h"
#include "rt_Robin.h"
#include "rt_HAL_CM.h"
#include "rt_Process.h"

/* This implementation uses uVisor APIs. */
#include "uvisor-lib/uvisor-lib.h"

/*----------------------------------------------------------------------------
 *      Local Functions
 *---------------------------------------------------------------------------*/

/*--------------------------- rt_proc_self ----------------------------------*/

OS_PID rt_proc_self(void)
{
    /* Return own process identifier value. */
    return (OS_PID) *(uvisor_privcall.active_box);
}

OS_PID rt_proc_from_tsk(P_TCB p_TCB)
{
    /* Return task's process identifier value. */
    return (OS_PID) p_TCB->process_id;
}

OS_RESULT rt_proc_switch(OS_PID dst_proc)
{
    OS_PID src_proc;

    src_proc = rt_proc_self();
    if (dst_proc == -1 || src_proc == -1) {
       return OS_R_NOK;
    } else if (src_proc != dst_proc) {
        uvisor_privcall.box_switch((uint8_t) (dst_proc & 0xFF));
    }
    return OS_R_OK;
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
