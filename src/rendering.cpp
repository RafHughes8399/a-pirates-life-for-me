#include "rendering.h"


float rendering::plane::signed_distance_to_plane(Vector3& point){
    return Vector3DotProduct(normal_, point) - distance_; 
}
rendering::frustrum& rendering::frustrum::operator=(const frustrum& other){
    near_ = other.near_;
    far_ = other.far_;

    top_ = other.top_;
    bottom_ = other.bottom_;

    right_ = other.right_;
    left_ = other.left_;
    return *this;
}
rendering::frustrum& rendering::frustrum::operator=(frustrum&& other){
    near_ = std::move(other.near_);
    far_ = std::move(other.far_);
    
    top_ = std::move(other.top_);
    bottom_ = std::move(other.bottom_);
    
    right_ = std::move(other.right_);
    left_ = std::move(other.left_);
    return *this;
    return *this;
}

bool on_in_front_plane(BoundingBox& box, rendering::plane& plane){
    auto centre = Vector3Scale(Vector3Add(box.min, box.max), 0.5);
    auto extents = Vector3Subtract(box.max, centre);

    const float r = extents.x * std::abs(plane.normal_.x) +
            extents.y * std::abs(plane.normal_.y) + extents.z * std::abs(plane.normal_.z);

    return -r <= plane.signed_distance_to_plane(centre);
}

bool rendering::frustrum::contains(BoundingBox& box){
    return on_in_front_plane(box, near_)
    and on_in_front_plane(box, far_)
    and on_in_front_plane(box, top_)
    and on_in_front_plane(box, bottom_)
    and on_in_front_plane(box, left_)
    and on_in_front_plane(box, right_);
}
