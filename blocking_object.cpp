#include "blocking_object.h"

bool BlockingObject::Collides(const BlockingObject& rhs) const {
    return collider_->Collide(GetShape(), rhs.GetShape());
}

bool BlockingObject::Collides(const Shape& rhs) const {
    return collider_->Collide(GetShape(), rhs);
}

std::array<int, 2> BlockingObject::GetPositionAsArray() const {
    const auto point = shape_->GetPosition();
    return {point.x, point.y};
}

Shape& BlockingObject::GetShape() {
    return *shape_;
}

const Shape& BlockingObject::GetShape() const {
    return *shape_;
}

void BlockingObject::OnCollision(const std::shared_ptr<BlockingObject>&) {
}
