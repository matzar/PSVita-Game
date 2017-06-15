#ifndef _SCENE_APP_H
#define _SCENE_APP_H

#include <system/application.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>
#include <input/input_manager.h>
#include <audio/audio_manager.h>
#include <box2d/Box2D.h>
#include "game_object.h"
#include "free_camera.h"
#include "game.h"
#include "frontend.h"
#include "game_state_enum.h"

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class InputManager;
	class Renderer3D;
} // gef

class SceneApp : public gef::Application
{
public:
	SceneApp(gef::Platform& platform);

	// Application virtual template classes - need to be defined
	virtual void Init();
	virtual void CleanUp();
	virtual bool Update(float frame_time);
	virtual void Render();
private:
	GAMESTATE game_state_;

	float fps_;

	Game* game_;
	Frontend* frontend_;

	friend class Game;
	friend class Frontend;
}; // class SceneApp : public gef::Application

#endif // _SCENE_APP_H