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

	void InitPlayer(PrimitiveBuilder* primitve_builder, b2World* world, uint16 categoryBits, uint16 maskBits);
	void DecrementHealth();
	
	void PlayerController(const gef::SonyController* controller);
	void StartContact();
	void EndContact();

	int IsContacting() { return num_contacts_; }
	b2Body* GetPlayerBody() { return player_body_; }
private:
	// player box 2d variables
	// player body
	b2Body* player_body_;
	// player colour
	b2Color colour_;

	// jumping
	bool jump_;

	// contacting
	int num_contacts_;
};

#endif // !_PLAYER_H
