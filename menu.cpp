#include "menu.h"
// gef headers
#include <system/platform.h>

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
#include "instructions_enum.h"

Menu::Menu(gef::Platform& platform, gef::InputManager* input_manager, GAMESTATE* gamestate, gef::Texture* texture, ...) :
	platform_(platform),
	gamestate_(gamestate),
	input_manager_(input_manager),
	sprite_renderer_(nullptr),
	title_texture_(texture),
	instructions_texture_1(texture),
	instructions_texture_2(texture),
	instructions_texture_3(texture),
	instructions_texture_4(texture),
	instructions_texture_5(texture),
	instructions_texture_6(texture),
	instructions_texture_7(texture),
	playstation_left_d_pad_(texture),
	playstation_right_d_pad_(texture),
	display_instrucitons_(texture),
	quit_(false),
	dev_(false),
	display_d_pad(true),
	instructions_page_(0),
	sprite_width_(190.0f),
	sprite_height(38.0f),
	height_correction_(3.0f),
	fps_(0),
	active_touch_id_(-1)
{
}

Menu::~Menu()
{
}

void Menu::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
} // !InitFont

void Menu::CleanFont()
{
	delete font_;
	font_ = nullptr;
} // CleanFont

void Menu::DrawHUD()
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

void Menu::InitTextures()
{
	// initialise button icon
	playstation_left_d_pad_ = CreateTextureFromPNG("png/playstation-left-d-pad.png", platform_);
	playstation_right_d_pad_ = CreateTextureFromPNG("png/playstation-right-d-pad.png", platform_);
	// initialize title texture
	title_texture_ = CreateTextureFromPNG("png/title_screen.png", platform_);
	// initialize instructions page 1
	instructions_texture_1 = CreateTextureFromPNG("png/instructions_1.png", platform_);
	instructions_texture_2 = CreateTextureFromPNG("png/instructions_2.png", platform_);
	instructions_texture_3 = CreateTextureFromPNG("png/instructions_3.png", platform_);
	instructions_texture_4 = CreateTextureFromPNG("png/instructions_4.png", platform_);
	instructions_texture_5 = CreateTextureFromPNG("png/instructions_5.png", platform_);
	instructions_texture_6 = CreateTextureFromPNG("png/instructions_6.png", platform_);
	instructions_texture_7 = CreateTextureFromPNG("png/instructions_7.png", platform_);
} // !InitTextures

void Menu::CleanTextures()
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

	delete instructions_texture_6;
	instructions_texture_6 = nullptr;

	delete instructions_texture_7;
	instructions_texture_7 = nullptr;

	// icon textures
	delete playstation_left_d_pad_;
	playstation_left_d_pad_ = nullptr;

	delete playstation_right_d_pad_;
	playstation_right_d_pad_ = nullptr;
} // !CleanTextures

void Menu::InitSprites()
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
} // !InitSprites

void Menu::CleanSprites()
{
	delete sprite_renderer_;
	sprite_renderer_ = nullptr;
} // !CleanSprites

void Menu::InitText()
{
	// set "START" vector
	menu_text_1_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() - 0.5 * sprite_height + height_correction_, -0.99f);
	// set "SETTINGS" vector
	menu_text_2_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() + sprite_height + height_correction_, -0.99f);
	// set "BACK" vector
	menu_text_3_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() + sprite_height * 2.5f + height_correction_, -0.99f);
	// set "BACK" vector
	menu_text_4_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() + sprite_height * 4.0f + height_correction_, -0.99f);
} // InitText()

void Menu::MenuInit()
{
	// initialise input manager
	//input_manager_ = gef::InputManager::Create(platform_);

	InitFont();

	InitTextures();

	InitSprites();

	InitText();
} // !MenuInit

void Menu::MenuRelease()
{
	CleanSprites();

	CleanTextures();

	CleanFont();
} // !MenuRelease

