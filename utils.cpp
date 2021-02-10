#include "utils.h"

void ClearScreen(SDL_Renderer* renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

std::mt19937& GetRandomGen() {
    static std::mt19937 gen(SEED);
    return gen;
}

int GenRandomNumber(int from, int to) {
    std::uniform_int_distribution<> dis(from, to - 1);
    return dis(GetRandomGen());
}

bool PointInRect(const Point& point, const SDL_Rect& rect) {
    return SDL_PointInRect(&point, &rect);
}
