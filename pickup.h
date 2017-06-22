#ifndef PICKUP_H
#define PICKUP_H

#include "game_object.h"

class Platform;

class Pickup : public GameObject
{
public:
	Pickup(Platform& platform);
	~Pickup();

	void InitPickup(
		PrimitiveBuilder* primitive_builder, 
		Platform& platform,
		b2World* world, 
		b2Vec2 position, 
		float32 radius, 
		uint16 category_bits, 
		uint16 mask_bits, 
		uint16 group_index,
		OBJECT_TYPE type);
private:
	Platform& platform_;
	PrimitiveBuilder* primitive_builder_;
};

#endif // !PICKUP_H

