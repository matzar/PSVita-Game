#include "scene_app.h"

SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform),
	frontend_(nullptr),
	settings_(nullptr),
	game_(nullptr)
{
} // !SceneApp

void SceneApp::Init()
{
	// initialise gamestate_
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
				frontend_ = new Frontend(platform_, &gamestate_);
				frontend_->FrontendInit();

				// going to the FRONTEND state possible only from the GAME state...
				if (game_) 
				{
					// delete game
					game_->GameRelease();
					delete game_;
					game_ = nullptr;
				}
				// ... or the SETTINGS state
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
				// create settings
				// reference to the platform object is passed
				// Settings class has 'GAMESTATE* gamestate' pointer
				// adress of gamestate_ is passed to the class and assigned to the GAMESTATE pointer
				settings_ = new Settings(platform_, &gamestate_);
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
				//if (game_)
				//{
				//	// delete game
				//	game_->GameRelease();
				//	delete game_;
				//	game_ = nullptr;
				//}
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
				game_ = new Game(platform_, &gamestate_);
				game_->GameInit();

				// going to the GAME state possible only from the FRONTEND state
				if (frontend_) 
				{
					// delete frontend
					frontend_->FrontendRelease();
					delete frontend_;
					frontend_ = nullptr;
				}
				//if (settings_)
				//{
				//	// delete settings
				//	settings_->SettingsRelease();
				//	delete settings_;
				//	settings_ = nullptr;
				//}
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