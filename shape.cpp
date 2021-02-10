#include "constants.h"
#include "shape.h"

Shape::~Shape() = default;

Point Shape::GetPosition() const {
    return Point();
}

Rect::Rect(SDL_Rect rect) : rect_(std::move(rect)) {
}

Point Rect::GetPosition() const {
    return Point{rect_.x, rect_.y};
}

Circ::Circ(Point center, int radius) : center_(std::move(center)), radius_(radius) {
}

Point Circ::GetPosition() const {
    return Point{
        center_.x - radius_,
        center_.y - radius_,
    };
}
