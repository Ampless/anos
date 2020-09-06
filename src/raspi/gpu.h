#pragma once
#include "../stdint.h"

class GPU {
        private:
                bool _Valid;
        public:
                uint32_t width, height, pitch;
                uint8_t *buffer;

                uint32_t &pixel(uint32_t x, uint32_t y) noexcept;
                void drawpixel(int x, int y, int r, int g, int b) noexcept;
                //Show the #included picture (temporary)
                void showpicture() noexcept;
                bool valid() noexcept;

                GPU(uint32_t width = 1024, uint32_t height = 768) noexcept;
};
