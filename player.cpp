#include "player.h"
#include <system/debug_log.h>

#include "input/sony_controller_input_manager.h"

Player::Player() :
	//body_(nullptr),
	jump_(false),
	alive_(true),
	red_(true),
	num_contacts_(0),
	x_velocity(5.0f),
	y_velocity(7.0f)
{
	//SetGameObjectType(PLAYER);
	//SetGameObjectColour(RED);
}

Player::~Player()
{
}

void Player::InitPlayer(
	PrimitiveBuilder* primitve_builder, 
	b2World* world,
	b2Vec2 position,
	float32 radius,
	uint16 category_bits, 
	uint16 mask_bits,
	uint16 group_index,
	OBJECT_TYPE type,
	OBJECT_COLOUR colour)
{
	//// set colour
	//colour_ = colour;

	// setup the mesh for the player
	set_mesh(primitve_builder->GetDefaultSphereMesh());

	// create a physics body for the player
	b2BodyDef player_body_def;
	player_body_def.type = b2_dynamicBody;
	player_body_def.position = position;
	player_body_def.angle = 0.0f;
	//player_body_def.fixedRotation;

	SetBody(world->CreateBody(&player_body_def));

	// create the shape for the player
	//b2PolygonShape player_shape;
	b2CircleShape player_shape;
	// if cube 1x1, need to pass half of both dimensions
	//player_shape.SetAsBox(0.5f, 0.5f);
	player_shape.m_radius = radius;

	// create the fixture
	b2FixtureDef player_fixture_def;
	player_fixture_def.shape = &player_shape;
	player_fixture_def.density = 1.0f;
	// filter mask fixture definition
	// I am a ... (category_bits)
	player_fixture_def.filter.categoryBits = category_bits;
	// I collide with ... (mask_bits)
	player_fixture_def.filter.maskBits = mask_bits;
	// group index 
	// if both groupIndex values are the same and positive, collide 
	// if both groupIndex values are the same and negative, don't collide
	player_fixture_def.filter.groupIndex = group_index;

	SetGameObjectType(type);
	SetGameObjectColour(colour);

	// create the fixture on the rigid body
	GetBody()->CreateFixture(&player_fixture_def);

	// update visuals from simulation data
	UpdateFromSimulation(GetBody());

	// create a connection between the rigid body and GameObject
	GetBody()->SetUserData(this);
}

void Player::DecrementHealth()
{
	gef::DebugOut("Player has taken damage.\n");
	jump_ = true;
}

void Player::PlayerController(const gef::SonyController * controller)
{
	if (alive_)
	{
		// move the player
		b2Vec2 vel = GetBody()->GetLinearVelocity();
		vel.x = x_velocity;
		GetBody()->SetLinearVelocity(vel);
		
		/*b2Vec2 vel = GetBody()->GetLinearVelocity();
		vel.x = b2Max(vel.x - 0.1f, -5.0f);
		vel.x *= 0.98;
		vel.x = b2Min(vel.x + 0.1f, 5.0f);
		GetBody()->SetLinearVelocity(vel);*/

		if (jump_)
		{
			if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS)
			{
				b2Vec2 vel = GetBody()->GetLinearVelocity();
				vel.y = y_velocity;	//upwards - don't change x velocity
				GetBody()->SetLinearVelocity(vel);

				jump_ = false;
			}
		}

		// TODO change colour at runtime
		if (controller->buttons_pressed() & gef_SONY_CTRL_SQUARE)
		{
			red_ = !red_;

			if (red_)
				this->SetGameObjectColour(RED);
			else
				this->SetGameObjectColour(BLUE);
		}
	}
	else
	{
		// stop the player
		b2Vec2 vel = GetBody()->GetLinearVelocity();
		vel.x = 0.0f;
		GetBody()->SetLinearVelocity(vel);
	}
}

void Player::StartContact()
{
	num_contacts_++;
	gef::DebugOut("Start Contact()\n");
}

void Player::EndContact()
{
	num_contacts_--;
	//jump_ = false;
	gef::DebugOut("EndContact()\n");
}

void Player::RestartJump()
{
	jump_ = true;
	gef::DebugOut("RestartJump()\n");
}

void Player::DeadPlayer()
{
	alive_ = false;
	gef::DebugOut("DeadPlayer()\n");
}


