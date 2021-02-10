#pragma once

#include "collider.h"
#include "object.h"

#include <array>
#include <memory>
#include <vector>

class BlockingObject : public Object {
public:
    template <class ...Args>
    BlockingObject(std::unique_ptr<Collider> collider, std::unique_ptr<Shape> shape, Args&& ...args)
        : Object(std::forward<Args>(args)...)
        , collider_(std::move(collider))
        , shape_(std::move(shape)) {
    }

    [[nodiscard]] virtual bool Collides(const BlockingObject& rhs) const;
    [[nodiscard]] virtual bool Collides(const Shape& rhs) const;

    virtual std::array<int, 2> GetPositionAsArray() const;

    virtual Shape& GetShape();

    virtual const Shape& GetShape() const;

    virtual void OnCollision(const std::shared_ptr<BlockingObject>& rhs);

protected:
    std::unique_ptr<Collider> collider_;
    std::unique_ptr<Shape> shape_;
};

using BlockingLayer = std::vector<std::shared_ptr<BlockingObject>>;
