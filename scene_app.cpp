#include "scene_app.h"
#include "game.h"
#include "frontend.h"
#include "settings.h"
#include "camera_enum.h"
#include "difficulty_enum.h"

// gef headers
#include <audio/audio_manager.h>
#include <input/input_manager.h>
#include <input/touch_input_manager.h>
#include <graphics/font.h>
#include <graphics/sprite_renderer.h>

SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform),
	input_manager_(nullptr),
	audio_manager_(nullptr),
	sprite_renderer_(nullptr),
	font_(nullptr),
	camera_count_(CAM1),
	difficulty_count_(EASY),
	number_of_grounds_(10),
	pickup_sfx_id_(-1),
	frontend_(nullptr),
	settings_(nullptr),
	game_(nullptr)
{
} // !SceneApp

void SceneApp::InitFont()
{
	// initlalise sprite renderer
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
} // !InitFont

void SceneApp::CleanFont()
{
	delete sprite_renderer_;
	sprite_renderer_ = nullptr;

	delete font_;
	font_ = nullptr;
} // CleanFont

void SceneApp::Loading()
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
			"LOADING...");
	} // !font_
} // !Loading

void SceneApp::InitAudio()
{
	audio_manager_ = gef::AudioManager::Create();

	pickup_sfx_id_ = audio_manager_->LoadSample("audio/box_collected.wav", platform_);

	audio_manager_->LoadMusic("audio/SPACE_IS_COOL.wav", platform_);

	audio_manager_->PlayMusic();
} // !InitAudio

void SceneApp::CleanAudio()
{
	if (audio_manager_)
	{
		audio_manager_->StopMusic();
		audio_manager_->UnloadAllSamples();

		pickup_sfx_id_ = -1;

		delete audio_manager_;
		audio_manager_ = nullptr;
	}
} // !ClenaAudio

void SceneApp::Init()
{
	// initialize input manager
	input_manager_ = gef::InputManager::Create(platform_);

	InitFont();

	// make sure if there is a panel to detect touch input, then activate it
	if (input_manager_ && input_manager_->touch_manager() && (input_manager_->touch_manager()->max_num_panels() > 0))
		input_manager_->touch_manager()->EnablePanel(0);

	// only on PSVita
#ifndef _WIN32
	// initialize audio 
	InitAudio();
#endif // _WIN32

	// initialize gamestate_
	gamestate_ = FRONTEND;
} // !Init

void SceneApp::CleanUp()
{
	// frontend_
	if (frontend_) 
	{
		frontend_->FrontendRelease();
		delete frontend_;
		frontend_ = nullptr;
	}

	// settings
	if (settings_)
	{
		settings_->SettingsRelease();
		delete settings_;
		settings_ = nullptr;
	}

	// game_
	if (game_) 
	{
		game_->GameRelease();
		delete game_;
		game_ = nullptr;
	}

	// clean up input manager
	delete input_manager_;
	input_manager_ = nullptr;

	CleanFont();

	// only on PSVita
#ifndef _WIN32
	// clean up audio
	CleanAudio();
#endif // _WIN32
} // !CleanUp

bool SceneApp::Update(float frame_time)
{
	switch (gamestate_)
	{
		case FRONTEND:
		{
			if (frontend_ == nullptr) 
			{

				// create frontend
				// reference to the platform object is passed
				// Frontend class has 'GAMESTATE* gamestate' pointer
				// adress of gamestate_ is passed to the class and assigned to the GAMESTATE pointer
				frontend_ = new Frontend(platform_, input_manager_, &gamestate_);
				frontend_->FrontendInit();

				// going to the FRONTEND state possible only from the GAME state...
				if (game_) 
				{
					// delete game
					game_->GameRelease();
					delete game_;
					game_ = nullptr;
				}
				// ...or the SETTINGS state
				if (settings_)
				{
					// delete settings
					settings_->SettingsRelease();
					delete settings_;
					settings_ = nullptr;
				}
			}

			// frontend update function
			frontend_->FrontendUpdate(frame_time);

			// quit application
			if (frontend_->Quit())
			{
				return false;
			}
		} // !FRONTEND
		break;

		case SETTINGS:
		{
			if (settings_ == nullptr)
			{
				sprite_renderer_->Begin();
				Loading();
				sprite_renderer_->End();
				// create settings
				// reference to the platform object is passed
				// Settings class has 'GAMESTATE* gamestate' pointer
				// adress of gamestate_ is passed to the class and assigned to the GAMESTATE pointer
				settings_ = new Settings(platform_, input_manager_, &gamestate_, &camera_count_, &difficulty_count_, &number_of_grounds_);
				settings_->SettingsInit();

				// going to the SETTINGS state possible only from the FRONTEND state
				// delete frontend
				if (frontend_) 
				{
					// delete frontend
					frontend_->FrontendRelease();
					delete frontend_;
					frontend_ = nullptr;
				}
			}

			// settings update function
			settings_->SettingsUpdate(frame_time);

			// quit application
			if (settings_->Quit())
			{
				return false;
			}
		} // !SETTINGS
		break;

		case GAME:
		{
			if (game_ == nullptr) 
			{
				// create game
				// reference to the platform object is passed
				// Game class has 'GAMESTATE* gamestate' pointer
				// adress of gamestate_ is passed to the class and assigned to the GAMESTATE pointer
				game_ = new Game(platform_, input_manager_, audio_manager_, &gamestate_, &camera_count_, &difficulty_count_, &number_of_grounds_, &pickup_sfx_id_);
				game_->GameInit();

				// going to the GAME state possible only from the FRONTEND state
				if (frontend_) 
				{
					// delete frontend
					frontend_->FrontendRelease();
					delete frontend_;
					frontend_ = nullptr;
				}
			}
			// game update function
			game_->GameUpdate(frame_time);

			// quit application
			if (game_->Quit())
			{
				return false;
			}
		} // !GAME
		break;
	} // !gamestate_

	// keep running application
	return true;
} // !Update

void SceneApp::Render()
{
	switch (gamestate_)
	{
		case FRONTEND:
		{
			if (frontend_)
			{
				frontend_->FrontendRender();
			}
		} // !FRONTEND
		break;

		case SETTINGS:
		{
			if (settings_)
			{
				settings_->SettingsRender();
			}
		} // !SETTINGS
		break;

		case GAME:
		{
			if (game_)
			{
				game_->GameRender();
			}
		} // !GAME
		break;
	} // !gamestate_
} // !Render