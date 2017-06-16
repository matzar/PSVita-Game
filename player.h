#ifndef _PLAYER_H
#define _PLAYER_H

// extra headers
#include "game_object.h"

class Player : 
	public GameObject 
{
public:
	Player();
	~Player();

	void DecrementHealth();
};

#endif // !_PLAYER_H
