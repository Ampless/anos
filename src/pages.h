#include "stdint.h"
#define PAGE_SIZE 0x1000

void *k_page_alloc(size_t count);
void k_page_free(void *ptr);
