#pragma once

#include "direction.h"

#include <SDL.h>

#include <optional>

constexpr int DEFAULT_ZOOM = 1;

class Camera {
public:
    explicit Camera(SDL_Rect rect);

    void ClearZoom();

    template <class P>
    P GetPoint(P point) const {
        Point center{rect_.x + rect_.w / 2, rect_.y + rect_.h / 2};
        point.x -= center.x;
        point.y -= center.y;
        point.x *= zoom_;
        point.y *= zoom_;
        return point;
    }

    void Move(const Point& direction);

    [[nodiscard]] std::optional<SDL_Rect> ShowRect(SDL_Rect rect) const;

    void Zoom(int diff);

private:
    SDL_Rect defaultRect_;
    SDL_Rect rect_;
    Point diff_;
    int zoom_;
};
