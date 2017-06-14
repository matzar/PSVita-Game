#include "frontend.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <graphics/sprite.h>
#include <maths/math_utils.h>
#include <input/sony_controller_input_manager.h>
#include "load_texture.h"

#ifdef _WIN32
// only on windows platforms
#include <Windows.h>
#include <platform/d3d11/input/keyboard_d3d11.h>
#include <platform/d3d11/input/touch_input_manager_d3d11.h>
//#include <freeglut.h>
#endif 

Frontend::Frontend(gef::Platform& platform) :
	platform_(platform),
	button_icon_(NULL)
{
}


Frontend::~Frontend()
{
}

void Frontend::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void Frontend::FrontendInit()
{
	// initlalise sprite renderer
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	// initialise input manager
	input_manager_ = gef::InputManager::Create(platform_);
	// initialise audio manager
	audio_manager_ = gef::AudioManager::Create();

	InitFont();

	button_icon_ = CreateTextureFromPNG("playstation-cross-dark-icon.png", platform_);
}

void Frontend::FrontendRelease()
{
	delete button_icon_;
	button_icon_ = NULL;
}

void Frontend::FrontendUpdate(float frame_time)
{
	// get controller input data and read controller data for controler 0
	const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);

	if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS)
	{
		// release any resources for the frontend
		FrontendRelease();

		// update the current state for the game state machine
		game_state_ = GAME;

		// initialise game state
		
	}
}

void Frontend::FrontendRender()
{
	sprite_renderer_->Begin();

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
	sprite_renderer_->End();
}