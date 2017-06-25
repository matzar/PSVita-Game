#include "frontend.h"
// gef headers
#include <system/platform.h>
#include <system/debug_log.h>

#include <graphics/font.h>

#include <graphics/sprite_renderer.h>
#include <graphics/sprite.h>

#include <input/input_manager.h>
#include <input/sony_controller_input_manager.h>
#include <audio/audio_manager.h>

#include <maths/vector4.h>
// extra headers
#include "load_texture.h"

#ifdef _WIN32
// only on windows platforms
#include <platform/d3d11/input/keyboard_d3d11.h>
#include <platform/d3d11/input/touch_input_manager_d3d11.h>
#endif 

Frontend::Frontend(gef::Platform& platform, GAMESTATE* gamestate) :
	platform_(platform),
	gamestate_(gamestate),
	input_manager_(nullptr),
	audio_manager_(nullptr),
	button_icon_(nullptr),
	sfx_voice_id_(-1),
	sfx_id_(-1),
	fps_(0)
{
}


Frontend::~Frontend()
{
}

void Frontend::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
} // !InitFont

void Frontend::CleanUpFont()
{
	delete font_;
	font_ = nullptr;
} // CleanUpFont

void Frontend::DrawHUD()
{
	if (font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	} // !font_
} // !DrawHUD

void Frontend::InitAudio()
{
	// audio manager
	audio_manager_ = gef::AudioManager::Create();

	// load audio assets
	if (audio_manager_)
	{
		// load a sound effect
		sfx_voice_id_ = audio_manager_->LoadSample("box_collected.wav", platform_);

		// load in music
		audio_manager_->LoadMusic("music.wav", platform_);

		// play music
		audio_manager_->PlayMusic();
	}
} // !InitAudio

void Frontend::FrontendInit()
{
	// initialise input manager
	input_manager_ = gef::InputManager::Create(platform_);
	// initlalise sprite renderer
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

	InitAudio();

	// TODO createing material from the texture
	// initialise button icon
	button_icon_ = CreateTextureFromPNG("playstation-cross-dark-icon.png", platform_);
	
	InitFont();
} // !FrontendInit

void Frontend::FrontendRelease()
{
	delete input_manager_;
	input_manager_ = nullptr;

	delete sprite_renderer_;
	sprite_renderer_ = nullptr;

	delete audio_manager_;
	audio_manager_ = nullptr;

	delete button_icon_;
	button_icon_ = nullptr;

	CleanUpFont();
} // !FrontendRelease

void Frontend::SonyController(const gef::SonyController* controller)
{
	if (controller)
	{
		if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS)
		{
			// TODO release any resources for the frontend


			// update the current state for the game state machine
			(*gamestate_) = GAME; // get the object that gamestate points to
		}

		// trigger a sound effect
		if (audio_manager_)
		{
			if (controller->buttons_pressed() & gef_SONY_CTRL_CIRCLE)
			{
				audio_manager_->PlaySample(sfx_voice_id_);
				/*audio_manager_->StopPlayingSampleVoice(sfx_voice_id_);
				sfx_voice_id_ = -1;*/
			}
		}
	} // !audio_manager_
} // !SonyController

#ifdef _WIN32 // Only on windows platforms
void Frontend::KeyboardController(float frame_time)
{
	// if there is a keyboard, check the arrow keys to control the direction of the character
	gef::Keyboard* keyboard = input_manager_->keyboard();
	if (keyboard)
	{
		//const gef::KeyboardD3D11* keyboard_d3d11 = (const gef::KeyboardD3D11*)keyboard;
		float camera_speed = 10.0f;

		// keyboard input
		if (keyboard->IsKeyDown(gef::Keyboard::KC_X))
		{
			// update the current state for the game state machine
			(*gamestate_) = GAME; // get the object that gamestate points to
		}
			
	} // keyboard

	  // mouse input
	const gef::TouchInputManager* touch_input = input_manager_->touch_manager();
	if (touch_input)
	{
		// initialise the mouse position
		gef::Vector2 mouse_position(0.0f, 0.0f); // left upper corner of the window

												 // get a pointer to the d3d11 implementation of the TouchInputManager
		const gef::TouchInputManagerD3D11* touch_input_d3d11 = (const gef::TouchInputManagerD3D11*)touch_input;

		// get the mouse position
		mouse_position = touch_input_d3d11->mouse_position();

		if (touch_input_d3d11->is_button_down(0))
		{
			//SetCursorPos(480, 272);	
		}

		//gef::DebugOut("Mouse position x, y: %f %f\n", mouse_position.x, mouse_position.y);
	} // touch_input (mouse)
}
#endif // !_WIN32

void Frontend::FrontendUpdate(float frame_time)
{
	fps_ = 1.0f / frame_time;

	// get the latest date from the input devices
	if (input_manager_)
	{
		input_manager_->Update();

		const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);

		SonyController(controller);
#ifdef _WIN32
		KeyboardController(frame_time);
#endif // _WIN32
	} // !input_manager_
} // !FrontendUpdate

void Frontend::FrontendRender()
{
	sprite_renderer_->Begin();
	{
		// render "PRESS" text
		font_->RenderText(
			sprite_renderer_,
			gef::Vector4(platform_.width()*0.5f, platform_.height()*0.5f - 56.0f, -0.99f),
			1.0f,
			0xffffffff,
			gef::TJ_CENTRE,
			"PRESS");

		// Render button icon
		gef::Sprite button;
		button.set_texture(button_icon_);
		button.set_position(gef::Vector4(platform_.width()*0.5f, platform_.height()*0.5f, -0.99f));
		button.set_height(32.0f);
		button.set_width(32.0f);
		sprite_renderer_->DrawSprite(button);


		// render "TO START" text
		font_->RenderText(
			sprite_renderer_,
			gef::Vector4(platform_.width()*0.5f, platform_.height()*0.5f + 32.0f, -0.99f),
			1.0f,
			0xffffffff,
			gef::TJ_CENTRE,
			"TO START");


		DrawHUD();
	}
	sprite_renderer_->End();
} // !FrontendRender
