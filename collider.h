#pragma once

#include "shape.h"

class Collider {
public:
    virtual ~Collider() = default;

    [[nodiscard]] virtual bool Collide(const Shape& lhs, const Shape& rhs) const;
};

class BoxCollider : public Collider {
public:
    BoxCollider();

    [[nodiscard]] bool Collide(const Shape& lhs, const Shape& rhs) const override;

private:
    static bool IsRectAngleOutside(const SDL_Rect& lhs, const SDL_Rect& rhs);
};

class CircCollider : public Collider {
public:
    CircCollider() = default;

    [[nodiscard]] bool Collide(const Shape& lhs, const Shape& rhs) const override;
};
