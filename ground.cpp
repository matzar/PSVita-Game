#include "ground.h"

Ground::Ground()
{
}

Ground::~Ground()
{
}

void Ground::InitGround(PrimitiveBuilder* primitive_builder, b2World* world)
{
	// create GameObject ground_ class
	//ground_ = new Ground();
	// ground dimensions
	gef::Vector4 ground_half_dimensions(5.0f, 0.5f, 0.5f);

	// setup the mesh for the ground
	ground_mesh_ = primitive_builder->CreateBoxMesh(ground_half_dimensions);
	ground_->set_mesh(ground_mesh_);

	// create a physics body
	b2BodyDef body_def;
	body_def.type = b2_staticBody;
	body_def.position = b2Vec2(0.0f, 0.0f);

	ground_body_ = world->CreateBody(&body_def);

	// create the shape
	b2PolygonShape shape;
	shape.SetAsBox(ground_half_dimensions.x(), ground_half_dimensions.y());

	// create the fixture
	b2FixtureDef fixture_def;
	fixture_def.shape = &shape;

	// create the fixture on the rigid body
	ground_body_->CreateFixture(&fixture_def);

	// update visuals from simulation data
	ground_->UpdateFromSimulation(ground_body_);
}
