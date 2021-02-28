mmu_enable:
        // sets the page table addresses
        MSR TTBR0_EL1, X0
        MSR TTBR1_EL1, X1
        MSR TCR_EL1, X2
        ISB

        MRS X0, SCTLR_EL1
        //setting the lsb in SCTLR_EL1 actually enables the MMU
        ORR X0, X0, #1
        MSR SCTLR_EL1, X0
        ISB
