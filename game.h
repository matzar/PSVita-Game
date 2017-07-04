#ifndef _GAME_H
#define _GAME_H

// gef headers
#include <system/application.h>

// std headers
#include <vector>
#include <box2d/Box2D.h>
#include <camera_enum.h>

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class Font;
	class SpriteRenderer;
	class InputManager;
	class AudioManager;
	class Renderer3D;
	class Mesh;
	class SonyController;
	class Texture;
	class Scene;
	class Vector2;
	class Material;
} // gef


// Class forward declarations
class PrimitiveBuilder;
class Player;
class Ground;
class Pickup;
//class GameObject;
// My class forward declarations
enum GAMESTATE;
enum CAMERA_ENUM;
class Camera;
class ContactListener;
class ContactFilter;
// box2D forward declarations
class b2World;
class b2Body;
class b2ContactListener;
class b2ContactFilter;

class Game
{
public:
	Game(gef::Platform& platform, GAMESTATE* gamestate);
	~Game();

	void GameInit();
	void GameRelease();
	void GameUpdate(float frame_time);
	void GameRender();

	inline const bool Quit() { return quit_; }
private:
	// platform
	gef::Platform& platform_;

	// gamestate_ pointer
	GAMESTATE* gamestate_;

	CAMERA_ENUM camera_enum;
	// font
	gef::Font* font_;

	// textures
	gef::Texture* texture_;
	// textures' materials
	gef::Material* texture_material_;

	// sprite renderer
	gef::SpriteRenderer* sprite_renderer_;

	// input manager
	gef::InputManager* input_manager_;

	// audio manager
	gef::AudioManager* audio_manager_;

	// render 3D
	gef::Renderer3D* renderer_3d_;

	// primitive builder
	PrimitiveBuilder* primitive_builder_;

	// camera
	Camera* camera_;

	// contact listener
	ContactListener* contact_listener_;

	// contact filter
	//b2ContactFilter* contact_filter_;

	// create the physics world
	b2World* world_;

	// ground
	std::vector<Ground*> ground_;
	//Ground* ground_;

	// player
	Player* player_;

	// pickup
	std::vector<Pickup*> pickups_;

	// model
	gef::Scene* model_scene_;
	gef::Mesh* mesh_;
	//gef::MeshInstance apple_;

	// camera variables
	bool camera_1;
	bool camera_2;
	bool camera_3;
	uint16 camera_count;
	// quit
	bool quit_;
	// FPS
	float fps_;
	// audio variables
	int pickup_sfx_id_;
	int sfx_voice_id_;
	// pickups count
	uint16 pickups_count_;

	uint16 catA;
	uint16 maskA;
	uint16 catB;
	uint16 maskB;

#ifdef _WIN32
	void KeyboardController(Camera* camera, float fps);
#endif // _WIN32
	void SonyController(const gef::SonyController* controller);
	void UpdatePickups();
	void UpdateSimulation(float frame_time);
	void InitAudio();
	void InitFont();
	void CleanupFont();
	void DrawHUD();
	void SetupLights();
	void InitTextures();
	void CleanupTextures();
	void InitCamera();
	void CleanupCamera();
	void InitWorld();
	void CleanupWorld();
	void InitPlayer();
	void CleanupPlayer();
	void InitLevel();
	void CleanupGround();
	//void InitPickups();
	void CleanupPickups();
};

#endif // !_GAME_H
