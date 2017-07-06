#include "settings.h"
// gef headers
#include <system/platform.h>
#include <system/debug_log.h>

#include <graphics/font.h>

#include <graphics/sprite_renderer.h>
#include <graphics/sprite.h>

#include <input/input_manager.h>
#include <input/sony_controller_input_manager.h>

#include <audio/audio_manager.h>

#include <maths/vector2.h>
#include <maths/vector4.h>
// extra headers
#include "load_texture.h"
// my headers
#include "game_state_enum.h"
#include "camera_enum.h"
#include "difficulty_enum.h"

Settings::Settings(gef::Platform& platform, GAMESTATE* gamestate, unsigned* camera_count, unsigned* difficulty_count) :
	platform_(platform),
	gamestate_(gamestate),
	camera_count_(camera_count),
	difficulty_count_(difficulty_count),
	input_manager_(nullptr),
	sprite_renderer_(nullptr),
	audio_manager_(nullptr),
	cross_button_icon_(nullptr),
	playstation_left_d_pad_(nullptr),
	playstation_right_d_pad_(nullptr),
	quit_(false),
	sprite_width_(190.0f),
	sprite_height(38.0f),
	sfx_voice_id_(-1),
	sfx_id_(-1),
	fps_(0)
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
	cross_button_icon_ = CreateTextureFromPNG("playstation-cross-dark-icon.png", platform_);
	// initialise left d-pad icon
	playstation_left_d_pad_ = CreateTextureFromPNG("playstation-left-d-pad.png", platform_);
	// initialise right d-pad icon
	playstation_right_d_pad_ = CreateTextureFromPNG("playstation-right-d-pad.png", platform_);

	// initialise camera's 1 texture to show its perspective
	camera_1_texture_ = CreateTextureFromPNG("CAM1.png", platform_);
	// initialise camera's 2 texture to show its perspective
	camera_2_texture_ = CreateTextureFromPNG("CAM2.png", platform_);
	// initialise camera's 3 texture to show its perspective
	camera_3_texture_ = CreateTextureFromPNG("CAM3.png", platform_);
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
	float height_correction = 2.0f;
	// set "START" vector
	camera_text_position_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() - 0.5 * sprite_height + height_correction, -0.99f);
	// set "SETTINGS" vector
	difficulty_text_position_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() + 1.5 * sprite_height + height_correction, -0.99f);
	// set "BACK" vector
	back_text_position_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() + sprite_height * 3.5f + height_correction, -0.99f);
} // InitText()

void Settings::InitAudio()
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

void Settings::CleanAudio()
{
	if (audio_manager_)
	{
		audio_manager_->StopMusic();
		audio_manager_->UnloadAllSamples();
		sfx_id_ = -1;
		sfx_voice_id_ = -1;

		delete audio_manager_;
		audio_manager_ = nullptr;
	}
} // !CleanAudio

void Settings::SettingsInit()
{
	// initialise input manager
	input_manager_ = gef::InputManager::Create(platform_);

	InitFont();

	InitTextures();

	InitSprites();
	
	InitText();

	InitAudio();
} // !SettingsInit

void Settings::SettingsRelease()
{
	delete input_manager_;
	input_manager_ = nullptr;

	CleanFont();

	CleanTextures();

	CleanSprites();

	CleanAudio();
} // !SettingsRelease

