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

	void InitPlayer(PrimitiveBuilder* primitve_builder, b2World* world);
	void DecrementHealth();
	
	void PlayerController(const gef::SonyController* controller);
	void startContact();
	void endContact();
	b2Body* GetPlayerBody() { return player_body_; }

	bool contacting_;
private:
	// player box 2d variables
	b2Body* player_body_;

	// jumping
	bool jump_;
};

#endif // !_PLAYER_H
