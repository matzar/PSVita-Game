#include "player.h"
// gef headers
#include <input/touch_input_manager.h>
#include <input/sony_controller_input_manager.h>

Player::Player(float32* x_velocity, float32* y_velocity) :
	p_x_velocity(x_velocity), // 5.0
	p_y_velocity(y_velocity),	// 7.5
	jump_(false),
	alive_(true),
	win_(false),
	red_(true),
	num_contacts_(0)
{
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

void Player::PlayerController(const gef::SonyController * controller)
{
	if (alive_)
	{
		// move the player
		b2Vec2 vel = GetBody()->GetLinearVelocity();
		vel.x = (*p_x_velocity);
		GetBody()->SetLinearVelocity(vel);
		
		if (jump_)
		{
			if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS)
			{
				b2Vec2 vel = GetBody()->GetLinearVelocity();
				vel.y = (*p_y_velocity);	//upwards - don't change x velocity
				GetBody()->SetLinearVelocity(vel);

				jump_ = false;
			}
		}

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

void Player::PlayerTouchController(gef::Vector2 touch_position_)
{
	// tap on the right side of the screen - jump
	if (touch_position_.x > 480.0f && // 480 is half of PSVita's screen in the x direction
		touch_position_.y > 100.0f)    // prevent jump getting triggered when pressing the pause button
	{
		if (alive_)
		{
			if (jump_)
			{
				b2Vec2 vel = GetBody()->GetLinearVelocity();
				vel.y = (*p_y_velocity);	//upwards - don't change x velocity
				GetBody()->SetLinearVelocity(vel);

				jump_ = false;
			}
		}
	}
	// tap on the left side of the screen - change colour
	// 480 is half of PSVita's screen in the x direction
	if (touch_position_.x < 480.0f) 
	{
		if (alive_)
		{
			red_ = !red_;

			if (red_)
				this->SetGameObjectColour(RED);
			else
				this->SetGameObjectColour(BLUE);
		}
	}
}
			
// future improvement
void Player::ReloadPlayer()
{
	body_->SetTransform(b2Vec2(-4.0f, 4.0f), 0.0f);

	// in case player falls into the nether reset y velocity
	b2Vec2 vel = GetBody()->GetLinearVelocity();
	vel.y = 0.0f;
	GetBody()->SetLinearVelocity(vel);

	SetAlive(true);
	SetWin(false);

	if (this->GetGameObjectColour() == RED)
		RedPlayer(true);
	else
		RedPlayer(false);
}


