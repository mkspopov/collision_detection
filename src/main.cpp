#include "kdtree.h"
#include "square.h"
#include "timer.h"
#include "utils.h"

#include <iostream>

static const std::vector<SDL_Color> COLOR_MAP{
    SALAD_COLOR,
    BLUE_COLOR,
    RED_COLOR,
    PINK_COLOR,
    NAVY_COLOR,
};

static constexpr int WIDTH = 1000;
static constexpr int SQUARES = 1000;
static constexpr int DIMENSION = 2;

template <class Callback>
void SimpleCollisionHandler(const BlockingLayer& blockingLayer, Callback reporter) {
    for (size_t i = 0; i < blockingLayer.size(); ++i) {
        for (size_t j = i + 1; j < blockingLayer.size(); ++j) {
            reporter(blockingLayer[i], blockingLayer[j]);
        }
    }
}

int main() {
    BlockingLayer squares;
    squares.reserve(SQUARES);
    auto add_squares = [&]() {
        for (int i = 0; i < SQUARES; ++i) {
            squares.push_back(std::make_shared<Square>(
                GenRandomNumber(0, WIDTH),
                GenRandomNumber(0, WIDTH),
                GenRandomNumber(1, 30),
                GenRandomNumber(1, 30),
                COLOR_MAP[GenRandomNumber(0, COLOR_MAP.size())],
                nullptr));
        }
    };

    add_squares();
    auto random_move = [&]() {
        for (auto& square : squares) {
            auto d = GenRandomNumber(-1, 2);
            if (GenRandomNumber(0, 2)) {
                static_cast<Square*>(square.get())->MoveY(d);
            } else {
                static_cast<Square*>(square.get())->MoveX(d);
            }
        }
    };

    static constexpr int NUM_TESTS = 100;
    uint64_t kdTime = 0;
    uint64_t simpleHandlerTime = 0;
    for (int i = 0; i < NUM_TESTS; ++i) {
        Timer timer;
        KdTree kdTree(squares, DIMENSION);
        int collisionsNumber = 0;
        int collisionChecks = 0;
        auto reporter = [&](const auto& lhs, const auto& rhs) {
            ++collisionChecks;
            if (lhs->Collides(*rhs)) {
                ++collisionsNumber;
            }
        };
        kdTree.ReportCollisions(reporter);
        kdTime += timer.ElapsedNs();
        std::cerr << "Collisions: " << collisionsNumber << ' ' << collisionChecks << '\n';

        collisionsNumber = 0;
        collisionChecks = 0;
        timer.Tick();
        SimpleCollisionHandler(squares, reporter);
        simpleHandlerTime += timer.ElapsedNs();
        std::cerr << "Collisions: " << collisionsNumber << ' ' << collisionChecks << '\n';
    }

    std::cerr << "Total KDTree time: " << kdTime / 1000000.0 << "ms\n";
    std::cerr << "Average: " << kdTime / 1000000.0 / NUM_TESTS << "ms\n";
    std::cerr << '\n';

    std::cerr << "Total Simple time: " << simpleHandlerTime / 1000000.0 << "ms\n";
    std::cerr << "Average: " << simpleHandlerTime / 1000000.0 / NUM_TESTS << "ms\n";
    std::cerr << '\n';
}
