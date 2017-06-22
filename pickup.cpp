#include "pickup.h"
#include "primitive_builder.h"

Pickup::Pickup(Platform& platform) :
	platform_(platform)
{
	/*SetGameObjectType(PICKUP);*/
}

Pickup::~Pickup()
{
	// destroy pickup
	//GetBody()->SetActive(false);
	//GetBody()->GetWorld()->DestroyBody(GetBody());
	//~GameObject();
	GetBody()->SetActive(false);
	//mesh_->~Mesh();
	this->primitive_builder_->CleanUp();
}

void Pickup::InitPickup(
	PrimitiveBuilder* primitive_builder, 
	Platform& platform,
	b2World* world, 
	b2Vec2 position,
	float32 radius,
	uint16 category_bits, 
	uint16 mask_bits, 
	uint16 group_index,
	OBJECT_TYPE type)
{
	platform_ = platform;
	primitive_builder_ = new PrimitiveBuilder(platform);
	// setup the mesh for the pickup
	set_mesh(primitive_builder_->CreateSphereMesh(radius, 10, 10, gef::Vector4(0.0f, 0.0f, 0.0f), (gef::Material*)(&primitive_builder->blue_material())));
	//set_mesh(primitive_builder->GetDefaultSphereMesh());

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

	// create the fixture on the rigid body
	GetBody()->CreateFixture(&pickup_fixture_def);

	// update visuals from simulation data
	UpdateFromSimulation(GetBody());

	// create a connection between the rigid body and GameObject
	GetBody()->SetUserData(this);
}
