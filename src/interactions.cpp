#include "interactions.h"

void interactions::interaction::process(){
    //TODO implement
    // something along the lines of interactor.interact(interactee);
    // that is the next stage, for now just check
    return;
}

void interactions::interaction_list::add_interaction(interactions::interaction& interaction){
    interactions_.push_back(interaction);
    return;
}
void interactions::interaction_list::emplace_interaction(entities::entity* interactor, entities::entity* interactee){
    interactions_.emplace_back(interactor, interactee);
}

void interactions::interaction_list::process_interactions(){
    std::for_each(interactions_.begin(), interactions_.end(), 
    [](auto& i) -> void {
        i.process();
    });
}

void interactions::interaction_list::clear(){
    interactions_.clear();
}