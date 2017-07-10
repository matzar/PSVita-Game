#include "settings.h"
// gef headers
#include <system/platform.h>

#include <graphics/font.h>

#include <graphics/sprite_renderer.h>
#include <graphics/sprite.h>

#include <input/input_manager.h>
#include <input/touch_input_manager.h>
#include <input/sony_controller_input_manager.h>

#include <maths/vector2.h>
#include <maths/vector4.h>
// extra headers
#include "load_texture.h"
// my headers
#include "game_state_enum.h"
#include "camera_enum.h"
#include "difficulty_enum.h"

Settings::Settings(gef::Platform& platform, gef::InputManager* input_manager, GAMESTATE* gamestate, unsigned* camera_count, unsigned* difficulty_count, unsigned* number_of_grounds) :
	platform_(platform),
	input_manager_(input_manager),
	gamestate_(gamestate),
	camera_count_(camera_count),
	difficulty_count_(difficulty_count),
	number_of_grounds_(number_of_grounds),
	sprite_renderer_(nullptr),
	cross_button_icon_(nullptr),
	playstation_left_d_pad_(nullptr),
	playstation_right_d_pad_(nullptr),
	quit_(false),
	dev_(false),
	sprite_width_(190.0f),
	sprite_height(38.0f),
	sfx_voice_id_(-1),
	sfx_id_(-1),
	fps_(0),
	active_touch_id_(-1)
{
}

Settings::~Settings()
{
}

void Settings::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
} // !InitFont

void Settings::CleanFont()
{
	delete font_;
	font_ = nullptr;
} // CleanFont

void Settings::DrawHUD()
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

void Settings::InitTextures()
{
	// initialise button icon
	cross_button_icon_ = CreateTextureFromPNG("png/playstation-cross-dark-icon.png", platform_);
	// initialise left d-pad icon
	playstation_left_d_pad_ = CreateTextureFromPNG("png/playstation-left-d-pad.png", platform_);
	// initialise right d-pad icon
	playstation_right_d_pad_ = CreateTextureFromPNG("png/playstation-right-d-pad.png", platform_);

	// initialise camera's 1 texture to show its perspective
	camera_1_texture_ = CreateTextureFromPNG("png/CAM1.png", platform_);
	// initialise camera's 2 texture to show its perspective
	camera_2_texture_ = CreateTextureFromPNG("png/CAM2.png", platform_);
	// initialise camera's 3 texture to show its perspective
	camera_3_texture_ = CreateTextureFromPNG("png/CAM3.png", platform_);
} // !InitTextures

void Settings::CleanTextures()
{
	// icon textures
	delete cross_button_icon_;
	cross_button_icon_ = nullptr;

	delete playstation_left_d_pad_;
	playstation_left_d_pad_ = nullptr;

	delete playstation_right_d_pad_;
	playstation_right_d_pad_ = nullptr;

	// camera illustrations textures
	delete camera_1_texture_;
	camera_1_texture_ = nullptr;

	delete camera_2_texture_;
	camera_2_texture_ = nullptr;

	delete camera_3_texture_;
	camera_3_texture_ = nullptr;
} // !CleanTextures

void Settings::InitSprites()
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
	// menu box sprite
	menu_box_sprite_.set_position(platform_.width() * 0.5f, platform_.height() * 0.5f + sprite_height * 1.5f, 0.0f);
	menu_box_sprite_.set_width(sprite_width_);
	menu_box_sprite_.set_height(sprite_height);

	sprite_init_position_y_ = menu_box_sprite_.position().y();
} // !InitSprites

void Settings::CleanSprites()
{
	delete sprite_renderer_;
	sprite_renderer_ = nullptr;
} // !CleanSprites

void Settings::InitText()
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

void Settings::SettingsInit()
{
	InitFont();

	InitTextures();

	InitSprites();
	
	InitText();
} // !SettingsInit

void Settings::SettingsRelease()
{
	CleanFont();

	CleanTextures();

	CleanSprites();
} // !SettingsRelease

