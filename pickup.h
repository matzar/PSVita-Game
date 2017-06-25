#ifndef PICKUP_H
#define PICKUP_H

#include "game_object.h"

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
		gef::Mesh* mesh,
		uint16 category_bits, 
		uint16 mask_bits, 
		uint16 group_index,
		OBJECT_TYPE type);
};

#endif // !PICKUP_H

