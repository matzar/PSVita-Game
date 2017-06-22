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
		uint16 category_bits, 
		uint16 mask_bits, 
		uint16 group_index,
		OBJECT_TYPE type,
		OBJECT_COLOUR colour);
	//void CollisionResponse();

	//b2Body* GetGroundBody() { return ground_body_; }
	//gef::Mesh* GetGroundMesh() { return ground_mesh_; }
private:
	// box2d ground variables
	//b2Body* ground_body_;
	// ground mesh
	//gef::Mesh* ground_mesh_;
};

#endif // !_GROUND_H