void Settings::MenuTouchInput()
{
	// CAMERA press
	if (touch_position_.y > (menu_text_1_.y() - sprite_height * 0.5f) &&
		touch_position_.y < (menu_text_1_.y() + sprite_height) &&
		touch_position_.x > (platform_.width() / 2 - sprite_width_ / 2) &&
		touch_position_.x < (platform_.width() / 2 + sprite_width_ / 2))
	{
		(*camera_count_)++;

		if ((*camera_count_) >= 3)
			(*camera_count_) = 0;

		// move down menu box sprite
		menu_box_sprite_.set_position(
			platform_.width() / 2.0f,
			menu_text_1_.y() + sprite_height * 0.5f - 3.0f,
			0.0f);

		// move down left d-pad sprite
		left_d_pad_sprite_.set_position(
			left_d_pad_sprite_.position().x(),
			menu_text_1_.y() + sprite_height * 0.5f - 3.0f,
			0.0f);


		// lerp down right d-pad sprite
		right_d_pad_sprite_.set_position(
			right_d_pad_sprite_.position().x(),
			menu_text_1_.y() + sprite_height * 0.5f - 3.0f,
			0.0f);
	}

	// DIFFICULTY press
	if (touch_position_.y > (menu_text_2_.y() - sprite_height * 0.5f) &&
		touch_position_.y < (menu_text_2_.y() + sprite_height) &&
		touch_position_.x > (platform_.width() / 2 - sprite_width_ / 2) &&
		touch_position_.x < (platform_.width() / 2 + sprite_width_ / 2))
	{
		(*difficulty_count_)++;

		if ((*difficulty_count_) >= 2)
			(*difficulty_count_) = 0;

		// move down menu box sprite
		menu_box_sprite_.set_position(
			platform_.width() / 2.0f,
			menu_text_2_.y() + sprite_height * 0.5f - 3.0f,
			0.0f);

		// move down left d-pad sprite
		left_d_pad_sprite_.set_position(
			left_d_pad_sprite_.position().x(),
			menu_text_2_.y() + sprite_height * 0.5f - 3.0f,
			0.0f);


		// lerp down right d-pad sprite
		right_d_pad_sprite_.set_position(
			right_d_pad_sprite_.position().x(),
			menu_text_2_.y() + sprite_height * 0.5f - 3.0f,
			0.0f);
	}

	// GROUNDS press
	if (touch_position_.y > (menu_text_3_.y() - sprite_height * 0.5f) &&
		touch_position_.y < (menu_text_3_.y() + sprite_height) &&
		touch_position_.x > (platform_.width() / 2 - sprite_width_ / 2) &&
		touch_position_.x < (platform_.width() / 2 + sprite_width_ / 2))
	{
		(*number_of_grounds_) += 10;

		if ((*number_of_grounds_) >= 40)
			(*number_of_grounds_) = 10;

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

	// BACK press
	if (touch_position_.y > (menu_text_4_.y() - sprite_height * 0.5f) &&
		touch_position_.y < (menu_text_4_.y() + sprite_height) &&
		touch_position_.x > (platform_.width() / 2 - sprite_width_ / 2) &&
		touch_position_.x < (platform_.width() / 2 + sprite_width_ / 2))
	{
		// update the current state of the game state machine
		// get the value that the gamestate points to and change it
		(*gamestate_) = FRONTEND;
	}
}

void Settings::TouchController(const gef::TouchInputManager* touch_input)
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
	// reset active touch (it doesn't get fully reset in RELEASE)
	active_touch_id_ = -1;
} // !TouchController

