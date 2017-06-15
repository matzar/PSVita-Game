#include "scene_app.h"

SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform)
{
	// frontend scene
	// reference to the platform object is passed
	// Frontend class has 'GAMESTATE* gamestate' pointer
	// adress of gamestate_ is passed to the class and assigned to the GAMESTATE pointer
	frontend_ = new Frontend(platform, &gamestate_);
	// game scene
	// reference to the platform object is passed
	// Game class has 'GAMESTATE* gamestate' pointer
	// adress of gamestate_ is passed to the class and assigned to the GAMESTATE pointer
	game_ = new Game(platform, &gamestate_);
}

void SceneApp::Init()
{
	// initialise gamestate_
	gamestate_ = FRONTEND;

	// initialise frontend_
	frontend_->FrontendInit();;

	// initialise game_
	game_->GameInit();
}

void SceneApp::CleanUp()
{
	delete frontend_;
	frontend_ = nullptr;

	delete game_;
	game_ = nullptr;
}

bool SceneApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;

	switch (gamestate_)
	{
		case FRONTEND:
		{
			frontend_->FrontendUpdate(frame_time);
		} // !FRONTEND
		break;

		case GAME:
		{
			game_->GameUpdate(frame_time);
		} // !GAME
		break;
	} // !gamestate_

	return true;
}

void SceneApp::Render()
{
	switch (gamestate_)
	{
		case FRONTEND:
		{
			frontend_->FrontendRender();
		} // !FRONTEND
		break;

		case GAME:
		{
			game_->GameRender();
		} // !GAME
		break;
	} // !gamestate_
}