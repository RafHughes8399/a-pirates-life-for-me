#ifndef CONTROLS_H
#define CONTROLS_H

#include <vector>


#include "../lib/raylib/src/raylib.h"
// make singleton ?
namespace controls{
    class ship_controls{
        public:
        ship_controls(const ship_controls& other) = delete;
        ship_controls(ship_controls&& other) = delete;
        
        ship_controls& operator=(const ship_controls& other) = delete;
        ship_controls& operator=(ship_controls&& other) = delete;

        static ship_controls& get_instance(){
            static ship_controls instance;
            return instance;
        }
        //TODO implement 
        void update_control(int key);
        std::vector<int>& get_controls(){
            return controls_;
        }
        private:
        ~ship_controls() = default;
        ship_controls(){
            // init values
            // steering 
            controls_.push_back(KEY_A);
            controls_.push_back(KEY_D);
            // sail movement
            controls_.push_back(KEY_W);
            controls_.push_back(KEY_S);
            
            // anchor movement
            controls_.push_back(KEY_R);
            controls_.push_back(KEY_T);
        }
        std::vector<int> controls_;

    };
    // pirate_controls{
    
};
#endif