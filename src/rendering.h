#include <cmath>
#include "config.h"

#include "../lib/raylib/src/raymath.h"
#include "../lib/raylib/src/raylib.h"
namespace rendering{
    struct plane {
        Vector3 normal_;
        float distance_; // from origin to nearest plane
        plane() = default;
        plane(Vector3 normal, float distance)
        :   normal_(normal), distance_(distance){}

        plane(Vector3 point, Vector3 normal)
        : normal_(normal), distance_(Vector3DotProduct(normal, point)){}

        float signed_distance_to_plane(Vector3& point) const;
    };

    class frustrum{
        public:
            void update_frustrum(Camera3D& camera, float aspect, float fov_y, float z_near, float z_far);
            
            ~frustrum() = default;

            /**
             * camera is the camera,
             * aspect is screen width / screen height
             * fov_y is the camera fov
             * z_near and z_far describe the render distance, z-near is near the camear
             */
            frustrum(Camera3D& camera, float aspect, float fov_y, float z_near, float z_far){
                // then calcualti
                update_frustrum(camera, aspect, fov_y, z_near, z_far);   
            } 
            frustrum(const frustrum& other)
            : near_(other.near_), far_(other.far_), right_(other.right_), left_(other.left_), top_(other.top_), bottom_(other.bottom_){
                
            }
            frustrum(frustrum&& other)
            : near_(std::move(other.near_)), far_(std::move(other.far_)), right_(std::move(other.right_)), left_(std::move(other.left_)), top_(std::move(other.top_)), bottom_(std::move(other.bottom_)){
            }
            
            frustrum& operator=(const frustrum& other);
            
            frustrum& operator=(frustrum&& other);
            
            bool contains(BoundingBox& object_box) const;
            
            private:
            plane top_;
            plane bottom_;

            plane right_;
            plane left_;
             
            plane near_;
            plane far_;
    };
}