#pragma once

#include "camera.h"

#include <SDL.h>

#include <atomic>
#include <memory>

class Object : public std::enable_shared_from_this<Object> {
public:
    Object(SDL_Color color, SDL_Texture* texture);

    virtual ~Object();

    [[nodiscard]] virtual bool Exists() const;

    [[nodiscard]] std::shared_ptr<Object> GetObject();

    virtual void Kill();

    virtual void Move();

    [[nodiscard]] virtual bool OutOfBorders(const SDL_Rect&) const;

    virtual void Render(SDL_Renderer*, const Camera&) const;

    void SetColor(SDL_Color color);

protected:
    SDL_Color color_;
    SDL_Texture* texture_ = nullptr;

    std::atomic<bool> exists_{true};
};
