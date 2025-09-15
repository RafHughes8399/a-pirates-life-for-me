#ifndef HUD_H
#define HUD_H

#include "../lib/raylib/src/raylib.h"
#include "sprite.h"
#include "events.h"
#include <vector>
#include <memory>
// the hud has an abstract hud_strategy, which is a concrete strategy, just swap the strategy
// or you just build two instances of the hud

// you listen to direction changes, anchor changes, sail changes and positional changes

// i think a hud strategy is necessary, because they behave somewhat differently right
// in terms of the events they listen to, the elements they draw

// well the behaviour doesn't really change, just the elements 

// maybe a hud builder ?  it is more the contents that differ, less so the behaviour 
// and then upon events happening the hud would change ? or rather be rebuilt ?

// ? consider the way you will implement hud switching
// ? possibilites include a hud state or a hud strategy that change in response to events triggered by the player ? 
// ? the state / strategy define the specific elements of the hud
// ? i think state perhaps, because the behaviour is the same, all that differs is the content
namespace hud   {

} // namespace name

#endif