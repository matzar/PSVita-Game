#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

// gef headers
#include <graphics/mesh_instance.h>
// my headers
#include "object_type_enum.h"
#include "colour_type_enum.h"
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

	/*inline void SetBody(b2Body* body) { body_ = body; }
	inline b2Body* GetBody() { return body_; }

	inline void SetMest(gef::Mesh* mesh) { mesh_ = mesh; }
	inline gef::Mesh* GetMesh() { return mesh_; }*/

	inline void SetGameObjectType(OBJECT_TYPE type) { type_ = type; }
	inline OBJECT_TYPE GetGameObjectType() { return type_; }

	inline void SetGameObjectColour(OBJECT_COLOUR colour) { colour_ = colour; }
	inline OBJECT_COLOUR GetGameObjectColour() { return colour_; }

	b2Body* body_;
	gef::Mesh* mesh_;
private:
	OBJECT_TYPE type_;
	OBJECT_COLOUR colour_;
};

#endif // !_GAME_OBJECT_H