#include "player.h"
#include <system/debug_log.h>

#include "input/sony_controller_input_manager.h"

Player::Player() :
	player_body_(nullptr),
	jump_(false),
	num_contacts_(0)
{
	set_type(PLAYER);
}

Player::~Player()
{
}

void Player::InitPlayer(
	PrimitiveBuilder* primitve_builder, 
	b2World* world, 
	uint16 category_bits, 
	uint16 mask_bits)
{
	//// set colour
	//colour_ = colour;

	// setup the mesh for the player
	set_mesh(primitve_builder->GetDefaultCubeMesh());

	// create a physics body for the player
	b2BodyDef player_body_def;
	player_body_def.type = b2_dynamicBody;
	player_body_def.position = b2Vec2(0.0f, 4.0f);

	player_body_ = world->CreateBody(&player_body_def);

	// create the shape for the player
	b2PolygonShape player_shape;
	// if cube 1x1, need to pass half of both dimensions
	player_shape.SetAsBox(0.5f, 0.5f);

	// create the fixture
	b2FixtureDef player_fixture_def;
	player_fixture_def.shape = &player_shape;
	player_fixture_def.density = 1.0f;
	// filter mask fixture definition
	// I am a ... (category_bits)
	player_fixture_def.filter.categoryBits = category_bits;
	// I collide with ... (mask_bits)
	player_fixture_def.filter.maskBits = mask_bits;

	// create the fixture on the rigid body
	player_body_->CreateFixture(&player_fixture_def);

	// update visuals from simulation data
	UpdateFromSimulation(player_body_);

	// create a connection between the rigid body and GameObject
	player_body_->SetUserData(this);
}

void Player::DecrementHealth()
{
	gef::DebugOut("Player has taken damage.\n");
	jump_ = true;
}

void Player::PlayerController(const gef::SonyController * controller)
{
	if (jump_)
	{
		if (controller->buttons_pressed() & gef_SONY_CTRL_SQUARE)
		{
			player_body_->ApplyLinearImpulseToCenter(b2Vec2(0.0f, 5.0f), true);
			//player_body_->ApplyLinearImpulse(b2Vec2(0, 10), player_body_->GetWorldCenter(), true);
			//player_body_->ApplyForce(b2Vec2(0.0f, 10.0f), player_body_->GetWorldCenter(), true);
			jump_ = false;
		}
	}
}

void Player::StartContact()
{
	num_contacts_++;
	//jump_ = true;
	//gef::DebugOut("Start Contact\n");
}

void Player::EndContact()
{
	num_contacts_--;
	//gef::DebugOut("End Contact\n");
}
