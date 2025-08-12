#include "rendering.h"


void print_plane(rendering::plane p){
    std::cout << p.normal_.x << ", " << p.normal_.y << ", " << 
    p.normal_.z << std::endl;
}

float rendering::plane::signed_distance_to_plane(Vector3& point) const{
    return Vector3DotProduct(normal_, point) - distance_; 
}

void rendering::frustrum::update_frustrum(Camera3D& camera, float aspect, float fov_y, float z_near, float z_far){
    
    std::cout << "======================CALCULATE FRUSTRUM=================================" << std::endl;
     
    float half_v_side = z_far * tanf(fov_y * 0.5f);
    float half_h_side = half_v_side * aspect;
    auto camera_front = GetCameraForward(&camera);
    auto camera_right = GetCameraRight(&camera);
    auto camera_up = GetCameraUp(&camera);
    auto front_mult_far = Vector3Scale(camera_front, z_far);
    


    Vector3 near_point = Vector3Add(camera.position, Vector3Scale(camera_front, z_near));
    Vector3 near_normal = camera_front;
    near_ = {near_point, near_normal};

    Vector3 far_point = Vector3Add(camera.position, front_mult_far);
    Vector3 far_normal = Vector3Scale(camera_front, -1);
    far_ = {far_point, far_normal};

    Vector3 top_cross = Vector3Subtract(front_mult_far, Vector3Scale(camera_up, half_v_side));
    Vector3 top_normal = Vector3CrossProduct(camera_right, top_cross);
    top_ = {camera.position, top_normal};

    Vector3 bottom_cross = Vector3Add(front_mult_far, Vector3Scale(camera_up, half_v_side));
    Vector3 bottom_normal = Vector3CrossProduct(bottom_cross, camera_right);
    bottom_ = {camera.position, bottom_normal};

    Vector3 left_cross = Vector3Add(front_mult_far, Vector3Scale(camera_right, half_h_side));
    Vector3 left_normal = Vector3CrossProduct(camera_up, left_cross);
    left_ = {camera.position, left_normal};

    Vector3 right_cross = Vector3Subtract(front_mult_far, Vector3Scale(camera_right, half_h_side));
    Vector3 right_normal = Vector3CrossProduct(right_cross, camera_up);
    right_ =  {camera.position, right_normal};

    print_frustrum();
    std::cout << "======================END CALCULATE FRUSTRUM=================================" << std::endl;
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

void rendering::frustrum::print_frustrum() const{
    std::cout << "NEAR: ";
    print_plane(near_);
    std::cout << "FAR: ";
    print_plane(far_);
    std::cout << "TOP: ";
    print_plane(top_);
    std::cout << "BOTTOM: ";
    print_plane(bottom_);
    std::cout << "LEFT: ";
    print_plane(left_);
    std::cout << "RIGHT: ";
    print_plane(right_);
}

bool rendering::frustrum::contains(BoundingBox& box) const{
    return on_in_front_plane(box, near_)
    and on_in_front_plane(box, far_)
    and on_in_front_plane(box, top_)
    and on_in_front_plane(box, bottom_)
    and on_in_front_plane(box, left_)
    and on_in_front_plane(box, right_);
}
