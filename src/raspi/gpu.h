#pragma once
#include "../stdint.h"

class GPU {
        private:
                bool _valid;
                uint32_t _width, _height, pitch;
                uint8_t *buffer;
        public:
                //Show the #included picture (temporary)
                void showdemopicture() noexcept;

                bool      valid() noexcept { return _valid; }
                uint32_t  width() noexcept { return _width; }
                uint32_t  height() noexcept { return _height; }
                uint32_t &pixel(uint32_t x, uint32_t y) noexcept {
                        return *(uint32_t*)&buffer[y*pitch+x*4];
                }

                GPU(uint32_t width = 1024, uint32_t height = 768) noexcept;
};

#define rgba(r, g, b, a) ((r) | ((g)<<8) | ((b)<<16) | ((a)<<24))
