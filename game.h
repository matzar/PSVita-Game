#ifndef _GAME_H
#define _GAME_H

// gef headers
#include <system/application.h>
#include "graphics/sprite.h"
// std headers
#include <vector>
#include <box2d/Box2D.h>

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
	class Sprite;
} // gef

// Class forward declarations
class PrimitiveBuilder;
class Player;
class Ground;
class Pickup;
//class GameObject;
// My class forward declarations
enum GAMESTATE;
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
	Game(gef::Platform& platform, GAMESTATE* gamestate, unsigned* camera_count, unsigned* difficulty_count);
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

	// camera
	Camera* camera_;
	unsigned* camera_count_;

	// difficulty
	unsigned* difficulty_count_;
	float32 x_velocity;
	float32 y_velocity;

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

	// contact listener
	ContactListener* contact_listener_;

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

	// menu box sprite
	gef::Sprite menu_box_sprite_;
	float sprite_width_;
	float sprite_height;
	float sprite_init_position_y_;

	// private member functions
	void SonyController(const gef::SonyController* controller);

	void UpdatePickups();
	void UpdateSimulation(float frame_time);

	void InitFont();
	void CleanFont();
	void DrawHUD();

	void InitTextures();
	void CleanTextures();

	void InitSprites();
	void CleanSprites();

	void InitText();

	void InitAudio();
	void CleanAudio();

	void SetupLights();

	void InitCamera();
	void CleanCamera();

	void InitWorld();
	void CleanWorld();

	void InitPlayer();
	void CleanPlayer();

	void InitLevel();
	void CleanLevel();
};

#endif // !_GAME_H
