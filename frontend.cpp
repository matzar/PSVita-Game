#include "frontend.h"
// gef headers
#include <system/platform.h>
#include <system/debug_log.h>

#include <graphics/font.h>

#include <graphics/sprite_renderer.h>

#include <input/input_manager.h>
#include <input/touch_input_manager.h>
#include <input/sony_controller_input_manager.h>

#include <maths/vector2.h>
#include <maths/vector4.h>
// extra headers
#include "load_texture.h"
// my headers
#include "game_state_enum.h"

Frontend::Frontend(gef::Platform& platform, gef::InputManager* input_manager, GAMESTATE* gamestate) :
	platform_(platform),
	gamestate_(gamestate),
	input_manager_(input_manager),
	sprite_renderer_(nullptr),
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
	dev_(false),
	display_d_pad(true),
	instructions_page_(0),
	sprite_width_(190.0f),
	sprite_height(38.0f),
	sfx_voice_id_(-1),
	fps_(0),
	active_touch_id_(-1)
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

void Frontend::CleanFont()
{
	delete font_;
	font_ = nullptr;
} // CleanFont

void Frontend::DrawHUD()
{
	if (font_)
	{
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

void Frontend::InitTextures()
{
	// initialise button icon
	cross_button_icon_ = CreateTextureFromPNG("png/playstation-cross-dark-icon.png", platform_);
	playstation_left_d_pad_ = CreateTextureFromPNG("png/playstation-left-d-pad.png", platform_);
	playstation_right_d_pad_ = CreateTextureFromPNG("png/playstation-right-d-pad.png", platform_);
	// initialize title texture
	title_texture_ = CreateTextureFromPNG("png/frontend_instructions.png", platform_);
	// initialize instructions page 1
	instructions_texture_1 = CreateTextureFromPNG("png/playstation-circle-dark-icon.png", platform_);
	instructions_texture_2 = CreateTextureFromPNG("png/playstation-cross-dark-icon.png", platform_);
	instructions_texture_3 = CreateTextureFromPNG("png/playstation-circle-dark-icon.png", platform_);
	instructions_texture_4 = CreateTextureFromPNG("png/playstation-square-dark-icon.png", platform_);
	instructions_texture_5 = CreateTextureFromPNG("png/playstation-triangle-dark-icon.png", platform_);
} // !InitTextures

void Frontend::CleanTextures()
{
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
} // !CleanTextures

void Frontend::InitSprites()
{
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
} // !InitSprites

void Frontend::CleanSprites()
{
	delete sprite_renderer_;
	sprite_renderer_ = nullptr;
} // !CleanSprites

void Frontend::InitText()
{
	// menu text vectors init
	float height_correction = 3.0f;
	// set "START" vector
	menu_text_1_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() - 0.5 * sprite_height + height_correction, -0.99f);
	// set "SETTINGS" vector
	menu_text_2_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() + 1.0 * sprite_height + height_correction, -0.99f);
	// set "BACK" vector
	menu_text_3_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() + sprite_height * 2.5f + height_correction, -0.99f);
	// set "BACK" vector
	menu_text_4_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() + sprite_height * 4.0f + height_correction, -0.99f);
} // InitText()

void Frontend::FrontendInit()
{
	// initialise input manager
	//input_manager_ = gef::InputManager::Create(platform_);

	InitFont();

	InitTextures();

	InitSprites();

	InitText();
} // !FrontendInit

void Frontend::FrontendRelease()
{
	CleanSprites();

	CleanTextures();

	CleanFont();
} // !FrontendRelease

void Frontend::MenuTouchInput()
{
	// GAME press
	if (touch_position_.y > (menu_text_1_.y() - sprite_height * 0.5f) &&
		touch_position_.y < (menu_text_1_.y() + sprite_height) &&
		touch_position_.x > (platform_.width() / 2 - sprite_width_ / 2) &&
		touch_position_.x < (platform_.width() / 2 + sprite_width_ / 2))
	{
		// update the current state of the game state machine
		// get the value that the gamestate points to and change it
		(*gamestate_) = GAME;
	}

	// SETTINGS press
	if (touch_position_.y > (menu_text_2_.y() - sprite_height * 0.5f) &&
		touch_position_.y < (menu_text_2_.y() + sprite_height) &&
		touch_position_.x > (platform_.width() / 2 - sprite_width_ / 2) &&
		touch_position_.x < (platform_.width() / 2 + sprite_width_ / 2))
	{
		// update the current state of the game state machine
		// get the value that the gamestate points to and change it
		(*gamestate_) = SETTINGS;
	}

	// INSTRUCTIONS press
	if (touch_position_.y > (menu_text_3_.y() - sprite_height * 0.5f) &&
		touch_position_.y < (menu_text_3_.y() + sprite_height) &&
		touch_position_.x > (platform_.width() / 2 - sprite_width_ / 2) &&
		touch_position_.x < (platform_.width() / 2 + sprite_width_ / 2))
	{
		if (!display_d_pad) // prevent from incrementing 'instructions_page_' before displaying the first page
		{
			// move down menu box sprite
			menu_box_sprite_.set_position(
				platform_.width() / 2.0f,
				menu_text_3_.y() + sprite_height * 0.5f - 3.0f,
				0.0f);

			// move down left d-pad sprite
			left_d_pad_sprite_.set_position(
				left_d_pad_sprite_.position().x(),
				menu_text_3_.y() + sprite_height * 0.5f - 3.0f,
				0.0f);


			// lerp down right d-pad sprite
			right_d_pad_sprite_.set_position(
				right_d_pad_sprite_.position().x(),
				menu_text_3_.y() + sprite_height * 0.5f - 3.0f,
				0.0f);
		}
		else
		{
			instructions_page_++;

			if (instructions_page_ >= 5)
				instructions_page_ = 0;
		}
	}

	// QUIT press
	if (touch_position_.y > (menu_text_4_.y() - sprite_height * 0.5f) &&
		touch_position_.y < (menu_text_4_.y() + sprite_height) &&
		touch_position_.x > (platform_.width() / 2 - sprite_width_ / 2) &&
		touch_position_.x < (platform_.width() / 2 + sprite_width_ / 2))
	{
		quit_ = true;
	}
}

