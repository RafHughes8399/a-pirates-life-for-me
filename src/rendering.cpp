#include "rendering.h"


float rendering::plane::signed_distance_to_plane(Vector3& point) const{
    return Vector3DotProduct(normal_, point) - distance_; 
}

void rendering::frustrum::update_frustrum(Camera3D& camera, float aspect, float fov_y, float z_near, float z_far){
    float half_v_side = z_far * tanf(fov_y * 0.5f);
    float half_h_side = half_v_side * aspect;
    auto camera_front = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    auto camera_right = Vector3CrossProduct(camera_front, camera.up);
    auto front_mult_far = Vector3Scale(camera_front, z_far);
                
            // planes are constructed with a point on the plane and a normal vector
    near_  = plane{Vector3Add(camera.position, Vector3Scale(camera_front, z_near)), camera_front};
    far_  = plane{Vector3Add(camera.position, front_mult_far), camera_front};
     
     right_  = plane{camera.position, Vector3CrossProduct(Vector3Subtract(front_mult_far, Vector3Scale(camera_right, half_h_side)), camera.up)};
     left_  = plane{camera.position, Vector3CrossProduct(camera.up, Vector3Add(front_mult_far, Vector3Scale(camera_right, half_h_side)))};
     top_  = plane{camera.position, Vector3CrossProduct(camera_right, Vector3Subtract(front_mult_far, Vector3Scale(camera.up, half_v_side)))};
     bottom_  = plane{camera.position, Vector3CrossProduct(Vector3Add(front_mult_far, Vector3Scale(camera.up, half_v_side)), camera_right)};
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

bool on_in_front_plane(BoundingBox& box, const rendering::plane& plane){
    auto centre = Vector3Scale(Vector3Add(box.min, box.max), 0.5);
    auto extents = Vector3Subtract(box.max, centre);

    const float r = extents.x * std::abs(plane.normal_.x) +
            extents.y * std::abs(plane.normal_.y) + extents.z * std::abs(plane.normal_.z);

    return -r <= plane.signed_distance_to_plane(centre);
}

bool rendering::frustrum::contains(BoundingBox& box) const{
    return on_in_front_plane(box, near_)
    and on_in_front_plane(box, far_)
    and on_in_front_plane(box, top_)
    and on_in_front_plane(box, bottom_)
    and on_in_front_plane(box, left_)
    and on_in_front_plane(box, right_);
}
