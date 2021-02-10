#pragma once

#include "parameters.h"

#include <SDL.h>

#include <memory>
#include <random>
#include <stdexcept>

template <class T, class U>
std::shared_ptr<T> As(const std::shared_ptr<U>& ptr) {
    return std::dynamic_pointer_cast<T>(ptr);
}

template <class T>
void CheckSDLPointer(T* sdlPointer) {
    if (!sdlPointer) {
        throw std::runtime_error(SDL_GetError());
    }
}

void ClearScreen(SDL_Renderer* renderer, SDL_Color color);

std::mt19937& GetRandomGen();

int GenRandomNumber(int from, int to);

template <class T, class U>
bool Is(const std::shared_ptr<U>& ptr) {
    return static_cast<bool>(std::dynamic_pointer_cast<T>(ptr));
}

bool PointInRect(const Point& point, const SDL_Rect& rect);

constexpr int64_t Sqr(int64_t x) {
    return x * x;
}

constexpr double SquaredDistance(const Point& lhs, const Point& rhs) {
    return (lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y);
}

#define ASSERT(expression) do {                     \
    if (!(expression)) {                            \
        throw std::runtime_error("ASSERT failed."); \
    }                                               \
} while (false)

#define ASSERT_THROWS(expression) do {                     \
    bool throws = false;                                   \
    try {                                                  \
        (expression);                                      \
    } catch (...) {                                        \
         throws = true;                                    \
    }                                                      \
    if (!throws) {                                         \
        throw std::runtime_error("ASSERT_THROWS failed."); \
    }                                                      \
} while (false)

template <class Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end) : begin_(std::move(begin)), end_(std::move(end)) {
    }

    Iterator begin() const {
        return begin_;
    }

    Iterator end() const {
        return end_;
    }

private:
    Iterator begin_;
    Iterator end_;
};
