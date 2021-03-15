#include <stdint.h>
#include <pages.hh>

namespace {
        inline static void *kalloc(size_t size) {
                return k_page_alloc((size+PAGE_SIZE-1) / PAGE_SIZE);
        }

#define kfree k_page_free

#define kcalloc(nmemb, size) kalloc(nmemb * size)

        //void *realloc(void *ptr, size_t size);
        //void *reallocarray(void *ptr, size_t nmemb, size_t size);
}
