#include "direction.h"
#include "utils.h"

Point GetRandomDirection() {
    const auto dir = GenRandomNumber(0, 4);
    switch (dir) {
        case 0: return Direction::DOWN();
        case 1: return Direction::LEFT();
        case 2: return Direction::RIGHT();
        case 3: return Direction::UP();
    }
    __builtin_unreachable();
}
