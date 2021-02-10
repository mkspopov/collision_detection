#include "square.h"

Square::Square(int x, int y, int width, int height, SDL_Color color, SDL_Texture* texture)
    : BlockingObject(
        std::make_unique<BoxCollider>(),
        std::make_unique<Rect>(SDL_Rect{x, y, width, height}),
        std::move(color),
        texture) {
}

void Square::Move(const Point& alongVector) {
    Square::MoveX(alongVector.x);
    Square::MoveY(alongVector.y);
}

void Square::MoveX(int dx) {
    MyRect().x += dx;
}

void Square::MoveY(int dy) {
    MyRect().y += dy;
}

bool Square::OutOfBorders(const SDL_Rect& borders) const {
    return MyRect().x < borders.x || MyRect().x > (borders.x + borders.w)
        || MyRect().y < borders.y || MyRect().y > (borders.y + borders.h);
}

Point Square::Position() const {
    return Point{MyRect().x, MyRect().y};
}

void Square::Render(SDL_Renderer* renderer, const Camera& camera) const {
    if (!exists_) {
        return;
    }
    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);
    const auto rect = camera.ShowRect(MyRect());
    if (rect) {
        SDL_RenderFillRect(renderer, &*rect);
    }
}
