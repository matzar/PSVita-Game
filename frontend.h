#ifndef _FRONTEND_H
#define _FRONTEND_H

// gef headers
#include <graphics/sprite.h>

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
	class Sprite;
	class Vector2;
	class Vector4;
} // gef

// my class forward declarations
enum GAMESTATE;

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

	// font
	gef::Font* font_;

	float fps_;

	int sfx_id_;
	int sfx_voice_id_;

	unsigned instructions_page_;
	bool display_instrucitons_;

	bool quit_;

	// title texture
	gef::Texture* title_texture_;
	// instructions textures
	gef::Texture* instructions_texture_1;
	gef::Texture* instructions_texture_2;
	gef::Texture* instructions_texture_3;
	gef::Texture* instructions_texture_4;
	gef::Texture* instructions_texture_5;
	// icons textures
	gef::Texture* cross_button_icon_;
	gef::Texture* playstation_left_d_pad_;
	gef::Texture* playstation_right_d_pad_;

	// sprite
	gef::Sprite menu_box_sprite_;
	float sprite_width_;
	float sprite_height;
	float sprite_init_position_y_;
	// d-pad sprites
	gef::Sprite left_d_pad_sprite_;
	gef::Sprite right_d_pad_sprite_;

	bool display_d_pad = true;

	// menu text vectors
	gef::Vector4 start_text_position_;
	gef::Vector4 settings_text_position_;
	gef::Vector4 instructions_text_position_;

	void SonyController(const gef::SonyController* controller);

	void InitFont();
	void CleanFont();
	void DrawHUD();

	void InitTextures();
	void CleanTextures();

	void InitSprites();
	void CleanSprites();

	void InitText();

	void InitAudio();
	void CleanAudio();
};

#endif // !_FRONTEND_H
