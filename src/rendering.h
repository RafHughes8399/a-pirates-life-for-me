#include <cmath>
#include "config.h"

#include "../lib/raylib/src/raymath.h"

namespace rendering{
    struct plane {
        Vector3 normal_;
        float distance_; // from origin to nearest plane
        plane() = default;
        plane(Vector3 normal, float distance)
        :   normal_(normal), distance_(distance){}

        plane(Vector3 point, Vector3 normal)
        : normal_(normal), distance_(Vector3DotProduct(normal, point)){}
    };

    class frustrum{
        public:
            ~frustrum() = default;

            /**
             * camera is the camera,
             * aspect is screen width / screen height
             * fov_y is the camera fov
             * z_near and z_far describe the render distance, z-near is near the camear
             */
            frustrum(Camera3D& camera, float aspect, float fov_y, float z_near, float z_far){
                // then calcualti
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

            frustrum(const frustrum& other)
            : near_(other.near_), far_(other.far_), right_(other.right_), left_(other.left_), top_(other.top_), bottom_(other.bottom_){
                
            }
            frustrum(frustrum&& other)
                : near_(std::move(other.near_)), far_(std::move(other.far_)), right_(std::move(other.right_)), left_(std::move(other.left_)), top_(std::move(other.top_)), bottom_(std::move(other.bottom_)){
            }

            frustrum& operator=(const frustrum& other);

            frustrum& operator=(frustrum&& other);

        private:
            plane top_;
            plane bottom_;

            plane right_;
            plane left_;
             
            plane near_;
            plane far_;
    };
}