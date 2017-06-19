#ifndef _GROUND_H
#define _GROUND_H

#include "game_object.h"

class Ground : public GameObject
{
public:
	Ground();
	~Ground();

	void InitGround(PrimitiveBuilder* primitive_builder, b2World* world);
	void CollisionResponse();

	b2Body* GetGroundBody() { return ground_body_; }
	gef::Mesh* GetGroundMesh() { return ground_mesh_; }
private:
	// ground mesh
	gef::Mesh* ground_mesh_;
	// box2d ground variables
	b2Body* ground_body_;
};

#endif // !_GROUND_H