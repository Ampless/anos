#include "pages.h"

#define MEMORY_SIZE  (256 * 1024 * 1024)
#define PAGE_SIZE    (4096)
#define PAGE_COUNT   (MEMORY_SIZE / PAGE_SIZE)
#define FIRST_PAGE   ((16 * 1024 * 1024) / PAGE_SIZE)
#define PAGE2ADDR(x) ((void *)((x) * PAGE_SIZE))
#define ADDR2PAGE(x) ((x) / PAGE_SIZE)

struct { bool allocated = false, continued = false; } pages[PAGE_COUNT];

bool k_pages_viable(size_t first, size_t count) {
        if(first + count >= PAGE_COUNT) return false;
        for(size_t i = first; i < first + count; i++)
                if(pages[i].allocated)
                        return false;
        return true;
}

void *k_pages_alloc_unchecked(size_t first, size_t count) {
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
        //TODO: actually free the pages
}
