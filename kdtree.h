#include "blocking_object.h"
#include "utils.h"

#include <limits>

class KdTree {
public:
    struct Node {
        ~Node() {
            delete left;
            delete right;
        }

        Point location{0, 0};
        Node* left = nullptr;
        Node* right = nullptr;

        BlockingLayer layer;
    };

    KdTree(BlockingLayer& blockingLayer, int dim)
        : dim_(dim) {
        Build(&root_, blockingLayer, 0);
        ASSERT(dim_ == 2);
    }

    void Build(Node* node, BlockingLayer& blockingLayer, int depth, int wasBadBuild = 0) {
        const int axis = depth % dim_;
        std::sort(blockingLayer.begin(), blockingLayer.end(), [axis](const auto& lhs, const auto& rhs) {
            return lhs->GetPositionAsArray()[axis] < rhs->GetPositionAsArray()[axis];
        });

        const auto median = blockingLayer[blockingLayer.size() / 2]->GetPositionAsArray()[axis];

        SDL_Rect line;
        if (axis == 0) {
            line = SDL_Rect{median, -1000, 0, 2000};
            node->location.x = median;
            node->location.y = std::numeric_limits<int>::max();
        } else {
            line = SDL_Rect{-1000, median, 2000, 0};
            node->location.x = std::numeric_limits<int>::max();
            node->location.y = median;
        }

        BlockingLayer left, right;
        for (const auto& blockingObject : blockingLayer) {
            const int coordinate = blockingObject->GetPositionAsArray()[axis];
            if (coordinate > median) {
                right.push_back(blockingObject);
            } else if (BoxCollider().Collide(Rect(line), blockingObject->GetShape())) {
                node->layer.push_back(blockingObject);
            } else {
                left.push_back(blockingObject);
            }
        }

        const int middleSize = node->layer.size();
        if (middleSize > 4) {
            if (wasBadBuild > 0) {
                if (wasBadBuild < middleSize) {
                    node->layer.clear();
                    return Build(node, blockingLayer, depth - 1, middleSize);
                }
            } else {
                node->layer.clear();
                return Build(node, blockingLayer, depth + 1, middleSize);
            }
        }

        static const int MAX_DEPTH = 7;
        static const int MAX_ELEMENTS = 2;

        if (!left.empty()) {
            if (left.size() <= MAX_ELEMENTS || depth > MAX_DEPTH) {
                node->left = new Node();
                node->location = left[left.size() / 2]->GetShape().GetPosition();
                node->left->layer = std::move(left);
            } else {
                node->left = new Node();
                Build(node->left, left, depth + 1);
            }
        }
        if (!right.empty()) {
            if (right.size() <= MAX_ELEMENTS || depth > MAX_DEPTH) {
                node->right = new Node();
                node->location = right[right.size() / 2]->GetShape().GetPosition();
                node->right->layer = std::move(right);
            } else {
                node->right = new Node();
                Build(node->right, right, depth + 1);
            }
        }
    }

    template <class Handler>
    void ReportCollisions(const Handler& handler) const {
        BlockingLayer stack;
        size_t total = 0;
        ReportCollisions(handler, &root_, stack, total);
    }

    template <class Handler>
    void ReportCollisions(const Handler& handler, const Node* node, BlockingLayer& stack, size_t& total) const {
        if (!node) {
            return;
        }
        total += node->layer.size();

        for (size_t i = 0; i < node->layer.size(); ++i) {
            for (size_t j = i + 1; j < node->layer.size(); ++j) {
                handler(node->layer[i], node->layer[j]);
            }
            for (const auto& object : stack) {
                handler(node->layer[i], object);
            }
        }

        for (const auto& object : node->layer) {
            stack.push_back(object);
        }

        ReportCollisions(handler, node->left, stack, total);
        ReportCollisions(handler, node->right, stack, total);

        for (const auto& __attribute__((unused)) _ : node->layer) {
            stack.pop_back();
        }
    }

private:
    int dim_ = 0;
    Node root_;
};