void Menu::MenuTouchInput()
{
	// GAME press
	if (touch_position_.y > (menu_text_1_.y() - sprite_height * 0.5f) &&
		touch_position_.y < (menu_text_1_.y() + sprite_height) &&
		touch_position_.x >(platform_.width() / 2 - sprite_width_ / 2) &&
		touch_position_.x < (platform_.width() / 2 + sprite_width_ / 2))
	{
		// update the current state of the game state machine
		// get the value that the gamestate points to and change it
		(*gamestate_) = GAME;
	}

	// SETTINGS press
	if (touch_position_.y >(menu_text_2_.y() - sprite_height * 0.5f) &&
		touch_position_.y < (menu_text_2_.y() + sprite_height) &&
		touch_position_.x >(platform_.width() / 2 - sprite_width_ / 2) &&
		touch_position_.x < (platform_.width() / 2 + sprite_width_ / 2))
	{
		// update the current state of the game state machine
		// get the value that the gamestate points to and change it
		(*gamestate_) = SETTINGS;
	}

	// INSTRUCTIONS press
	if (touch_position_.y >(menu_text_3_.y() - sprite_height * 0.5f) &&
		touch_position_.y < (menu_text_3_.y() + sprite_height) &&
		touch_position_.x >(platform_.width() / 2 - sprite_width_ / 2) &&
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

			if (instructions_page_ >= 7)
				instructions_page_ = 0;
		}
	}

	// QUIT press
	if (touch_position_.y > (menu_text_4_.y() - sprite_height * 0.5f) &&
		touch_position_.y < (menu_text_4_.y() + sprite_height) &&
		touch_position_.x >(platform_.width() / 2 - sprite_width_ / 2) &&
		touch_position_.x < (platform_.width() / 2 + sprite_width_ / 2))
	{
		quit_ = true;
	}
}

void Menu::TouchController(const gef::TouchInputManager* touch_input)
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

void Menu::SonyController(const gef::SonyController* controller)
{
	if (controller)
	{
		// D-pad up
		if (controller->buttons_pressed() & gef_SONY_CTRL_UP &&
			menu_text_1_.y() <= menu_box_sprite_.position().y() - sprite_height)
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
			menu_text_4_.y() + sprite_height >= menu_box_sprite_.position().y() + sprite_height)
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

			if (instructions_page_ >= 7)
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
			if (instructions_page_ < 6)
				instructions_page_++;
		}
		// QUIT press
		if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
			menu_box_sprite_.position().y() >(menu_text_4_.y() - sprite_height * 0.5f) &&
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

void Menu::MenuUpdate(float frame_time)
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
} // !MenuUpdate

void Menu::MenuRender()
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
			case INSTRUCTIONS_1:
			{
				gef::Sprite instructions_1;
				instructions_1.set_texture(instructions_texture_1);
				instructions_1.set_position(gef::Vector4(menu_text_1_.x(), menu_text_1_.y() - sprite_height * 4.0f, -0.99f));
				instructions_1.set_height(platform_.height() * 0.5f);
				instructions_1.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(instructions_1);
			} //
			break;

			case INSTRUCTIONS_2:
			{
				gef::Sprite instructions_2;
				instructions_2.set_texture(instructions_texture_2);
				instructions_2.set_position(gef::Vector4(menu_text_1_.x(), menu_text_1_.y() - sprite_height * 4.0f, -0.99f));
				instructions_2.set_height(platform_.height() * 0.5f);
				instructions_2.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(instructions_2);
			} //
			break;

			case INSTRUCTIONS_3:
			{
				gef::Sprite instructions_3;
				instructions_3.set_texture(instructions_texture_3);
				instructions_3.set_position(gef::Vector4(menu_text_1_.x(), menu_text_1_.y() - sprite_height * 4.0f, -0.99f));
				instructions_3.set_height(platform_.height() * 0.5f);
				instructions_3.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(instructions_3);
			} //
			break;

			case INSTRUCTIONS_4:
			{
				gef::Sprite instructions_4;
				instructions_4.set_texture(instructions_texture_4);
				instructions_4.set_position(gef::Vector4(menu_text_1_.x(), menu_text_1_.y() - sprite_height * 4.0f, -0.99f));
				instructions_4.set_height(platform_.height() * 0.5f);
				instructions_4.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(instructions_4);
			} //
			break;

			case INSTRUCTIONS_5:
			{
				gef::Sprite instructions_5;
				instructions_5.set_texture(instructions_texture_5);
				instructions_5.set_position(gef::Vector4(menu_text_1_.x(), menu_text_1_.y() - sprite_height * 4.0f, -0.99f));
				instructions_5.set_height(platform_.height() * 0.5f);
				instructions_5.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(instructions_5);
			} //
			break;

			case INSTRUCTIONS_6:
			{
				gef::Sprite instructions_6;
				instructions_6.set_texture(instructions_texture_6);
				instructions_6.set_position(gef::Vector4(menu_text_1_.x(), menu_text_1_.y() - sprite_height * 4.0f, -0.99f));
				instructions_6.set_height(platform_.height() * 0.5f);
				instructions_6.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(instructions_6);
			} //
			break;

			case INSTRUCTIONS_7:
			{
				gef::Sprite instructions_7;
				instructions_7.set_texture(instructions_texture_7);
				instructions_7.set_position(gef::Vector4(menu_text_1_.x(), menu_text_1_.y() - sprite_height * 4.0f, -0.99f));
				instructions_7.set_height(platform_.height() * 0.5f);
				instructions_7.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(instructions_7);
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
} // !MenuRender

