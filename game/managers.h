#pragma once
#include "../environment/world.h"
/**  manages collisions in the game */

/**
 * two types of collision to manage, object <-> object and object <-> terrain
 * terrain is land or water essentially
 *
 * so an object has aterrain based on its position'
 *
 *  how can I implement this grid
 *
 * i think the grid method is good for directional forces and collisions
 * because then you can do object <-> and as they apply force to each other
 *
 * but if you simulate say 250 * 100 * 250 that is 5000 cell checks per tick that is a lot
 *
 * so maybe the sort and sweep is better
 * do a sort sweep object to object
 * and do a sort sweep object to terrain
 *
 * I think the big boy academic book is better
 *
 *
 * i think terrain is an object, yes
 * then terrain can be land, water, and maybe air ?
 * so ocean is an object
 * and an island is an object
 * and the underlying height map  terrain is an object,
 * yes i think so
 *
 *
 * this way they can be compared in the sweep and sort
 * that way the collision manager can take the reference to the list of pointers held by the world
 *
 * physics properties
 * volume,
 * velocity
 * acceleration
 * mass
 *
 * water has density
 *
 *
 
 /**
  * observer pattern between the world and the managers, the world notifies the managers when things
  * change so they can update certain characteristics.
  * 
  * 
  * there is a parent manager class 
  * then sub concrete classes that is tempalted over the type of information
  * that is managed, for example the texture manages a list of textures
  */
/**  manages textures in the game */
class Collision_Manager {
public:
private:

};
class TextureManager {

};
/**  manages models within the game  */
class ModelManager {


};
/**  manages sounds within the game */
class SoundManager {

};
