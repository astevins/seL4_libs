/*
 * Copyright 2017, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */

#pragma once

/* This file provides backwards compatible API wrappers for seemlessly switching between the MCS kernel
   and non-MCS kernel without too many #ifdefs and without confusing cscope and other analysis tools.

   Of course, if any of the new functionality of the MCS kernel is leveraged, the switch won't be
   seemless.
*/

#include <autoconf.h>
#include <sel4/sel4.h>

#ifdef CONFIG_KERNEL_RT
#define CONFIG_TIMER_TICK_MS CONFIG_BOOT_THREAD_TIME_SLICE
#else
#define CONFIG_BOOT_THREAD_TIME_SLICE CONFIG_TIMER_TICK_MS
#endif

static inline seL4_MessageInfo_t api_wait(seL4_CPtr ep, seL4_Word *badge)
{
#ifdef CONFIG_KERNEL_RT
    return seL4_Wait(ep, badge);
#else
    return seL4_Recv(ep, badge);
#endif
}

static inline seL4_MessageInfo_t api_recv(seL4_CPtr ep, seL4_Word *badge, UNUSED seL4_CPtr reply)
{
#ifdef CONFIG_KERNEL_RT
    return seL4_Recv(ep, badge, reply);
#else
    return seL4_Recv(ep, badge);
#endif
}

static inline seL4_MessageInfo_t api_nbrecv(seL4_CPtr ep, seL4_Word *badge, UNUSED seL4_CPtr reply)
{
#ifdef CONFIG_KERNEL_RT
    return seL4_NBRecv(ep, badge, reply);
#else
    return seL4_NBRecv(ep, badge);
#endif
}

static inline void api_reply(UNUSED seL4_CPtr reply, seL4_MessageInfo_t msg)
{
#ifdef CONFIG_KERNEL_RT
    seL4_Send(reply, msg);
#else
    seL4_Reply(msg);
#endif
}

static inline seL4_MessageInfo_t api_reply_recv(seL4_CPtr ep, seL4_MessageInfo_t msg, seL4_Word *badge,
        UNUSED seL4_CPtr reply)
{
#ifdef CONFIG_KERNEL_RT
    return seL4_ReplyRecv(ep, msg, badge, reply);
#else
    return seL4_ReplyRecv(ep, msg, badge);
#endif
}

static inline seL4_Error api_tcb_configure(seL4_CPtr tcb, seL4_CPtr ep, UNUSED seL4_CPtr timeout_ep,
                                           seL4_PrioProps_t props, UNUSED seL4_CPtr sc, seL4_CPtr cspace,
                                           seL4_CapData_t cdata, seL4_CPtr vspace, seL4_CapData_t vdata,
                                           seL4_Word ipc_buffer_addr, seL4_CPtr ipc_buffer_cap)
{
#ifdef CONFIG_KERNEL_RT
    return seL4_TCB_Configure(tcb, ep, timeout_ep, props, sc, cspace, cdata, vspace, vdata,
                              ipc_buffer_addr, ipc_buffer_cap);
#else
    return seL4_TCB_Configure(tcb, ep, props, cspace, cdata, vspace, vdata,
                              ipc_buffer_addr, ipc_buffer_cap);
#endif
}

static inline seL4_Error api_tcb_set_space(seL4_CPtr tcb, seL4_CPtr ep, UNUSED seL4_CPtr timeout_ep,
                                           seL4_CPtr cspace,
                                           seL4_CapData_t cdata, seL4_CPtr vspace, seL4_CapData_t vdata)
{
#ifdef CONFIG_KERNEL_RT
    return seL4_TCB_SetSpace(tcb, ep, timeout_ep, cspace, cdata, vspace, vdata);
#else
    return seL4_TCB_SetSpace(tcb, ep, cspace, cdata, vspace, vdata);
#endif
}
