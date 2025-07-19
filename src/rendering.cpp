#include "rendering.h"

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
