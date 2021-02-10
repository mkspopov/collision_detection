#pragma once

#include "constants.h"

class Direction {
public:
    static constexpr Point DOWN() {
        return {0, 1};
    }
    static constexpr Point LEFT() {
        return {-1, 0};
    }
    static constexpr Point RIGHT() {
        return {1, 0};
    }
    static constexpr Point UP() {
        return {0, -1};
    }
};

Point GetRandomDirection();
