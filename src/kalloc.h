#include "stdint.h"
#include "pages.h"

namespace {
        inline static void *kalloc(size_t size) {
                return k_page_alloc((size+PAGE_SIZE-1) / PAGE_SIZE);
        }

        inline static void kfree(void *ptr) {
                k_page_free(ptr);
        }

        inline static void *kcalloc(size_t nmemb, size_t size) {
                return kalloc(nmemb * size);
        }

        //void *realloc(void *ptr, size_t size);
        //void *reallocarray(void *ptr, size_t nmemb, size_t size);
}