void Settings::SonyController(const gef::SonyController* controller)
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

		// CAMERA CROSS press
		if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
			menu_box_sprite_.position().y() > (menu_text_1_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (menu_text_1_.y() + sprite_height))
		{
			(*camera_count_)++;

			if ((*camera_count_) >= 3)
				(*camera_count_) = 0;
		}
		// CAMERA - D-pad left
		if (controller->buttons_pressed() & gef_SONY_CTRL_LEFT &&
			menu_box_sprite_.position().y() > (menu_text_1_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (menu_text_1_.y() + sprite_height))
		{
			if ((*camera_count_) > 0)
				(*camera_count_)--;
		}
		// CAMERA - D-pad right
		if (controller->buttons_pressed() & gef_SONY_CTRL_RIGHT &&
			menu_box_sprite_.position().y() > (menu_text_1_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (menu_text_1_.y() + sprite_height))
		{

			if ((*camera_count_) < 2)
				(*camera_count_)++;
		}
		// DIFFICULTY press
		if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
			menu_box_sprite_.position().y() > (menu_text_2_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (menu_text_2_.y() + sprite_height))
		{
			(*difficulty_count_)++;

			if ((*difficulty_count_) >= 2)
				(*difficulty_count_) = 0;
		}
		// DIFFICULTY left d-pad
		if (controller->buttons_pressed() & gef_SONY_CTRL_LEFT &&
			menu_box_sprite_.position().y() > (menu_text_2_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (menu_text_2_.y() + sprite_height))
		{
			if ((*difficulty_count_) > 0)
				(*difficulty_count_)--;
		}
		// DIFFICULTY right d-pad
		if (controller->buttons_pressed() & gef_SONY_CTRL_RIGHT &&
			menu_box_sprite_.position().y() > (menu_text_2_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (menu_text_2_.y() + sprite_height))
		{
			if ((*difficulty_count_) < 1)
				(*difficulty_count_)++;
		}

		// GROUND press
		if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
			menu_box_sprite_.position().y() > (menu_text_3_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (menu_text_3_.y() + sprite_height))
		{
			(*number_of_grounds_) += 10;

			if ((*number_of_grounds_) >= 40)
				(*number_of_grounds_) = 10;
		}
		// GROUND left d-pad
		if (controller->buttons_pressed() & gef_SONY_CTRL_LEFT &&
			menu_box_sprite_.position().y() > (menu_text_3_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (menu_text_3_.y() + sprite_height))
		{
			if ((*number_of_grounds_) > 10)
				(*number_of_grounds_) -= 10;
		}
		// GROUND right d-pad
		if (controller->buttons_pressed() & gef_SONY_CTRL_RIGHT &&
			menu_box_sprite_.position().y() > (menu_text_3_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (menu_text_3_.y() + sprite_height))
		{
			if ((*number_of_grounds_) < 30)
				(*number_of_grounds_) += 10;
		}
		
		// BACK press
		if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
			menu_box_sprite_.position().y() > (menu_text_4_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (menu_text_4_.y() + sprite_height))
		{
			// update the current state of the game state machine
			// get the value that the gamestate points to and change it
			(*gamestate_) = FRONTEND; 
		}
		// toggle fps display
		if (controller->buttons_pressed() & gef_SONY_CTRL_CIRCLE)
		{
			dev_ = !dev_;
		}
		// d-pad display
		if (menu_box_sprite_.position().y() > (menu_text_4_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (menu_text_4_.y() + sprite_height))
		{
			display_d_pad = false;
		}
		else
		{
			display_d_pad = true;
		}
	}
} // !SonyController

void Settings::SettingsUpdate(float frame_time)
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
} // !SettingsUpdate

void Settings::SettingsRender()
{
	sprite_renderer_->Begin();
	{
		switch ((*camera_count_))
		{
			case CAM1:
			{
				// render CAM1 picture
				gef::Sprite camera_1;
				camera_1.set_texture(camera_1_texture_);
				camera_1.set_position(gef::Vector4(menu_text_1_.x(), menu_text_1_.y() - sprite_height * 4.0f, -0.99f));
				camera_1.set_height(platform_.height() * 0.5f);
				camera_1.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(camera_1);

				// render "CAMERA1" text
				font_->RenderText(
					sprite_renderer_,
					gef::Vector4(menu_text_1_.x(), menu_text_1_.y(), -0.99f),
					1.0f,
					0xffffffff,
					gef::TJ_CENTRE,
					"CAMERA 1");
			} // !CAM1
			break;

			case CAM2:
			{
				// render CAM2 picture
				gef::Sprite camera_2;
				camera_2.set_texture(camera_2_texture_);
				camera_2.set_position(gef::Vector4(menu_text_1_.x(), menu_text_1_.y() - sprite_height * 4.0f, -0.99f));
				camera_2.set_height(platform_.height() * 0.5f);
				camera_2.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(camera_2);

				// render "CAMERA 2" text
				font_->RenderText(
					sprite_renderer_,
					gef::Vector4(menu_text_1_.x(), menu_text_1_.y(), -0.99f),
					1.0f,
					0xffffffff,
					gef::TJ_CENTRE,
					"CAMERA 2");
			} // !CAM2
			break;

			case CAM3:
			{
				// render CAM3 picture
				gef::Sprite camera_3;
				camera_3.set_texture(camera_3_texture_);
				camera_3.set_position(gef::Vector4(menu_text_1_.x(), menu_text_1_.y() - sprite_height * 4.0f, -0.99f));
				camera_3.set_height(platform_.height() * 0.5f);
				camera_3.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(camera_3);

				// render "CAMERA 3" text
				font_->RenderText(
					sprite_renderer_,
					gef::Vector4(menu_text_1_.x(), menu_text_1_.y(), -0.99f),
					1.0f,
					0xffffffff,
					gef::TJ_CENTRE,
					"CAMERA 3");
			} // !CAM3
			break;
		} // !camera_count_

		// render "DIFFICULTY" text
		switch (*difficulty_count_)
		{
			case EASY:
			{
				font_->RenderText(
					sprite_renderer_,
					gef::Vector4(menu_text_2_.x(), menu_text_2_.y(), -0.99f),
					1.0f,
					0xffffffff,
					gef::TJ_CENTRE,
					"DIFFIC EASY");
			} // !EASY
			break;

			case HARD:
			{
				font_->RenderText(
					sprite_renderer_,
					gef::Vector4(menu_text_2_.x(), menu_text_2_.y(), -0.99f),
					1.0f,
					0xffffffff,
					gef::TJ_CENTRE,
					"DIFFIC HARD");
			} // !EASY
			break;
		} // !difficulty_count_

		// render "GROUND" text
		font_->RenderText(
			sprite_renderer_,
			gef::Vector4(menu_text_3_.x(), menu_text_3_.y(), -0.99f),
			1.0f,
			0xffffffff,
			gef::TJ_CENTRE,
			"GROUNDS %d",
			(*number_of_grounds_));

		// render "BACK" text
		font_->RenderText(
			sprite_renderer_,
			gef::Vector4(menu_text_4_.x(), menu_text_4_.y(), -0.99f),
			1.0f,
			0xffffffff,
			gef::TJ_CENTRE,
			"BACK");

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
} // !SettingsRender
