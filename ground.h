#ifndef _GROUND_H
#define _GROUND_H

#include "game_object.h"

class Ground : public GameObject
{
public:
	Ground();
	~Ground();

	void InitGround(
		PrimitiveBuilder* primitive_builder,
		b2World* world, 
		b2Vec2 position, 
		gef::Vector4 ground_half_dimensions,
		uint16 category_bits, 
		uint16 mask_bits, 
		uint16 group_index,
		OBJECT_TYPE type,
		OBJECT_COLOUR colour);
};

#endif // !_GROUND_H