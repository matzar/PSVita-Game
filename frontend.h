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
	class SonyController;
} // gef

class Frontend
{
public:
	Frontend(gef::Platform& platform, GAMESTATE* gamestate);
	~Frontend();

	void FrontendInit();
	void FrontendRelease();
	void FrontendUpdate(float frame_time);
	void FrontendRender();

	inline const bool Quit() { return quit_; }
private:
	// platform 
	gef::Platform& platform_;

	// gamestate
	GAMESTATE* gamestate_;

	// input manager
	gef::InputManager* input_manager_;

	// audio manager
	gef::AudioManager* audio_manager_;

	// sprite renderer
	gef::SpriteRenderer* sprite_renderer_;

	// texture
	gef::Texture* button_icon_;

	// font
	gef::Font* font_;

	float fps_;

	int sfx_id_;
	int sfx_voice_id_;

	bool quit_;

	void SonyController(const gef::SonyController* controller);
#ifdef _WIN32
	void KeyboardController(float fps);
#endif // _WIN32
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
	void InitAudio();
};

#endif // !_FRONT_END_H
