#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

// gef headers
#include <graphics/mesh_instance.h>
// my headers
#include "object_type_enum.h"
// extra headers
#include "primitive_builder.h"
// box2D header
#include <box2d/Box2D.h>

class GameObject : public gef::MeshInstance
{
public:
	GameObject() {}
	virtual ~GameObject() {}
	void UpdateFromSimulation(const b2Body* body);
	//void MyCollisionResponse();

	inline void set_type(OBJECT_TYPE type) { type_ = type; }
	inline OBJECT_TYPE type() { return type_; }
private:
	OBJECT_TYPE type_;
};

#endif // !_GAME_OBJECT_H