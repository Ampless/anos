#include <stdint.h>

#ifndef ssize_t
typedef int64_t ssize_t;
#endif

struct afile {
        uint64_t size;
        uint64_t first_sector;
        bool is_dir;
        uint8_t name[492];
};

static_assert(sizeof(afile) == 512, "");

template <typename fd>
class afs {
private:
        void(*aseek)(fd, ssize_t);
        void(*aread)(fd, uint32_t, uint8_t *);
        void(*awrite)(fd, uint32_t, uint8_t *, bool);

public:
        afs(void(*_aseek)(fd, ssize_t), void(*_aread)(fd, uint32_t, uint8_t *),
            void(*_awrite)(fd, uint32_t, uint8_t *, bool))
                : aseek{_aseek}, aread{_aread}, awrite{_awrite} {}
};
