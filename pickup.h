#ifndef PICKUP_H
#define PICKUP_H

#include "game_object.h"

class Pickup : public GameObject
{
public:
	Pickup();
	~Pickup();

	void InitPickUp(PrimitiveBuilder* primitive_builder, b2World* world, b2Vec2 position, uint16 category_bits, uint16 mask_bits, uint16 group_index);
private:
};

#endif // !PICKUP_H

