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
#include "rt_Process.h"

/* This implementation uses uVisor APIs. */
#include "uvisor-lib/uvisor-lib.h"

/*----------------------------------------------------------------------------
 *      Local Functions
 *---------------------------------------------------------------------------*/

/* Get ID of currently active process. */
/* Note: This function can only be called in privileged mode. */
U32 os_get_current_pid(void)
{
    return (U32) *(uvisor_privcall.active_box);
}

/* Switch processes, if needed. */
/* Note: This function can only be called in privileged mode. */
void rt_switch_processes(P_TCB p_TCB)
{
    U32 current_pid = os_get_current_pid();
    U32 dst_pid = p_TCB->pid;

    if (dst_pid != current_pid) {
        uvisor_privcall.switch_boxes(dst_pid);
    }
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
