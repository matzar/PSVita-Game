#ifndef _GAME_H
#define _GAME_H

#include <system/application.h>
#include <maths/vector2.h>

#include "game_object.h"
#include "primitive_builder.h"

#include "free_camera.h"
#include "game_state_enum.h"

#include <box2d/Box2D.h>

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class InputManager;
	class Renderer3D;
	class Mesh;
	class InputManager;
	class AudioManager;
} // gef


class Game
{
public:
	Game(gef::Platform& platform, GAMESTATE* gamestate);
	~Game();

	void GameInit();
	void GameRelease();
	void UpdateSimulation(float frame_time);
	void GameUpdate(float frame_time);
	void GameRender();
private:
	// gamestate_ pointer
	GAMESTATE* gamestate_;

	// sprite renderer
	gef::SpriteRenderer* sprite_renderer_;

	// font
	gef::Font* font_;

	// input manager
	gef::InputManager* input_manager_;

	// audio manager
	gef::AudioManager* audio_manager_;

	// platform
	gef::Platform& platform_;

	// render 3D
	gef::Renderer3D* renderer_3d_;

	// primitive builder
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

	float fps_;
	// audio variables
	int sfx_id_;
	int sfx_voice_id_;

	void InitPlayer();
	void InitGround();
	void SetupLights();
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
};

#endif // !_GAME_H