void Frontend::TouchController(const gef::TouchInputManager* touch_input)
{
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

					MenuTouchInput();
				}
			}
			else if (active_touch_id_ == touch->id)
			{
				// we are processing touch data with a matching id to the one we are looking for
				if (touch->type == gef::TT_ACTIVE)
				{
					// update an active touch here
					// do any processing for a new touch here


					// we're just going to record the position of the touch
					touch_position_ = touch->position;

				}
				else if (touch->type == gef::TT_RELEASED)
				{
					// the touch we are tracking has been released
					// perform any actions that need to happen when a touch is released here

					// we're not doing anything here apart from resetting the active touch id
					active_touch_id_ = -1;
				}
			}
		}
	}
} // !TouchController

void Frontend::SonyController(const gef::SonyController* controller)
{
	if (controller)
	{
		// D-pad up
		if (controller->buttons_pressed() & gef_SONY_CTRL_UP &&
			sprite_init_position_y_ - sprite_height <= menu_box_sprite_.position().y() - sprite_height * 1.5f)
		{
			// move down menu box sprite
			menu_box_sprite_.set_position(
				menu_box_sprite_.position().x(),
				menu_box_sprite_.position().y() - sprite_height * 1.5f,
				0.0f);

			// move down left d-pad sprite
			left_d_pad_sprite_.set_position(
				left_d_pad_sprite_.position().x(),
				left_d_pad_sprite_.position().y() - sprite_height * 1.5f,
				0.0f);


			// lerp down right d-pad sprite
			right_d_pad_sprite_.set_position(
				right_d_pad_sprite_.position().x(),
				right_d_pad_sprite_.position().y() - sprite_height * 1.5f,
				0.0f);
		}

		// D-pad down
		if (controller->buttons_pressed() & gef_SONY_CTRL_DOWN &&
			sprite_init_position_y_ + sprite_height * 5.0f >= menu_box_sprite_.position().y() + sprite_height * 1.5f)
		{
			// move down menu box sprite
			menu_box_sprite_.set_position(
				menu_box_sprite_.position().x(), 
				menu_box_sprite_.position().y() + sprite_height * 1.5f, 
				0.0f);

			// move down left d-pad sprite
			left_d_pad_sprite_.set_position(
				left_d_pad_sprite_.position().x(), 
				left_d_pad_sprite_.position().y() + sprite_height * 1.5f, 
				0.0f);


			// lerp down right d-pad sprite
			right_d_pad_sprite_.set_position(
				right_d_pad_sprite_.position().x(), 
				right_d_pad_sprite_.position().y() + sprite_height * 1.5f, 
				0.0f);
		}

		// START press
		if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
			menu_box_sprite_.position().y() > (menu_text_1_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (menu_text_1_.y() + sprite_height))
		{
			// update the current state of the game state machine
			// get the value that the gamestate points to and change it
			(*gamestate_) = GAME; 
		}
		// SETTINGS press
		if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
			menu_box_sprite_.position().y() > (menu_text_2_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (menu_text_2_.y() + sprite_height))
		{
			// update the current state of the game state machine
			// get the value that the gamestate points to and change it
			(*gamestate_) = SETTINGS; 
		}
		// INSTRUCTIONS press
		if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
			menu_box_sprite_.position().y() > (menu_text_3_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (menu_text_3_.y() + sprite_height))
		{
			instructions_page_++;

			if (instructions_page_ >= 5)
				instructions_page_ = 0;
		}
		// INSTRUCTIONS left d-pad
		if (controller->buttons_pressed() & gef_SONY_CTRL_LEFT &&
			menu_box_sprite_.position().y() > (menu_text_3_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (menu_text_3_.y() + sprite_height))
		{
			if (instructions_page_ > 0)
				instructions_page_--;
		}
		// INSTRUCTIONS right d-pad
		if (controller->buttons_pressed() & gef_SONY_CTRL_RIGHT &&
			menu_box_sprite_.position().y() > (menu_text_3_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (menu_text_3_.y() + sprite_height))
		{
			if (instructions_page_ < 4)
				instructions_page_++;
		}
		// QUIT press
		if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
			menu_box_sprite_.position().y() > (menu_text_4_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (menu_text_4_.y() + sprite_height))
		{

			quit_ = true;
		}
		// toggle fps display
		if (controller->buttons_pressed() & gef_SONY_CTRL_CIRCLE)
		{
			dev_ = !dev_;
		}
		// toggle d-pad and instrucitons display
		if (menu_box_sprite_.position().y() > (menu_text_3_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (menu_text_3_.y() + sprite_height))
		{
			display_d_pad = true;
			display_instrucitons_ = true;
		}
		else
		{
			display_d_pad = false;
			display_instrucitons_ = false;
		}
	} 
} // !SonyController

void Frontend::FrontendUpdate(float frame_time)
{
	fps_ = 1.0f / frame_time;
	
	// get the latest date from the input devices
	if (input_manager_)
	{
		input_manager_->Update();

		const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);
		SonyController(controller);

		// get touch input
		const gef::TouchInputManager* touch_input = input_manager_->touch_manager();
		TouchController(touch_input);
	} // !input_manager_
} // !FrontendUpdate

void Frontend::FrontendRender()
{
	sprite_renderer_->Begin();
	{
		// render "START" text
		font_->RenderText(
			sprite_renderer_,
			gef::Vector4(menu_text_1_.x(), menu_text_1_.y(), -0.99f),
			1.0f,
			0xffffffff,
			gef::TJ_CENTRE,
			"START");

		// render "SETTINGS" text
		font_->RenderText(
			sprite_renderer_,
			gef::Vector4(menu_text_2_.x(), menu_text_2_.y(), -0.99f),
			1.0f,
			0xffffffff,
			gef::TJ_CENTRE,
			"SETTINGS");

		// render "INSTRUCTIONS" text
		font_->RenderText(
			sprite_renderer_,
			gef::Vector4(menu_text_3_.x(), menu_text_3_.y(), -0.99f),
			1.0f,
			0xffffffff,
			gef::TJ_CENTRE,
			"INSTRUCTIONS");

		// render "QUIT" text
		font_->RenderText(
			sprite_renderer_,
			gef::Vector4(menu_text_4_.x(), menu_text_4_.y(), -0.99f),
			1.0f,
			0xffffffff,
			gef::TJ_CENTRE,
			"QUIT");

		// Render title picture
		if (display_instrucitons_)
		{
			switch (instructions_page_)
			{ 
			case 0 :
			{
				gef::Sprite instructions_page_1;
				instructions_page_1.set_texture(instructions_texture_1);
				instructions_page_1.set_position(gef::Vector4(menu_text_1_.x(), menu_text_1_.y() - sprite_height * 4.0f, -0.99f));
				instructions_page_1.set_height(platform_.height() * 0.5f);
				instructions_page_1.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(instructions_page_1);
			} //
			break;

			case 1 :
			{
				gef::Sprite instructions_page_2;
				instructions_page_2.set_texture(instructions_texture_2);
				instructions_page_2.set_position(gef::Vector4(menu_text_1_.x(), menu_text_1_.y() - sprite_height * 4.0f, -0.99f));
				instructions_page_2.set_height(platform_.height() * 0.5f);
				instructions_page_2.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(instructions_page_2);
			} //
			break;

			case 2 :
			{
				gef::Sprite instructions_page_3;
				instructions_page_3.set_texture(instructions_texture_3);
				instructions_page_3.set_position(gef::Vector4(menu_text_1_.x(), menu_text_1_.y() - sprite_height * 4.0f, -0.99f));
				instructions_page_3.set_height(platform_.height() * 0.5f);
				instructions_page_3.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(instructions_page_3);
			} //
			break;

			case 3 :
			{
				gef::Sprite instructions_page_4;
				instructions_page_4.set_texture(instructions_texture_4);
				instructions_page_4.set_position(gef::Vector4(menu_text_1_.x(), menu_text_1_.y() - sprite_height * 4.0f, -0.99f));
				instructions_page_4.set_height(platform_.height() * 0.5f);
				instructions_page_4.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(instructions_page_4);
			} //
			break;

			case 4 :
			{
				gef::Sprite instructions_page_5;
				instructions_page_5.set_texture(instructions_texture_5);
				instructions_page_5.set_position(gef::Vector4(menu_text_1_.x(), menu_text_1_.y() - sprite_height * 4.0f, -0.99f));
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
			title.set_position(gef::Vector4(menu_text_1_.x(), menu_text_1_.y() - sprite_height * 4.0f, -0.99f));
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
		// display fps if dev mode on
		if (dev_)
		{
			DrawHUD();
		}
	}
	sprite_renderer_->End();
} // !FrontendRender
