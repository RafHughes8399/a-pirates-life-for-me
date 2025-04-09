#include "event.h"



enum class CollisionEvents {

	OceanShipCollision,
	OceanPlayerCollision
};

class OceanShipCollisionEvent : public Event<CollisionEvents> {
public:
	~OceanShipCollisionEvent() = default;
	OceanShipCollisionEvent()
		: Event<CollisionEvents>(CollisionEvents::OceanShipCollision, std::string("OceanShipInteraction")) {
	}

};