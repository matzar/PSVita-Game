#ifndef _SETTINGS_H
#define _SETTINGS_H

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

// My class forward declarations
enum GAMESTATE;

class Settings
{
public:
	Settings(gef::Platform& platform, gef::InputManager* input_manager, GAMESTATE* gamestate, unsigned* camera_count, unsigned* difficulty_count, unsigned* number_of_grounds);
	~Settings();

	void SettingsInit();
	void SettingsRelease();
	void SettingsUpdate(float frame_time);
	void SettingsRender();

	inline const bool Quit() { return quit_; }
private:
	// platform 
	gef::Platform& platform_;

	// gamestate
	GAMESTATE* gamestate_;

	// camera
	unsigned* camera_count_;

	// difficulty
	unsigned* difficulty_count_;
	// number of grounds
	unsigned* number_of_grounds_;

	// input manager
	gef::InputManager* input_manager_;

	// sprite renderer
	gef::SpriteRenderer* sprite_renderer_;

	// icons textures
	gef::Texture* playstation_left_d_pad_;
	gef::Texture* playstation_right_d_pad_;

	// camera illustration textures
	gef::Texture* camera_1_texture_;
	gef::Texture* camera_2_texture_;
	gef::Texture* camera_3_texture_;

	// font
	gef::Font* font_;
	
	float fps_;

	int sfx_id_;
	int sfx_voice_id_;

	// quit
	bool quit_;
	// toggle fps display
	bool dev_;

	// menu box sprite
	gef::Sprite menu_box_sprite_;
	float sprite_width_;
	float sprite_height;

	// d-pad sprites
	gef::Sprite left_d_pad_sprite_;
	gef::Sprite right_d_pad_sprite_;

	bool display_d_pad = true;

	// menu text vectors
	gef::Vector4 menu_text_1_;
	gef::Vector4 menu_text_2_;
	gef::Vector4 menu_text_3_;
	gef::Vector4 menu_text_4_;
	// menu text height correction
	float height_correction_;

	// controlls
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

#endif // !_SETTINGS_H

