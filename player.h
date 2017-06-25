#ifndef _PLAYER_H
#define _PLAYER_H

// extra headers
#include "game_object.h"

// forward gef declarations
namespace gef
{
	class SonyController;
}

class PrimitiveBuilder;

class Player : 
	public GameObject 
{
public:
	Player();
	~Player();

	void InitPlayer(
		PrimitiveBuilder* primitve_builder,
		b2World* world,
		b2Vec2 position,
		float32 radius,
		uint16 categoryBits,
		uint16 maskBits,
		uint16 group_index,
		OBJECT_TYPE type,
		OBJECT_COLOUR colour);

	void DecrementHealth();
	
	void PlayerController(const gef::SonyController* controller);
	void StartContact();
	void EndContact();
	void RestartJump();
	void DeadPlayer();

	int IsContacting() { return num_contacts_; }
	bool PlayerIsRed() { return red_; }
	//b2Body* GetPlayerBody() { return player_body_; }
private:
	// player box 2d variables
	// player body
	//b2Body* player_body_;
	// player colour
	//b2Color colour_;

	// jumping
	bool jump_;

	// player alive
	bool alive_;

	// player colour
	bool red_;

	// contacting
	int num_contacts_;

	// player's x velocity
	float32 x_velocity;
	// player's y velocity (jumping force)
	float32 y_velocity;
};

#endif // !_PLAYER_H
