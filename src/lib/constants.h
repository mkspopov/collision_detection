#pragma once

#include <SDL.h>

#include <chrono>

using namespace std::chrono_literals;
using Point = SDL_Point;

constexpr SDL_Color BLUE_COLOR {
    0,
    0,
    0xFF,
    0xFF,
};

constexpr SDL_Color TRANSPARENT_COLOR {
    0,
    0,
    0,
    0,
};

constexpr SDL_Color RED_COLOR {
    0xFF,
    0,
    0,
    0xFF,
};

constexpr SDL_Color PEACH_COLOR {
    0xFF,
    0xCB,
    0xA4,
    0xFF,  // alpha --- transparency --- combination with a pixel from the buffer
};

constexpr SDL_Color SOFT_PEACH_COLOR {
    0xF1,
    0xD8,
    0xD8,
    0xFF,
};

constexpr SDL_Color PINK_COLOR {
    0xF9,
    0x8B,
    0x88,
    0xFF,
};

constexpr SDL_Color SALAD_COLOR {
    0xAA,
    0xEE,
    0xAF,
    0xFF,
};

constexpr SDL_Color WHITE_COLOR {
    0xFF,
    0xFF,
    0xFF,
    0xFF,
};

constexpr SDL_Color EMERALD_SHADE_COLOR {
    0x19,
    0x4F,
    0x2B,
    0xFF,
};

constexpr SDL_Color NAVY_COLOR {
    0x00,
    0x00,
    0x80,
    0xFF,
};
