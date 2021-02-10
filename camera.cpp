#include "camera.h"
#include "parameters.h"
#include "utils.h"

Camera::Camera(SDL_Rect rect)
    : defaultRect_(std::move(rect))
    , zoom_(0)
{
    Zoom(DEFAULT_ZOOM);
}

void Camera::ClearZoom() {
    rect_ = defaultRect_;
    rect_.x = -SCREEN_WIDTH / 2;
    rect_.y = -SCREEN_HEIGHT / 2;
    zoom_ = 1;
}

void Camera::Move(const Point& direction) {
    rect_.x += direction.x * CAMERA_STEP * zoom_;
    rect_.y += direction.y * CAMERA_STEP * zoom_;
    defaultRect_.x += direction.x * CAMERA_STEP;
    defaultRect_.y += direction.y * CAMERA_STEP;
}

std::optional<SDL_Rect> Camera::ShowRect(SDL_Rect rect) const {
    const auto topLeftPoint = GetPoint(Point{rect.x, rect.y});
    const auto topRightPoint = GetPoint(Point{rect.x + rect.w, rect.y});
    const auto botLeftPoint = GetPoint(Point{rect.x, rect.y + rect.h});
    return SDL_Rect{
        topLeftPoint.x,
        topLeftPoint.y,
        topRightPoint.x - topLeftPoint.x,
        botLeftPoint.y - topLeftPoint.y
    };
}

void Camera::Zoom(int diff) {
    zoom_ += diff;
    ASSERT(zoom_ >= 1);

    diff_.x = defaultRect_.w / 2 * (1 - 1.0 / zoom_);
    diff_.y = defaultRect_.h / 2 * (1 - 1.0 / zoom_);

    rect_.x = defaultRect_.x + diff_.x;
    rect_.y = defaultRect_.y + diff_.y;
}