void Settings::SonyController(const gef::SonyController* controller)
{
	if (controller)
	{
		// record where to move sprite
		/*float left_horizontal_input = controller->left_stick_x_axis();
		float left_vertical_input = controller->left_stick_y_axis();

		float right_horizontal_input = controller->right_stick_x_axis();
		float right_vertical_input = controller->right_stick_y_axis();*/

		//if (left_vertical_input < 0) up = true;

		// D-pad up
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

		// D-pad down
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

		// CAMERA CROSS press
		if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
			menu_box_sprite_.position().y() > (camera_text_position_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (camera_text_position_.y() + sprite_height))
		{
			(*camera_count_)++;

			if ((*camera_count_) >= 3)
				(*camera_count_) = 0;
		}
		// CAMERA - D-pad left
		if (controller->buttons_pressed() & gef_SONY_CTRL_LEFT &&
			menu_box_sprite_.position().y() > (camera_text_position_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (camera_text_position_.y() + sprite_height))
		{
			if ((*camera_count_) > 0)
				(*camera_count_)--;
		}
		// CAMERA - D-pad right
		if (controller->buttons_pressed() & gef_SONY_CTRL_RIGHT &&
			menu_box_sprite_.position().y() > (camera_text_position_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (camera_text_position_.y() + sprite_height))
		{

			if ((*camera_count_) < 2)
				(*camera_count_)++;
		}
		// DIFFICULTY press
		if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
			menu_box_sprite_.position().y() > (difficulty_text_position_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (difficulty_text_position_.y() + sprite_height))
		{
			(*difficulty_count_)++;

			if ((*difficulty_count_) >= 2)
				(*difficulty_count_) = 0;
		}
		// DIFFICULTY left d-pad
		if (controller->buttons_pressed() & gef_SONY_CTRL_LEFT &&
			menu_box_sprite_.position().y() > (difficulty_text_position_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (difficulty_text_position_.y() + sprite_height))
		{
			if ((*difficulty_count_) > 0)
				(*difficulty_count_)--;
		}
		// DIFFICULTY right d-pad
		if (controller->buttons_pressed() & gef_SONY_CTRL_RIGHT &&
			menu_box_sprite_.position().y() > (difficulty_text_position_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (difficulty_text_position_.y() + sprite_height))
		{
			if ((*difficulty_count_) < 1)
				(*difficulty_count_)++;
		}
		// BACK press
		if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
			menu_box_sprite_.position().y() > (back_text_position_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (back_text_position_.y() + sprite_height))
		{
			// update the current state of the game state machine
			// get the value that the gamestate points to and change it
			(*gamestate_) = FRONTEND; 
		}
		// hide d-pad
		if (menu_box_sprite_.position().y() > (back_text_position_.y() - sprite_height * 0.5f) &&
			menu_box_sprite_.position().y() < (back_text_position_.y() + sprite_height))
		{
			display_d_pad = false;
		}
		else
		{
			display_d_pad = true;
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

void Settings::SettingsUpdate(float frame_time)
{
	fps_ = 1.0f / frame_time;

	// get the latest date from the input devices
	if (input_manager_)
	{
		input_manager_->Update();

		const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);

		SonyController(controller);
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
				camera_1.set_position(gef::Vector4(camera_text_position_.x(), camera_text_position_.y() - sprite_height * 4.0f, -0.99f));
				camera_1.set_height(platform_.height() * 0.5f);
				camera_1.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(camera_1);

				// render "CAMERA1" text
				font_->RenderText(
					sprite_renderer_,
					gef::Vector4(camera_text_position_.x(), camera_text_position_.y(), -0.99f),
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
				camera_2.set_position(gef::Vector4(camera_text_position_.x(), camera_text_position_.y() - sprite_height * 4.0f, -0.99f));
				camera_2.set_height(platform_.height() * 0.5f);
				camera_2.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(camera_2);

				// render "CAMERA 2" text
				font_->RenderText(
					sprite_renderer_,
					gef::Vector4(camera_text_position_.x(), camera_text_position_.y(), -0.99f),
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
				camera_3.set_position(gef::Vector4(camera_text_position_.x(), camera_text_position_.y() - sprite_height * 4.0f, -0.99f));
				camera_3.set_height(platform_.height() * 0.5f);
				camera_3.set_width(platform_.width() * 0.5f);
				sprite_renderer_->DrawSprite(camera_3);

				// render "CAMERA 3" text
				font_->RenderText(
					sprite_renderer_,
					gef::Vector4(camera_text_position_.x(), camera_text_position_.y(), -0.99f),
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
					gef::Vector4(difficulty_text_position_.x(), difficulty_text_position_.y(), -0.99f),
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
					gef::Vector4(difficulty_text_position_.x(), difficulty_text_position_.y(), -0.99f),
					1.0f,
					0xffffffff,
					gef::TJ_CENTRE,
					"DIFFIC HARD");
			} // !EASY
			break;
		} // !difficulty_count_

		// render "BACK" text
		font_->RenderText(
			sprite_renderer_,
			gef::Vector4(back_text_position_.x(), back_text_position_.y(), -0.99f),
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

		DrawHUD();
	}
	sprite_renderer_->End();
} // !SettingsRender
