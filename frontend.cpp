#include "frontend.h"
// gef headers
#include <system/platform.h>
#include <system/debug_log.h>

#include <graphics/font.h>

#include <graphics/sprite_renderer.h>
#include <graphics/sprite.h>

#include <input/input_manager.h>
#include <input/sony_controller_input_manager.h>
#include <input/touch_input_manager.h>

#include <audio/audio_manager.h>

#include <maths/vector2.h>
#include <maths/vector4.h>
// extra headers
#include "load_texture.h"
// my headers
#include "game_state_enum.h"

#ifdef _WIN32
// only on windows platforms
#include <platform/d3d11/input/keyboard_d3d11.h>
#include <platform/d3d11/input/touch_input_manager_d3d11.h>
#endif 

Frontend::Frontend(gef::Platform& platform, GAMESTATE* gamestate) :
	platform_(platform),
	gamestate_(gamestate),
	input_manager_(nullptr),
	sprite_renderer_(nullptr),
	audio_manager_(nullptr),
	title_texture_(nullptr),
	instructions_texture_1(nullptr),
	instructions_texture_2(nullptr),
	instructions_texture_3(nullptr),
	instructions_texture_4(nullptr),
	instructions_texture_5(nullptr),
	cross_button_icon_(nullptr),
	playstation_left_d_pad_(nullptr),
	playstation_right_d_pad_(nullptr),
	display_instrucitons_(nullptr),
	quit_(false),
	instructions_page_(0),
	sprite_width_(190.0f),
	sprite_height(38.0f),
	sfx_voice_id_(-1),
	sfx_id_(-1),
	active_touch_id_(-1),
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
		// if a touch is active lets draw some text
		if (active_touch_id_ != -1)
		{
			font_->RenderText(
				sprite_renderer_,
				gef::Vector4(touch_position_.x, touch_position_.y, -0.9f),
				1.0f, 
				0xffffffff, 
				gef::TJ_LEFT,
				"(%.1f, %.1f)",
				touch_position_.x, 
				touch_position_.y);
		}

		// display frame rate
		font_->RenderText(
			sprite_renderer_, 
			gef::Vector4(850.0f, 510.0f, -0.9f), 
			1.0f, 
			0xffffffff, 
			gef::TJ_LEFT, 
			"FPS: %.1f", 
			fps_);

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

	// make sure if there is a panel to detect touch input, then activate it
	if (input_manager_ && input_manager_->touch_manager() && (input_manager_->touch_manager()->max_num_panels() > 0))
		input_manager_->touch_manager()->EnablePanel(0);

	// initialise button icon
	cross_button_icon_ = CreateTextureFromPNG("playstation-cross-dark-icon.png", platform_);
	playstation_left_d_pad_ = CreateTextureFromPNG("playstation-left-d-pad.png", platform_);
	playstation_right_d_pad_ = CreateTextureFromPNG("playstation-right-d-pad.png", platform_);

	// initlalise sprite renderer
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

	// left d-pad sprite
	left_d_pad_sprite_.set_position(platform_.width() * 0.5f - sprite_width_, platform_.height() * 0.5f + sprite_height * 1.5f, 0.0f);
	left_d_pad_sprite_.set_texture(playstation_left_d_pad_);
	left_d_pad_sprite_.set_width(sprite_height);
	left_d_pad_sprite_.set_height(sprite_height);
	// right d-pad sprite			
	right_d_pad_sprite_.set_position(platform_.width() * 0.5f + sprite_width_, platform_.height() * 0.5f + sprite_height * 1.5f, 0.0f);
	right_d_pad_sprite_.set_texture(playstation_right_d_pad_);
	right_d_pad_sprite_.set_width(sprite_height);
	right_d_pad_sprite_.set_height(sprite_height);

	// sprite
	menu_box_sprite_.set_position(platform_.width() * 0.5f, platform_.height() * 0.5f + sprite_height * 1.5f, 0.0f);
	menu_box_sprite_.set_width(sprite_width_);
	menu_box_sprite_.set_height(sprite_height);

	//sprite_end_position_to_lerp_ = menu_box_sprite_.position();
	sprite_init_position_y_ = menu_box_sprite_.position().y();

	// menu text vectors init
	float height_correction = 2.0f;
	// set "START" vector
	start_text_position_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() - 0.5 * sprite_height + height_correction, -0.99f);
	// set "SETTINGS" vector
	settings_text_position_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() + 1.5 * sprite_height + height_correction, -0.99f);
	// set "INSTRUCTIONS" vector
	instructions_text_position_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() + sprite_height * 3.5f + height_correction, -0.99f);

	InitAudio();

	// initialize title texture
	title_texture_ = CreateTextureFromPNG("frontend_instructions.png", platform_);
	// initialize instructions page 1
	instructions_texture_1 = CreateTextureFromPNG("playstation-circle-dark-icon.png", platform_);
	instructions_texture_2 = CreateTextureFromPNG("playstation-cross-dark-icon.png", platform_);
	instructions_texture_3 = CreateTextureFromPNG("playstation-circle-dark-icon.png", platform_);
	instructions_texture_4 = CreateTextureFromPNG("playstation-square-dark-icon.png", platform_);
	instructions_texture_5 = CreateTextureFromPNG("playstation-triangle-dark-icon.png", platform_);

	InitFont();
} // !FrontendInit

