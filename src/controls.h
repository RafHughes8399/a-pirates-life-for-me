#ifndef CONTROLS_H
#define CONTROLS_H

#include <vector>

// make singleton ?
namespace controls{
    class ship_controls{
        public:
            ~ship_controls() = default;
            ship_controls(){
                // init values

            }
            ship_controls(const ship_controls& other) = default;
            ship_controls(ship_controls&& other) = default;

            //TODO implement 
            void update_control(int key);
        private:
            std::vector<int> controls_;

    };
    // pirate_controls{
    
};
#endif