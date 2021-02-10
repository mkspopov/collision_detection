#include "object.h"

Object::Object(SDL_Color color, SDL_Texture* texture)
    : color_(std::move(color))
    , texture_(texture) {
}

Object::~Object() {
    SDL_DestroyTexture(texture_);
    texture_ = nullptr;
}

[[nodiscard]] bool Object::Exists() const {
    return exists_.load(std::memory_order_relaxed);
}

std::shared_ptr<Object> Object::GetObject() {
    return shared_from_this();
}

void Object::Kill() {
    exists_.store(false, std::memory_order_relaxed);
}

void Object::Move() {
}

[[nodiscard]] bool Object::OutOfBorders(const SDL_Rect&) const {
    return false;
}

void Object::Render(SDL_Renderer*, const Camera&) const {
}

void Object::SetColor(SDL_Color color) {
    color_ = std::move(color);
}
