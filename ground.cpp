#include "ground.h"
#include <system/debug_log.h>

Ground::Ground()
{
	set_type(GROUND);
}

Ground::~Ground()
{
}

void Ground::InitGround(
	PrimitiveBuilder* primitive_builder, 
	b2World* world, 
	b2Vec2 position,
	uint16 category_bits,
	uint16 mask_bits)
{
	// ground dimensions
	gef::Vector4 ground_half_dimensions(5.0f, 0.5f, 0.5f);

	// setup the mesh for the ground
	ground_mesh_ = primitive_builder->CreateBoxMesh(ground_half_dimensions);
	set_mesh(ground_mesh_);

	// create a physics body
	b2BodyDef ground_body_def;
	ground_body_def.type = b2_staticBody;
	ground_body_def.position = position;
	ground_body_def.angle = 0.0f;

	ground_body_ = world->CreateBody(&ground_body_def);

	// create the shape
	b2PolygonShape shape;
	shape.SetAsBox(ground_half_dimensions.x(), ground_half_dimensions.y());

	// create the fixture
	b2FixtureDef ground_fixture_def;
	ground_fixture_def.shape = &shape;
	// filter mask fixture definition
	// I am a ... (category_bits)
	ground_fixture_def.filter.categoryBits = category_bits;
	// I collide with ... (mask_bits)
	ground_fixture_def.filter.maskBits = mask_bits;
	// group index 
	// if both groupIndex values are the same and positive, collide 
	// if both groupIndex values are the same and negative, don't collide
	ground_fixture_def.filter.groupIndex = -8;

	/*
	Since the default category for a fixture is 1, this arrangement means we don't need to do anything special for the boundary fixture 
	*/

	// create the fixture on the rigid body
	ground_body_->CreateFixture(&ground_fixture_def);

	// update visuals from simulation data
	UpdateFromSimulation(ground_body_);

	// create a connection between the rigid body and GameObject
	//ground_body_->SetUserData(this);
}

//void Ground::CollisionResponse()
//{
//	
//}
