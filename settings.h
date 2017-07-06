#ifndef _SETTINGS_H
#define _SETTINGS_H

// gef headers
#include <input/touch_input_manager.h>
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

// My class forward declarations
enum GAMESTATE;

class Settings
{
public:
	Settings(gef::Platform& platform, GAMESTATE* gamestate, unsigned* camera_count, unsigned* difficulty_count);
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

	// input manager
	gef::InputManager* input_manager_;

	// audio manager
	gef::AudioManager* audio_manager_;

	// sprite renderer
	gef::SpriteRenderer* sprite_renderer_;

	// icons textures
	gef::Texture* cross_button_icon_;
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

	bool quit_;

	// touch variables
	Int32 active_touch_id_;
	gef::Vector2 touch_position_;

	// menu box sprite
	gef::Sprite menu_box_sprite_;
	float sprite_width_;
	float sprite_height;
	float sprite_init_position_y_;
	// d-pad sprites
	gef::Sprite left_d_pad_sprite_;
	gef::Sprite right_d_pad_sprite_;

	bool display_d_pad = true;

	// lerp move variables
	//gef::Vector4 sprite_lerp_;
	//gef::Vector4 menu_box_sprite_end_position_to_lerp_;
	//gef::Vector4 left_d_pad_sprite_end_position_to_lerp_;
	//gef::Vector4 right_d_pad_sprite_end_position_to_lerp_;
	// menu text vectors
	gef::Vector4 camera_text_position_;
	gef::Vector4 difficulty_text_position_;
	gef::Vector4 back_text_position_;

	void SonyController(const gef::SonyController* controller);
#ifdef _WIN32
	void KeyboardController(float fps);
#endif // !_WIN32
	void ProcessTouchInput(); // TODO delete if not needed

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

#endif // !_SETTINGS_H

