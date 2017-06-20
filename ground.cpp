#include "ground.h"
#include <system/debug_log.h>

Ground::Ground()
{
	set_type(GROUND);
}

Ground::~Ground()
{
}

void Ground::InitGround(PrimitiveBuilder* primitive_builder, b2World* world, b2Vec2 position)
{
	// ground dimensions
	gef::Vector4 ground_half_dimensions(5.0f, 0.5f, 0.5f);

	// setup the mesh for the ground
	ground_mesh_ = primitive_builder->CreateBoxMesh(ground_half_dimensions);
	set_mesh(ground_mesh_);

	// create a physics body
	b2BodyDef body_def;
	body_def.type = b2_staticBody;
	body_def.position = position;

	ground_body_ = world->CreateBody(&body_def);

	// create the shape
	b2PolygonShape shape;
	shape.SetAsBox(ground_half_dimensions.x(), ground_half_dimensions.y());

	// create the fixture
	b2FixtureDef fixture_def;
	fixture_def.shape = &shape;
	/*
	Since the default category for a fixture is 1, this arrangement means we don't need to do anything special for the boundary fixture 
	*/

	// create the fixture on the rigid body
	ground_body_->CreateFixture(&fixture_def);

	// update visuals from simulation data
	UpdateFromSimulation(ground_body_);

	// create a connection between the rigid body and GameObject
	//ground_body_->SetUserData(this);
}

//void Ground::CollisionResponse()
//{
//	
//}
