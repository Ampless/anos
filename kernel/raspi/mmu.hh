#include <stdint.h>

// TODO: read ID_AA64MMFR0_EL1
#define GRANULE_SIZE 64 * 1024

// TODO: make it easy to create them
typedef uint64_t mmu_table_descriptor;
typedef uint64_t mmu_block_entry;

extern "C" void mmu_enable(void *TTBR0_EL1, void *TTBR1_EL1, void *TCR_EL1);
