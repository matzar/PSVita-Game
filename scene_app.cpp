#include "scene_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <graphics/sprite.h>
#include <maths/math_utils.h>
#include <input/sony_controller_input_manager.h>
#include "load_texture.h"


#ifdef _WIN32
// only on windows platforms
#include <Windows.h>
#include <platform/d3d11/input/keyboard_d3d11.h>
#include <platform/d3d11/input/touch_input_manager_d3d11.h>
//#include <freeglut.h>
#endif 


SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform)
{
	frontend_ = new Frontend(platform);
	game_ = new Game(platform);
}

void SceneApp::Init()
{


	// initialise the game state machine
	gamestate_ = FRONTEND;

	//// initlalise sprite renderer
	//sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	//// initialise input manager
	//input_manager_ = gef::InputManager::Create(platform_);
	//// initialise audio manager
	//audio_manager_ = gef::AudioManager::Create();

	//InitFont();
	//switch (gamestate_)
	//{
	//case FRONTEND:
	//{
		frontend_->FrontendInit();;
	//}
	//break;

	//case GAME:
	//{
		game_->GameInit();
	//}
	//break;
	//}
}

void SceneApp::CleanUp()
{
	// audio manager
	//delete audio_manager_;
	//audio_manager_ = NULL;

	//// input manager
	//delete input_manager_;
	//input_manager_ = NULL;

	//delete sprite_renderer_;
	//sprite_renderer_ = NULL;

	//CleanUpFont();
	delete frontend_;
	frontend_ = nullptr;

	delete game_;
	game_ = nullptr;
}

bool SceneApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;

	// get the latest date from the input devices
	//if (input_manager_)
	//{
	//	input_manager_->Update();

	//	// get controller input data and read controller data for controler 0
	//	const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);
	//	
	//	if (controller)
	//	{
	//		
	//	}
	//}

	switch (globals::gamestate)
	{
	case FRONTEND:
	{
		frontend_->FrontendUpdate(frame_time);
		// frontend_->GetNextGameState();
	}
	break;

	case GAME:
	{
		//game_->GameInit();
		game_->GameUpdate(frame_time);
	}
	break;
	}

	return true;
}

void SceneApp::Render()
{
	switch (globals::gamestate)
	{
	case FRONTEND:
	{
		frontend_->FrontendRender();
	}
	break;

	case GAME:
	{
		game_->GameRender();
	}
	break;
	}
}