#pragma once

#include "constants.h"

struct Shape {
    virtual ~Shape();

    virtual Point GetPosition() const;
};

struct Rect : public Shape {
    explicit Rect(SDL_Rect rect);

    virtual Point GetPosition() const;

    SDL_Rect rect_;
};

struct Circ : public Shape {
    Circ(Point center, int radius);

    virtual Point GetPosition() const;

    Point center_;
    int radius_;
};
