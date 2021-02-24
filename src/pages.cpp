#include "pages.h"

#define MEMORY_SIZE  (256 * 1024 * 1024)
// TODO: get this from arch specific check
#define PAGE_COUNT   (MEMORY_SIZE / PAGE_SIZE)
#define FIRST_PAGE   ((0x10 * 0x100000) / PAGE_SIZE)
#define PAGE2ADDR(x) ((void *)((x) * PAGE_SIZE))
#define ADDR2PAGE(x) (((uintptr_t)(x)) / PAGE_SIZE)

struct { bool allocated = false, continued = false; } pages[PAGE_COUNT];

inline bool k_pages_viable(size_t first, size_t count) {
        if(first + count >= PAGE_COUNT) return false;
        for(size_t i = first; i < first + count; i++)
                if(pages[i].allocated)
                        return false;
        return true;
}

// TODO: initialize to zero, because obviously that is the right thing to do
inline void *k_pages_alloc_unchecked(size_t first, size_t count) {
        for(size_t i = first; i < first + count - 1; i++) {
                pages[i].allocated = true;
                pages[i].continued = true;
        }
        pages[first + count - 1].allocated = true;
        pages[first + count - 1].continued = false;
        return PAGE2ADDR(first);
}

void *k_page_alloc(size_t count) {
        for(size_t i = FIRST_PAGE; i < PAGE_COUNT; i++)
                if(k_pages_viable(i, count))
                        return k_pages_alloc_unchecked(i, count);
        return NULL;
}

void k_page_free(void *ptr) {
        if((uintptr_t)ptr % PAGE_SIZE) return;
        size_t i = ADDR2PAGE(ptr);
        if(!pages[i--].allocated) return;
        while(pages[++i].continued) {
                pages[i].allocated = pages[i].continued = false;
        }
        pages[i].allocated = false;
}
