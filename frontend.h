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
	class Texture;
	class SonyController;
	class TouchInputManager;
	class Sprite;
	class Vector2;
	class Vector4;
} // gef

// my class forward declarations
enum GAMESTATE;

class Frontend
{
public:
	Frontend(gef::Platform& platform, gef::InputManager* input_manager, GAMESTATE* gamestate);
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

	// sprite renderer
	gef::SpriteRenderer* sprite_renderer_;

	// font
	gef::Font* font_;

	float fps_;

	int sfx_id_;
	int sfx_voice_id_;

	unsigned instructions_page_;
	bool display_instrucitons_;

	// quit
	bool quit_;
	// toggle fps display
	bool dev_;

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

	bool display_d_pad;

	// menu text vectors
	gef::Vector4 menu_text_1_;
	gef::Vector4 menu_text_2_;
	gef::Vector4 menu_text_3_;
	gef::Vector4 menu_text_4_;
	
	// private member functions
	void SonyController(const gef::SonyController* controller);
	void TouchController(const gef::TouchInputManager* touch_input);
	void MenuTouchInput();
	Int32 active_touch_id_;
	gef::Vector2 touch_position_;

	void InitFont();
	void CleanFont();
	void DrawHUD();

	void InitTextures();
	void CleanTextures();

	void InitSprites();
	void CleanSprites();

	void InitText();
};

#endif // !_FRONTEND_H
