#ifndef INTERACTION_H
#define INTERACTION_H

#include "entities.h"
namespace interactions{
    class interaction{
        public:
            ~interaction() = default;
            interaction(entities::entity* interactor, entities::entity* interactee)
                :interactor_(interactor), interactee_(interactee){};
            interaction(const interaction& other) = default;
            interaction(interaction&& other) = default;

            void process();
        private:
            entities::entity* interactor_;
            entities::entity* interactee_;
    };
    class interaction_list{
        public:
            // construct
            ~interaction_list() = default;
            interaction_list();
            interaction_list(const interaction_list& other) = default;
            interaction_list(interaction_list&& other) = default;
            // add interaction
            void add_interaction(interaction& interaction);
            // emplace back
            void emplace_interaction(entities::entity* interactor, entities::entity* interactee);
            // process interactions 
            void process_interactions();
            // clear interactions
            void clear();


        private:
        std::vector<interaction> interactions_;

    };

}

#endif