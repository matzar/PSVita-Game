#ifndef _FRONTEND_H
#define _FRONTEND_H

#include <system/application.h>
#include <graphics/font.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>
#include <input/input_manager.h>
#include <audio/audio_manager.h>
#include <box2d/Box2D.h>
#include "game_object.h"
#include "free_camera.h"
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

class Frontend
{
public:
	Frontend(gef::Platform& platform, GAMESTATE* gamestate);
	~Frontend();

	void InitFont();
	void FrontendInit();
	void FrontendRelease();
	void FrontendUpdate(float frame_time);
	void FrontendRender();
private:
	gef::SpriteRenderer* sprite_renderer_;

	gef::InputManager* input_manager_;

	gef::Platform& platform_;
	gef::Font* font_;
	gef::Texture* button_icon_;

	void DrawHUD();

	float fps_;
	GAMESTATE* gamestate;
};

#endif // !_FRONT_END_H
