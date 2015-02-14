/*
 * Copyright 2014, NICTA
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(NICTA_GPL)
 */

/*vm exits related with hlt'ing*/

#include <stdio.h>
#include <stdlib.h>

#include <sel4/sel4.h>

#include "vmm/vmm.h"

/* Handling halt instruction VMExit Events. */
int vmm_hlt_handler(vmm_vcpu_t *vcpu) {
    if (!(vmm_guest_state_get_rflags(&vcpu->guest_state, vcpu->guest_vcpu) & BIT(9))) {
        if (!vcpu->halted) {
            printf("vcpu %d is halted forever :(\n", vcpu->vcpu_id);
            vcpu->halted = 1;
        }
    }

    if (vmm_apic_has_interrupt(vcpu) == -1) {
        /* Halted, don't reply until we get an interrupt */
        vcpu->guest_state.virt.interrupt_halt = 1;
    }

    vmm_guest_exit_next_instruction(&vcpu->guest_state);
    return 0;
}
