#include "collider.h"
#include "utils.h"

namespace {

int Distance(int lhs, int rhs) {
    return std::abs(lhs - rhs);
}

Point FindFarthest(const Point& point, const SDL_Rect& rect) {
    return Point{
        Distance(rect.x + rect.w, point.x) > Distance(rect.x, point.x) ? rect.x + rect.w : rect.x,
        Distance(rect.y + rect.h, point.y) > Distance(rect.y, point.y) ? rect.y + rect.h : rect.y,
    };
}

Point FindClosest(const Point& point, const SDL_Rect& rect) {
    return Point{
        std::max(std::min(point.x, rect.x + rect.w), rect.x),
        std::max(std::min(point.y, rect.y + rect.h), rect.y),
    };
}

bool IsPointInsideCirc(const Point& point, const Circ& circ) {
    return Sqr(circ.center_.x - point.x) + Sqr(circ.center_.y - point.y) <= Sqr(circ.radius_);
}

bool IsPointOutsideCirc(const Point& point, const Circ& circ) {
    return Sqr(circ.center_.x - point.x) + Sqr(circ.center_.y - point.y) > Sqr(circ.radius_);
}

bool IsPointStrictlyInsideCirc(const Point& point, const Circ& circ) {
    return Sqr(circ.center_.x - point.x) + Sqr(circ.center_.y - point.y) < Sqr(circ.radius_);
}

bool Circ1DimAgainstRect2D(const Circ& circ, const SDL_Rect& rect) {
    const auto farthest = FindFarthest(circ.center_, rect);
    if (IsPointStrictlyInsideCirc(farthest, circ)) {
        return false;
    }
    const auto closest = FindClosest(circ.center_, rect);
    return IsPointInsideCirc(closest, circ);
}

}  // anonymous namespace

bool Collider::Collide(const Shape&, const Shape&) const {
    return false;
}

BoxCollider::BoxCollider() = default;

bool BoxCollider::Collide(const Shape& lhs, const Shape& rhs) const {
    ASSERT(dynamic_cast<const Rect*>(&lhs));
    if (auto circ = dynamic_cast<const Circ*>(&rhs)) {
        return Circ1DimAgainstRect2D(*circ, static_cast<const Rect&>(lhs).rect_);
    }
    return !IsRectAngleOutside(
        static_cast<const Rect&>(lhs).rect_,
        static_cast<const Rect&>(rhs).rect_);
}

bool BoxCollider::IsRectAngleOutside(const SDL_Rect& lhs, const SDL_Rect& rhs) {
    return lhs.x > rhs.x + rhs.w
        || lhs.x + lhs.w < rhs.x
        || lhs.y > rhs.y + rhs.h
        || lhs.y + lhs.h < rhs.y;
}

bool CircCollider::Collide(const Shape& lhs, const Shape& rhs) const {
    ASSERT(dynamic_cast<const Circ*>(&lhs));
    if (auto _ = dynamic_cast<const Rect*>(&rhs)) {
        return BoxCollider().Collide(rhs, lhs);
    }
    return Collider::Collide(lhs, rhs);
}
