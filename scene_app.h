#ifndef _SCENE_APP_H
#define _SCENE_APP_H

#include <system/application.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>
#include <input/input_manager.h>
#include <audio/audio_manager.h>
#include <box2d/Box2D.h>
#include "game_object.h"
#include "free_camera.h"
#include "front_end.h"
#include "game.h"

// GAMESTATE declarations
enum GAMESTATE
{
	FRONTEND,
	GAME
}; // GAMESTATE

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class InputManager;
	class Renderer3D;
} // gef

class SceneApp : public gef::Application
{
public:
	SceneApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();
private:
	void InitPlayer();
	void InitGround();
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
	void SetupLights();

	void UpdateSimulation(float frame_time);
    
	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
	
	// input manager
	gef::InputManager* input_manager_;

	// audio manager
	gef::AudioManager* audio_manager_;

	//
	// GAMESTATE VARIABLES
	//
	GAMESTATE game_state_;

	//
	// FRONTEND DECLARATIONS
	//
	gef::Texture* button_icon_;

	//
	// GAME DECLARATIONS
	//
	gef::Renderer3D* renderer_3d_;
	PrimitiveBuilder* primitive_builder_;

	// create the physics world
	b2World* world_;

	// player variables
	Player player_;
	b2Body* player_body_;

	// ground variables
	gef::Mesh* ground_mesh_;
	GameObject ground_;
	b2Body* ground_body_;

	// camera variables
	Camera* camera_;

	// audio variables
	int sfx_id_;
	int sfx_voice_id_;

	float fps_;

	//
	// FRONTEND FUNCTIONS
	//
	void FrontendInit();
	void FrontendRelease();
	void FrontendUpdate(float frame_time);
	void FrontendRender();

	//
	// GAMESTATE FUNCITONS
	//
	void GameInit();
	void GameRelease();
	void GameUpdate(float frame_time);
	void GameRender();
}; // class SceneApp : public gef::Application

#endif // _SCENE_APP_H
