#include "kdtree.h"
#include "square.h"
#include "timer.h"
#include "utils.h"

#include <iostream>
#include <string>
#include <vector>

static const std::string SCREENSHOTS_FOLDER = "/tmp/screenshots/";

static const std::vector<SDL_Color> COLOR_MAP{
    SALAD_COLOR,
    BLUE_COLOR,
    RED_COLOR,
    PINK_COLOR,
    NAVY_COLOR,
};

#ifdef _PERF
#define _SQUARES 1000;
#else
#define _SQUARES 40;
#endif

static constexpr int DIMENSION = 2;
static constexpr int NUM_TESTS = 100;
static constexpr int SQUARES = _SQUARES;
static constexpr int WIDTH = 500;

template <class Callback>
void SimpleCollisionHandler(const BlockingLayer& blockingLayer, Callback reporter) {
    for (size_t i = 0; i < blockingLayer.size(); ++i) {
        for (size_t j = i + 1; j < blockingLayer.size(); ++j) {
            reporter(blockingLayer[i], blockingLayer[j]);
        }
    }
}

class Manager {
public:
    template <class O>
    void AddObjectToRender(std::shared_ptr<O> sharedPtr);

    void Close();

    void Render();

private:
    friend Manager& GetManager();

    Manager();

    Camera mainCamera_;
    std::vector<std::shared_ptr<Object>> objectsToRender_;
    SDL_Renderer* renderer_ = nullptr;
    SDL_Window* window_ = nullptr;
};

Manager& GetManager() {
    static Manager manager;
    return manager;
}

template <class O>
void Manager::AddObjectToRender(std::shared_ptr<O> sharedPtr) {
    objectsToRender_.push_back(std::move(sharedPtr));
}

void Manager::Close() {
    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;
    SDL_DestroyWindow(window_);
    window_ = nullptr;
    SDL_Quit();
}

void Manager::Render() {
    ClearScreen(renderer_, PEACH_COLOR);
    for (const auto& object : objectsToRender_) {
        object->Render(renderer_, mainCamera_);
    }

    SDL_RenderPresent(renderer_);

    static size_t frame = 0;
    SDL_Surface* sshot = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00ff0000,
                                              0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(renderer_, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
    const std::string filename = SCREENSHOTS_FOLDER + "screenshot" + std::to_string(frame) + ".bmp";
    ++frame;
    SDL_SaveBMP(sshot, filename.c_str());
    SDL_FreeSurface(sshot);
}

Manager::Manager() : mainCamera_(SDL_Rect{0, 0, WIDTH, WIDTH}) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error(SDL_GetError());
    }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        throw std::runtime_error("Warning: Linear texture filtering not enabled!");
    }

    window_ = SDL_CreateWindow(
        "SDL Tutorial",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    CheckSDLPointer(window_);

    renderer_ = SDL_CreateRenderer(
        window_,
        -1,
        SDL_RENDERER_ACCELERATED);
    CheckSDLPointer(renderer_);

    SDL_SetRenderDrawColor(renderer_, PEACH_COLOR.r, PEACH_COLOR.g, PEACH_COLOR.b, 0xFF);
}

int main() {
    auto& manager = GetManager();

    BlockingLayer squares;
    squares.reserve(SQUARES);
    auto addSquares = [&]() {
        for (int i = 0; i < SQUARES; ++i) {
            squares.push_back(std::make_shared<Square>(
                GenRandomNumber(0, WIDTH),
                GenRandomNumber(0, WIDTH),
                GenRandomNumber(20, 30),
                GenRandomNumber(20, 30),
                WHITE_COLOR,
                nullptr));
            manager.AddObjectToRender(squares.back());
        }
    };

    addSquares();

    int collisionsNumber = 0;
    int collisionChecks = 0;

    auto visualReporter = [&](auto& lhs, auto& rhs) {
        lhs->SetColor(BLUE_COLOR);
        rhs->SetColor(BLUE_COLOR);
        manager.Render();
        ++collisionChecks;
        if (lhs->Collides(*rhs)) {
            ++collisionsNumber;
            lhs->SetColor(RED_COLOR);
            rhs->SetColor(RED_COLOR);
            manager.Render();
        }
        lhs->SetColor(WHITE_COLOR);
        rhs->SetColor(WHITE_COLOR);
        manager.Render();
    };

    auto moveSquares = [&]() {
        for (auto& square : squares) {
            static_cast<Square*>(square.get())->Move(GetRandomDirection());
        }
    };

    auto perfReporter = [&](auto& lhs, auto& rhs) {
        ++collisionChecks;
        if (lhs->Collides(*rhs)) {
            ++collisionsNumber;
        }
    };

    uint64_t kdTime = 0;
    uint64_t simpleHandlerTime = 0;
    for (int i = 0; i < NUM_TESTS; ++i) {
        Timer timer;
        KdTree kdTree(squares, DIMENSION);
        collisionsNumber = 0;
        collisionChecks = 0;

#ifdef _PERF
#define _REPORTER perfReporter
#else
#define _REPORTER visualReporter
#endif

        kdTree.ReportCollisions(_REPORTER);
        kdTime += timer.ElapsedNs();
        std::cerr << "Collisions: " << collisionsNumber << ' ' << collisionChecks << '\n';

        collisionsNumber = 0;
        collisionChecks = 0;
        timer.Tick();
        SimpleCollisionHandler(squares, _REPORTER);
        simpleHandlerTime += timer.ElapsedNs();
        std::cerr << "Collisions: " << collisionsNumber << ' ' << collisionChecks << '\n';

        moveSquares();
    }

    std::cerr << "Total KDTree time: " << kdTime / 1000000.0 << "ms\n";
    std::cerr << "Average: " << kdTime / 1000000.0 / NUM_TESTS << "ms\n";
    std::cerr << '\n';

    std::cerr << "Total Simple time: " << simpleHandlerTime / 1000000.0 << "ms\n";
    std::cerr << "Average: " << simpleHandlerTime / 1000000.0 / NUM_TESTS << "ms\n";
    std::cerr << '\n';

    manager.Close();
}
