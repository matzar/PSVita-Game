#ifndef _GROUND_H
#define _GROUND_H

#include "game_object.h"

// forward class declarations

class Ground : public GameObject
{
public:
	Ground();
	~Ground();

	void InitGround(PrimitiveBuilder* primitive_builder, b2World* world);

private:
	// ground variables
	// ground mesh
	gef::Mesh* ground_mesh_;
	// box2d ground variables
	b2Body* ground_body_;
};

#endif // !_GROUND_H