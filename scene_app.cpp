#include "scene_app.h"

SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform)
{
	// frontend scene
	// reference to the platform object is passed
	// Frontend class has 'GAMESTATE* gamestate' pointer
	// adress of gamestate_ is passed to the class and assigned to the GAMESTATE pointer
	frontend_ = nullptr;
	// game scene
	// reference to the platform object is passed
	// Game class has 'GAMESTATE* gamestate' pointer
	// adress of gamestate_ is passed to the class and assigned to the GAMESTATE pointer
	game_ = nullptr;
} // !SceneApp

void SceneApp::Init()
{
	// initialise gamestate_
	gamestate_ = FRONTEND;
} // !Init

void SceneApp::CleanUp()
{
	// frontend_
	if (frontend_) {
		frontend_->FrontendRelease();
		delete frontend_;
		frontend_ = nullptr;
	}

	// game_
	if (game_) {
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
			if (frontend_ == nullptr) {
				// Create frontend
				frontend_ = new Frontend(platform_, &gamestate_);
				frontend_->FrontendInit();

				// Delete game
				if (game_) {
					game_->GameRelease();
					delete game_;
					game_ = nullptr;
				}
			}
			frontend_->FrontendUpdate(frame_time);
		} // !FRONTEND
		break;

		case GAME:
		{
			if (game_ == nullptr) {
				// Create frontend
				game_ = new Game(platform_, &gamestate_);
				game_->GameInit();

				if (frontend_) {
					// Delete game
					frontend_->FrontendRelease();
					delete frontend_;
					frontend_ = nullptr;
				}
			}
			game_->GameUpdate(frame_time);
		} // !GAME
		break;
	} // !gamestate_

	return true;
} // !Update

void SceneApp::Render()
{
	switch (gamestate_)
	{
		case FRONTEND:
		{
			if (frontend_)
			frontend_->FrontendRender();
		} // !FRONTEND
		break;

		case GAME:
		{
			if (game_)
			game_->GameRender();
		} // !GAME
		break;
	} // !gamestate_
} // !Render