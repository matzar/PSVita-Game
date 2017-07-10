#ifndef _PLAYER_H
#define _PLAYER_H

// extra headers
#include "game_object.h"
// gef headers
#include <maths/vector2.h>

// forward gef declarations
namespace gef
{
	class SonyController;
	class TouchInputManager;
}

class PrimitiveBuilder;

class Player : 
	public GameObject 
{
public:
	Player(float32* x_velocity, float32* y_velocity);
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
	
	void ReloadPlayer();
	void PlayerController(const gef::SonyController* controller);
	void PlayerTouchController(const gef::TouchInputManager* touch_input, Int32 active_touch_id_, gef::Vector2 touch_position_);
	inline void StartContact() { num_contacts_++; }
	inline void EndContact() { num_contacts_--; }
	inline void RestartJump() { jump_ = true; }
	inline void SetAlive(bool alive) { alive_ = alive; }
	inline const bool Alive() { return alive_; }
	inline void SetWin(bool win) { win_ = win; }
	inline const bool Win() { return win_; }

	int IsContacting() const { return num_contacts_; }
	bool IsAlive() const { return alive_; }
	void RedPlayer(bool red) { red_ = red; }
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

	// player win
	bool win_;

	// player colour
	bool red_;

	// contacting
	int num_contacts_;

	Int32 active_touch_id_;
	gef::Vector2 touch_position_;

	// player's x velocity
	float32* p_x_velocity;
	// player's y velocity (jumping force)
	float32* p_y_velocity;
};

#endif // !_PLAYER_H
