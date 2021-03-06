#include "pickup.h"

Pickup::Pickup()
{
	
}

Pickup::~Pickup()
{
	//this->GetBody()->GetWorld()->DestroyBody(this->GetBody());
	this->GetBody()->SetActive(false);
}

void Pickup::InitPickup(
	PrimitiveBuilder* primitive_builder, 
	b2World* world, 
	b2Vec2 position,
	float32 radius,
	gef::Mesh* mesh,
	uint16 category_bits, 
	uint16 mask_bits, 
	uint16 group_index,
	OBJECT_TYPE type)
{
	// setup the mesh for the pickup
	// set from a model
	set_mesh(mesh);
	
	// create a physics body for the pickup
	b2BodyDef pickup_body_def;
	pickup_body_def.type = b2_staticBody;
	pickup_body_def.position = position;
	pickup_body_def.angle = 0.0f;

	SetBody(world->CreateBody(&pickup_body_def));

	// create the shape for the pickup
	b2CircleShape pickup_shape;
	pickup_shape.m_radius = radius;

	// create the fixture
	b2FixtureDef pickup_fixture_def;
	pickup_fixture_def.shape = &pickup_shape;
	pickup_fixture_def.density = 1.0f;
	pickup_fixture_def.isSensor = true;
	// filter mask fixture definition
	// I am a ... (category_bits)
	pickup_fixture_def.filter.categoryBits = category_bits;
	// I collide with ... (mask_bits)
	pickup_fixture_def.filter.maskBits = mask_bits;
	// group index 
	// if both groupIndex values are the same and positive, collide 
	// if both groupIndex values are the same and negative, don't collide
	pickup_fixture_def.filter.groupIndex = group_index;

	SetGameObjectType(type);
	SetGameObjectColour(NO_COL);

	// create the fixture on the rigid body
	GetBody()->CreateFixture(&pickup_fixture_def);

	// update visuals from simulation data
	UpdateFromSimulation(GetBody());

	// create a connection between the rigid body and GameObject
	GetBody()->SetUserData(this);
} // !InitPickup

void Pickup::InitPickup(
	PrimitiveBuilder* primitive_builder,
	b2World* world,
	b2Vec2 position,
	float32 radius,
	uint16 category_bits,
	uint16 mask_bits,
	uint16 group_index,
	OBJECT_TYPE type)
{
	// set from the primitive builder
	set_mesh(primitive_builder->CreateSphereMesh(radius, 10, 10, gef::Vector4(0.0f, 0.0f, 0.0f), (gef::Material*)(&primitive_builder->green_material())));

	// create a physics body for the pickup
	b2BodyDef pickup_body_def;
	pickup_body_def.type = b2_staticBody;
	pickup_body_def.position = position;
	pickup_body_def.angle = 0.0f;

	SetBody(world->CreateBody(&pickup_body_def));

	// create the shape for the pickup
	b2CircleShape pickup_shape;
	pickup_shape.m_radius = radius;

	// create the fixture
	b2FixtureDef pickup_fixture_def;
	pickup_fixture_def.shape = &pickup_shape;
	pickup_fixture_def.density = 1.0f;
	pickup_fixture_def.isSensor = true;
	// filter mask fixture definition
	// I am a ... (category_bits)
	pickup_fixture_def.filter.categoryBits = category_bits;
	// I collide with ... (mask_bits)
	pickup_fixture_def.filter.maskBits = mask_bits;
	// group index 
	// if both groupIndex values are the same and positive, collide 
	// if both groupIndex values are the same and negative, don't collide
	pickup_fixture_def.filter.groupIndex = group_index;

	SetGameObjectType(type);
	SetGameObjectColour(NO_COL);

	// create the fixture on the rigid body
	GetBody()->CreateFixture(&pickup_fixture_def);

	// update visuals from simulation data
	UpdateFromSimulation(GetBody());

	// create a connection between the rigid body and GameObject
	GetBody()->SetUserData(this);
} // !InitPickup