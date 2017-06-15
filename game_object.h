#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

// gef headers
#include <graphics/mesh_instance.h>
// box2D headers
#include <box2d/Box2D.h>

enum OBJECT_TYPE
{
	PLAYER,
	ENEMY,
	BULLET
};

class GameObject : public gef::MeshInstance
{
public:
	void UpdateFromSimulation(const b2Body* body);
	void MyCollisionResponse();

	inline void set_type(OBJECT_TYPE type) { type_ = type;  }
	inline OBJECT_TYPE type() { return type_; }
private:
	OBJECT_TYPE type_;
};

class Player : public GameObject 
{
public:
	Player();
	void DecrementHealth();
};


#endif // _GAME_OBJECT_H