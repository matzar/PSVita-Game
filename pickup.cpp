#include "pickup.h"
#include "primitive_builder.h"

Pickup::Pickup()
{
	SetGameObjectType(PICKUP);
}

Pickup::~Pickup()
{
}

void Pickup::InitPickUp(
	PrimitiveBuilder* primitive_builder, 
	b2World* world, 
	b2Vec2 position, 
	uint16 category_bits, 
	uint16 mask_bits, 
	uint16 group_index)
{
	// setup the mesh for the pickup
	set_mesh(primitive_builder->GetDefaultSphereMesh());

	// create a physics body for the pickup
	b2BodyDef pickup_body_def;
	pickup_body_def.type = b2_staticBody;
	pickup_body_def.position = b2Vec2(1.0f, 1.0f);
	pickup_body_def.angle = 0.0f;
	pickup_body_def.fixedRotation;

	body_ = world->CreateBody(&pickup_body_def);

	// create the shape for the pickup
	b2CircleShape pickup_shape;
	pickup_shape.m_radius = 0.5f;

	// create the fixture
	b2FixtureDef pickup_fixture_def;
	pickup_fixture_def.shape = &pickup_shape;
	pickup_fixture_def.density = 1.0f;
	// filter mask fixture definition
	// I am a ... (category_bits)
	pickup_fixture_def.filter.categoryBits = category_bits;
	// I collide with ... (mask_bits)
	pickup_fixture_def.filter.maskBits = mask_bits;
	// group index 
	// if both groupIndex values are the same and positive, collide 
	// if both groupIndex values are the same and negative, don't collide
	pickup_fixture_def.filter.groupIndex = group_index;

	// create the fixture on the rigid body
	body_->CreateFixture(&pickup_fixture_def);

	// update visuals from simulation data
	UpdateFromSimulation(body_);

	// create a connection between the rigid body and GameObject
	body_->SetUserData(this);
}
