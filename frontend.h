#ifndef _FRONTEND_H
#define _FRONTEND_H

// mu headers
#include "game_state_enum.h"

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class Font;
	class SpriteRenderer;
	class InputManager;
	class AudioManager;
	class Texture;
} // gef

class Frontend
{
public:
	Frontend(gef::Platform& platform, GAMESTATE* gamestate, Frontend* frontend);
	~Frontend();

	void FrontendInit();
	void FrontendRelease();
	void FrontendUpdate(float frame_time);
	void FrontendRender();
private:
	// platform 
	gef::Platform& platform_;

	// gamestate
	GAMESTATE* gamestate_;

	// frontend
	Frontend* frontend_;

	// input manager
	gef::InputManager* input_manager_;

	// sprite renderer
	gef::SpriteRenderer* sprite_renderer_;

	// texture
	gef::Texture* button_icon_;

	// font
	gef::Font* font_;

	float fps_;

	void InitFont();
	void CleanUpFont();
	void DrawHUD();
};

#endif // !_FRONT_END_H
