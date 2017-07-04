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

				// delete game
				if (game_) 
				{
					game_->GameRelease();
					delete game_;
					game_ = nullptr;
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