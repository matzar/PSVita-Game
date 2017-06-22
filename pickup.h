#ifndef PICKUP_H
#define PICKUP_H

#include "game_object.h"
#include <set>

class Pickup : public GameObject
{
public:
	Pickup();
	~Pickup();

	void InitPickup(
		PrimitiveBuilder* primitive_builder, 
		b2World* world, 
		b2Vec2 position, 
		float32 radius, 
		uint16 category_bits, 
		uint16 mask_bits, 
		uint16 group_index,
		OBJECT_TYPE type);

private:
};

static std::set<GameObject*> pickups_scheduled_for_removal_;

#endif // !PICKUP_H