void Frontend::FrontendRelease()
{
	delete input_manager_;
	input_manager_ = nullptr;

	delete sprite_renderer_;
	sprite_renderer_ = nullptr;

	if (audio_manager_)
	{
		audio_manager_->StopMusic();
		audio_manager_->UnloadAllSamples();
		sfx_id_ = -1;
		sfx_voice_id_ = -1;

		delete audio_manager_;
		audio_manager_ = nullptr;
	}


	delete title_texture_;
	title_texture_ = nullptr;

	delete instructions_texture_1;
	instructions_texture_1 = nullptr;

	delete instructions_texture_2;
	instructions_texture_2 = nullptr;

	delete instructions_texture_3;
	instructions_texture_3 = nullptr;

	delete instructions_texture_4;
	instructions_texture_4 = nullptr;

	delete instructions_texture_5;
	instructions_texture_5 = nullptr;

	// icon textures
	delete cross_button_icon_;
	cross_button_icon_ = nullptr;

	delete playstation_left_d_pad_;
	playstation_left_d_pad_ = nullptr;

	delete playstation_right_d_pad_;
	playstation_right_d_pad_ = nullptr;

	CleanUpFont();
} // !FrontendRelease

void Frontend::SonyController(const gef::SonyController* controller)
{
	if (controller)
	{
		// record where to move sprite
		float left_horizontal_input = controller->left_stick_x_axis();
		float left_vertical_input = controller->left_stick_y_axis();

		float right_horizontal_input = controller->right_stick_x_axis();
		float right_vertical_input = controller->right_stick_y_axis();

		//if (left_vertical_input < 0) up = true;

		// left stick up
		if (controller->buttons_pressed() & gef_SONY_CTRL_UP && 
			sprite_init_position_y_ - sprite_height <= menu_box_sprite_.position().y() - sprite_height * 2.0f)
		{
			// lerp menu box sprite
			gef::Vector4 menu_box_sprite_end_position_to_lerp;
			menu_box_sprite_end_position_to_lerp.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() - sprite_height * 2.0f, 0.0);

			gef::Vector4 menu_box_sprite_lerp;
			menu_box_sprite_.set_position(menu_box_sprite_lerp.LerpReturnVector(menu_box_sprite_.position(), menu_box_sprite_end_position_to_lerp, 1.0));

			// lerp left d-pad sprite
			gef::Vector4 left_d_pad_end_position_to_lerp;
			left_d_pad_end_position_to_lerp.set_value(platform_.width() * 0.5f - sprite_width_, left_d_pad_sprite_.position().y() - sprite_height * 2.0f, 0.0);

			gef::Vector4 left_d_pad_sprite_lerp;
			left_d_pad_sprite_.set_position(left_d_pad_sprite_lerp.LerpReturnVector(left_d_pad_sprite_.position(), left_d_pad_end_position_to_lerp, 1.0));

			// right right d-pad sprite
			gef::Vector4 right_d_pad_end_position_to_lerp;
			right_d_pad_end_position_to_lerp.set_value(platform_.width() * 0.5f + sprite_width_, right_d_pad_sprite_.position().y() - sprite_height * 2.0f, 0.0);

			gef::Vector4 right_d_pad_sprite_lerp;
			right_d_pad_sprite_.set_position(right_d_pad_sprite_lerp.LerpReturnVector(right_d_pad_sprite_.position(), right_d_pad_end_position_to_lerp, 1.0));

		}

		// left stick down
		if (controller->buttons_pressed() & gef_SONY_CTRL_DOWN &&
			sprite_init_position_y_ + sprite_height * 4.0f >= menu_box_sprite_.position().y() + sprite_height * 2.0f)
		{
			// lerp menu box sprite
			gef::Vector4 menu_box_sprite_end_position_to_lerp;
			menu_box_sprite_end_position_to_lerp.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() + sprite_height * 2.0f, 0.0);

			gef::Vector4 menu_box_sprite_lerp;
			menu_box_sprite_.set_position(menu_box_sprite_lerp.LerpReturnVector(menu_box_sprite_.position(), menu_box_sprite_end_position_to_lerp, 1.0));

			// lerp left d-pad sprite
			gef::Vector4 left_d_pad_end_position_to_lerp;
			left_d_pad_end_position_to_lerp.set_value(platform_.width() * 0.5f - sprite_width_, left_d_pad_sprite_.position().y() + sprite_height * 2.0f, 0.0);

			gef::Vector4 left_d_pad_sprite_lerp;
			left_d_pad_sprite_.set_position(left_d_pad_sprite_lerp.LerpReturnVector(left_d_pad_sprite_.position(), left_d_pad_end_position_to_lerp, 1.0));

			// right right d-pad sprite
			gef::Vector4 right_d_pad_end_position_to_lerp;
			right_d_pad_end_position_to_lerp.set_value(platform_.width() * 0.5f + sprite_width_, right_d_pad_sprite_.position().y() + sprite_height * 2.0f, 0.0);

			gef::Vector4 right_d_pad_sprite_lerp;
			right_d_pad_sprite_.set_position(right_d_pad_sprite_lerp.LerpReturnVector(right_d_pad_sprite_.position(), right_d_pad_end_position_to_lerp, 1.0));
		}

		// START press
		if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
			menu_box_sprite_.position().y() > (start_text_position_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (start_text_position_.y() + sprite_height))
		{
			// update the current state of the game state machine
			// get the value that the gamestate points to and change it
			(*gamestate_) = GAME; 
		}
		// SETTINGS press
		if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
			menu_box_sprite_.position().y() > (settings_text_position_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (settings_text_position_.y() + sprite_height))
		{
			// update the current state of the game state machine
			// get the value that the gamestate points to and change it
			(*gamestate_) = SETTINGS; 
		}
		// INSTRUCTIONS press
		if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
			menu_box_sprite_.position().y() > (instructions_text_position_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (instructions_text_position_.y() + sprite_height))
		{
			instructions_page_++;

			if (instructions_page_ <= 4)
				instructions_page_ = 0;
		}
		// INSTRUCTIONS left d-pad
		if (controller->buttons_pressed() & gef_SONY_CTRL_LEFT &&
			menu_box_sprite_.position().y() > (instructions_text_position_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (instructions_text_position_.y() + sprite_height))
		{
			if (instructions_page_ > 0)
				instructions_page_--;
		}
		// INSTRUCTIONS right d-pad
		if (controller->buttons_pressed() & gef_SONY_CTRL_RIGHT &&
			menu_box_sprite_.position().y() > (instructions_text_position_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (instructions_text_position_.y() + sprite_height))
		{
			if (instructions_page_ < 4)
				instructions_page_++;
		}
		// hide d-pad
		if (menu_box_sprite_.position().y() > (instructions_text_position_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (instructions_text_position_.y() + sprite_height))
		{
			display_d_pad = true;
			display_instrucitons_ = true;
		}
		else
		{
			display_d_pad = false;
			display_instrucitons_ = false;
		}
		// TODO delete
		if (controller->buttons_pressed() & gef_SONY_CTRL_SELECT)
		{
			quit_ = true;
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
		//if (keyboard->IsKeyDown(gef::Keyboard::KC_X))
		//{
			// update the current state for the game state machine
			// get the value that the gamestate points to and change it
		//	(*gamestate_) = GAME; 
		//}
			
	} // keyboard

	// mouse input
	//const gef::TouchInputManager* touch_input = input_manager_->touch_manager();
	//if (touch_input)
	//{
	//	// initialise the mouse position
	//	gef::Vector2 mouse_position(0.0f, 0.0f); // left upper corner of the window

	//											 // get a pointer to the d3d11 implementation of the TouchInputManager
	//	const gef::TouchInputManagerD3D11* touch_input_d3d11 = (const gef::TouchInputManagerD3D11*)touch_input;

	//	// get the mouse position
	//	mouse_position = touch_input_d3d11->mouse_position();

	//	if (touch_input_d3d11->is_button_down(0))
	//	{
	//		//SetCursorPos(480, 272);	
	//	}

	//	//gef::DebugOut("Mouse position x, y: %f %f\n", mouse_position.x, mouse_position.y);
	//} // touch_input (mouse)
}
#endif // !_WIN32

void Frontend::ProcessTouchInput()
{
	const gef::TouchInputManager* touch_input = input_manager_->touch_manager();
	if (touch_input && (touch_input->max_num_panels() > 0))
	{
		// get the active touches for this panel
		const gef::TouchContainer& panel_touches = touch_input->touches(0);

		// go through the touches
		for (gef::ConstTouchIterator touch = panel_touches.begin(); touch != panel_touches.end(); ++touch)
		{
			// if active touch id is -1, then we are not currently processing a touch
			if (active_touch_id_ == -1)
			{
				// check for the start of a new touch
				if (touch->type == gef::TT_NEW)
				{
					active_touch_id_ = touch->id;

					// we're just going to record the position of the touch
					touch_position_ = touch->position;

					// do any processing for a new touch here

					// record where to move sprite
					//sprite_end_position_to_lerp_.set_value(touch_position_.x, touch_position_.y, 0.0);
					// change colour
					menu_box_sprite_.set_colour(gef::Colour(1.0f, 0.0f, 0.0f).GetABGR());
				}
			}
			else if (active_touch_id_ == touch->id)
			{
				// we are processing touch data with a matching id to the one we are looking for
				if (touch->type == gef::TT_ACTIVE)
				{
					// update an active touch here
					//gef::DebugOut("Touch position: %f\n", touch->position);
					//gef::DebugOut("Touch id: %f\n", touch->id);

					//sprite_.set_position(touch_position_.x, touch_position_.y);

					// we're just going to record the position of the touch
					touch_position_ = touch->position;

					//// update variable
					//sprite_end_position_to_lerp_.set_value(touch_position_.x, touch_position_.y, 0.0);
				}
				else if (touch->type == gef::TT_RELEASED)
				{
					// the touch we are tracking has been released
					// perform any actions that need to happen when a touch is released here

					// change colour
					menu_box_sprite_.set_colour(gef::Colour(0.0f, 1.0f, 0.0f).GetABGR());

					// we're not doing anything here apart from resetting the active touch id
					active_touch_id_ = -1;
				}
			}
		}
	}
}

void Frontend::FrontendUpdate(float frame_time)
{
	fps_ = 1.0f / frame_time;

	// get the latest date from the input devices
	if (input_manager_)
	{
		input_manager_->Update();

		const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);

		SonyController(controller);

		//ProcessTouchInput();
#ifdef _WIN32
		//KeyboardController(frame_time);
#endif // _WIN32
	} // !input_manager_

	 // new version using LerpReturnVector function which returns a vector
	//sprite_.set_position(sprite_lerp_.LerpReturnVector(sprite_.position(), sprite_position_to_lerp_end_, 1.0));
} // !FrontendUpdate

void Frontend::FrontendRender()
{
	sprite_renderer_->Begin();
	{
		// render "START" text
		font_->RenderText(
			sprite_renderer_,
			gef::Vector4(start_text_position_.x(), start_text_position_.y(), -0.99f),
			1.0f,
			0xffffffff,
			gef::TJ_CENTRE,
			"START");

		// render "SETTINGS" text
		font_->RenderText(
			sprite_renderer_,
			gef::Vector4(settings_text_position_.x(), settings_text_position_.y(), -0.99f),
			1.0f,
			0xffffffff,
			gef::TJ_CENTRE,
			"SETTINGS");

		// render "INSTRUCTIONS" text
		font_->RenderText(
			sprite_renderer_,
			gef::Vector4(instructions_text_position_.x(), instructions_text_position_.y(), -0.99f),
			1.0f,
			0xffffffff,
			gef::TJ_CENTRE,
			"INSTRUCTIONS");

		// Render title picture
		if (display_instrucitons_)
		{
			switch (instructions_page_)
			{ 
			case 0 :
			{
				gef::Sprite instructions_page_1;
				instructions_page_1.set_texture(instructions_texture_1);
				instructions_page_1.set_position(gef::Vector4(start_text_position_.x(), start_text_position_.y() - sprite_height * 4.0f, -0.99f));
				instructions_page_1.set_height(platform_.height() * 0.5f);
				instructions_page_1.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(instructions_page_1);
			} //
			break;

			case 1 :
			{
				gef::Sprite instructions_page_2;
				instructions_page_2.set_texture(instructions_texture_2);
				instructions_page_2.set_position(gef::Vector4(start_text_position_.x(), start_text_position_.y() - sprite_height * 4.0f, -0.99f));
				instructions_page_2.set_height(platform_.height() * 0.5f);
				instructions_page_2.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(instructions_page_2);
			} //
			break;

			case 2 :
			{
				gef::Sprite instructions_page_3;
				instructions_page_3.set_texture(instructions_texture_3);
				instructions_page_3.set_position(gef::Vector4(start_text_position_.x(), start_text_position_.y() - sprite_height * 4.0f, -0.99f));
				instructions_page_3.set_height(platform_.height() * 0.5f);
				instructions_page_3.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(instructions_page_3);
			} //
			break;

			case 3 :
			{
				gef::Sprite instructions_page_4;
				instructions_page_4.set_texture(instructions_texture_4);
				instructions_page_4.set_position(gef::Vector4(start_text_position_.x(), start_text_position_.y() - sprite_height * 4.0f, -0.99f));
				instructions_page_4.set_height(platform_.height() * 0.5f);
				instructions_page_4.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(instructions_page_4);
			} //
			break;

			case 4 :
			{
				gef::Sprite instructions_page_5;
				instructions_page_5.set_texture(instructions_texture_5);
				instructions_page_5.set_position(gef::Vector4(start_text_position_.x(), start_text_position_.y() - sprite_height * 4.0f, -0.99f));
				instructions_page_5.set_height(platform_.height() * 0.5f);
				instructions_page_5.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(instructions_page_5);
			} //
			break;
			} // !display_instructions_
		}
		else
		{
			gef::Sprite title;
			title.set_texture(title_texture_);
			title.set_position(gef::Vector4(start_text_position_.x(), start_text_position_.y() - sprite_height * 4.0f, -0.99f));
			title.set_height(platform_.height() * 0.5f);
			title.set_width(platform_.width() * 0.5f);
			sprite_renderer_->DrawSprite(title);
		}

		// render menu box sprite
		sprite_renderer_->DrawSprite(menu_box_sprite_);

		// render d-pad sprites
		if (display_d_pad)
		{
			sprite_renderer_->DrawSprite(left_d_pad_sprite_);
			sprite_renderer_->DrawSprite(right_d_pad_sprite_);
		}

		DrawHUD();
	}
	sprite_renderer_->End();
	/*gef::DebugOut("sprite_.position().y(): %f\n", sprite_.position().y());
	gef::DebugOut("start_text_position_.y() - sprite_height * 0.5f: %f\n", sprite_.position().y() - sprite_height * 0.5f);
	gef::DebugOut("start_text_position_.y() + sprite_height: %f\n", sprite_.position().y() + sprite_height);*/
} // !FrontendRender
