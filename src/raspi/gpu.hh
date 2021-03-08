#pragma once
#include <stdint.h>

class GPU {
        private:
                bool _valid;
                uint32_t _width, _height, pitch;
                uint8_t *buffer;
        public:
                //Show the #included picture (temporary)
                void showdemopicture() noexcept;

                constexpr inline bool      valid() noexcept { return _valid; }
                constexpr inline uint32_t  width() noexcept { return _width; }
                constexpr inline uint32_t  height() noexcept { return _height; }
                inline uint32_t &pixel(uint32_t x, uint32_t y) noexcept {
                        return *(uint32_t*)&buffer[y*pitch+x*4];
                }
                inline void draw_picture(
                        uint32_t(*f)(uint32_t, uint32_t),
                        uint32_t width = 0,
                        uint32_t height = 0,
                        uint32_t offset_x = 0,
                        uint32_t offset_y = 0
                ) noexcept {
                        if(width == 0) width = _width;
                        if(height == 0) height = _height;
                        for(uint32_t y = 0; y < height; y++) {
                                for(uint32_t x = 0; x < width; x++) {
                                        pixel(x+offset_x, y+offset_y) = f(x, y);
                                }
                        }
                }

                GPU(uint32_t width = 1024, uint32_t height = 768) noexcept;
};

#define rgba(r, g, b, a) ((r) | ((g)<<8) | ((b)<<16) | ((a)<<24))
