#ifndef _PLAYER_H
#define _PLAYER_H

// extra headers
#include "game_object.h"

class PrimitiveBuilder;

class Player : 
	public GameObject 
{
public:
	Player();
	~Player();

	void InitPlayer(PrimitiveBuilder* primitve_builder, b2World* world);
	void DecrementHealth();
	
	// player variables
	// player box 2d variables
	b2Body* player_body_;
};

#endif // !_PLAYER_H
