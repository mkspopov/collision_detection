#pragma once

#include "blocking_object.h"

#include <SDL.h>

class Square : public BlockingObject {
    using Object::Move;
public:
    Square(int x, int y, int width, int height, SDL_Color color, SDL_Texture* texture);

    void Move(const Point& alongVector);

    virtual void MoveX(int dx);

    virtual void MoveY(int dy);

    Point Position() const;

    void Render(SDL_Renderer* renderer, const Camera& camera) const override;

    bool OutOfBorders(const SDL_Rect& borders) const override;

protected:
    inline SDL_Rect& MyRect() {
        return static_cast<Rect*>(shape_.get())->rect_;
    }

    inline const SDL_Rect& MyRect() const {
        return static_cast<Rect*>(shape_.get())->rect_;
    }
};
