#include "game.h"
// gef headers
#include <system/platform.h>
#include <system/debug_log.h>

#include <graphics/font.h>

#include <graphics/sprite_renderer.h>
#include <graphics/sprite.h>

#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <graphics/scene.h>
#include <maths/math_utils.h>

#include <input/input_manager.h>
#include <input/sony_controller_input_manager.h>
#include <audio/audio_manager.h>
// std headers
#include <typeinfo>	 // for 'typeid'
//#include <vector>
// extra headers
#include "load_texture.h"
#include "primitive_builder.h"
//#include "game_object.h"
#include "player.h"
#include "ground.h"
#include "pickup.h"
//#include "contact_filter.h"
// my headers
#include "free_camera.h"
#include "game_state_enum.h"
#include "contact_listener.h"
// box2D headers
#include <box2d/Box2D.h>

#ifdef _WIN32
// only on windows platforms
#include <Windows.h>
#include <platform/d3d11/input/keyboard_d3d11.h>
#include <platform/d3d11/input/touch_input_manager_d3d11.h>
//#include <freeglut.h>
#endif 

Game::Game(gef::Platform& platform, GAMESTATE* gamestate) :
	platform_(platform),
	gamestate_(gamestate),
	font_(nullptr),
	texture_(nullptr),
	sprite_renderer_(nullptr),
	input_manager_(nullptr),
	audio_manager_(nullptr),
	renderer_3d_(nullptr),
	primitive_builder_(nullptr),
	camera_(nullptr),
	contact_listener_(nullptr),
	//contact_filter_(nullptr),
	world_(nullptr),
	player_(nullptr),
	model_scene_(nullptr),
	mesh_(nullptr),
	quit_(false),
	fps_(0),
	sfx_id_(-1),
	sfx_voice_id_(-1)
{
	ground_.reserve(5);
	pickups_.reserve(3);
}

Game::~Game()
{
}

void Game::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
} // !InitFont

void Game::CleanupFont()
{
	delete font_;
	font_ = nullptr;
} // !CleanupFont

void Game::DrawHUD()
{
	if (font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
} // !DrawHUD

void Game::SetupLights()
{
	// grab the data for the default shader used for rendering 3D geometry
	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();

	// set the ambient light
	default_shader_data.set_ambient_light_colour(gef::Colour(0.25f, 0.25f, 0.25f, 1.0f));

	// add a point light that is almost white, but with a blue tinge
	// the position of the light is set far away so it acts light a directional light
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));
	default_shader_data.AddPointLight(default_point_light);
} // !SetupLights

void Game::InitTextures()
{
	texture_ = CreateTextureFromPNG("rock.png", platform_);
	texture_material_.set_texture(texture_);
} // !InitTextures

void Game::CleanupTextures()
{
	delete texture_;
	texture_ = nullptr;
} // !ClenupTextures

void Game::InitAudio()
{
	// audio manager
	audio_manager_ = gef::AudioManager::Create();

	// load audio assets
	if (audio_manager_)
	{
		// load a sound effect
		sfx_id_ = audio_manager_->LoadSample("box_collected.wav", platform_);

		// load in music
		audio_manager_->LoadMusic("music.wav", platform_);

		// play music
		audio_manager_->PlayMusic();
	}
} // !InitAudio

void Game::InitCamera()
{
	camera_ = new FreeCamera;
	camera_->Update();
	camera_->DisplayCameraPosition();
} // !InitCamera

void Game::InitWorld()
{
	// initialise the physics world
	b2Vec2 gravity(0.0f, -9.81f);
	world_ = new b2World(gravity);

	contact_listener_ = new ContactListener();
	// possible to pass custom ContactListener class becuase it has inherited from b2ContactListener class
	world_->SetContactListener(contact_listener_);

	// contact filter
	//contact_filter_ = new ContactFilter();
	//world_->SetContactFilter(contact_filter_);
} // !InitWorld

void Game::InitPlayer()
{
	// create Player player_ class
	player_ = new Player();
	player_->InitPlayer(primitive_builder_, world_, b2Vec2(-4.0f, 4.0f), 0.5f, PLAYER, GROUND | PICKUP, 1, PLAYER, RED);
} // !InitPlayer

void Game::InitGround()
{
	// will be used for grounds intervals
	float interval = 0.0f;

	//for (auto ground : ground_)
	for (int i = 0; i < 5; ++i)
	{
		ground_.push_back( new Ground());
		if (i % 2 == 0)
			ground_.at(i)->InitGround(primitive_builder_, world_, b2Vec2(0.0f + interval, 0.0f), GROUND, PLAYER | PICKUP, 1, GROUND, RED);
		else
			ground_.at(i)->InitGround(primitive_builder_, world_, b2Vec2(0.0f + interval, 0.0f), GROUND, PLAYER | PICKUP, 1, GROUND, BLUE);
		interval += 15.0f;
	}
} // !InitGround

void Game::InitPickups()
{
	// create a new scene object and read in the data from the file
	// no meshes or materials are created yet
	// we're not making any assumptions about what the data may be loaded in for
	model_scene_ = new gef::Scene();
	model_scene_->ReadSceneFromFile(platform_, "triceratop.scn");

	// we do want to render the data stored in the scene file so lets create the materials from the material data present in the scene file
	model_scene_->CreateMaterials(platform_);

	// now check to see if there is any mesh data in the file, if so lets create a mesh from it
	if (model_scene_->meshes.size() > 0)
		mesh_ = model_scene_->CreateMesh(platform_, model_scene_->meshes.front());

	// get the player mesh instance to use this mesh for drawing
	//pickups_.at(0)->set_mesh(mesh_);

	// create Pickup pclass
	float j = 0.0f;

	//for (auto ground : ground_)
	for (int i = 0; i < 3; ++i)
	{
		pickups_.push_back(new Pickup());
		pickups_.at(i)->InitPickup(primitive_builder_, world_, b2Vec2(0.0f + j, 1.0f), 0.2f, mesh_, PICKUP, PLAYER | GROUND, 1, PICKUP);
		j += 3.0f;
	}
} // !InitPickups

void Game::GameInit()
{
	// initialise input manager
	input_manager_ = gef::InputManager::Create(platform_);

	// initlalise sprite renderer
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

	// create the renderer for draw 3D geometry
	renderer_3d_ = gef::Renderer3D::Create(platform_);

	// initialise primitive builder to make 3D geometry creation easier
	primitive_builder_ = new PrimitiveBuilder(platform_);

	InitAudio();

	InitFont();

	SetupLights();

	InitTextures();

	InitCamera();

	InitWorld();

	InitPlayer();

	InitGround();

	InitPickups();
} // !GameInit

void Game::GameRelease()
{
	delete font_;
	font_ = nullptr;

	delete sprite_renderer_;
	sprite_renderer_ = nullptr;

	delete  input_manager_;
	input_manager_ = nullptr;

	// unload audio resources
	if (audio_manager_)
	{
		audio_manager_->StopMusic();
		audio_manager_->UnloadAllSamples();
		sfx_id_ = -1;
		sfx_voice_id_ = -1;

		delete audio_manager_;
		audio_manager_ = nullptr;
	}

	delete renderer_3d_;
	renderer_3d_ = nullptr;

	primitive_builder_->CleanUp();
	delete primitive_builder_;
	primitive_builder_ = nullptr;

	// destroying the physics world also destroys all the objects within it
	// shapes and joints are destroyed in b2World::Destroy 
	// no need to explicitly delete player, ground or pickups
	// contact listener part of the world as well, no need to explicitly delete either
	delete world_;
	world_ = nullptr;

	// clean up camera
	delete camera_;
	camera_ = nullptr;

	CleanupTextures();

	delete model_scene_;
	model_scene_ = nullptr;

	delete mesh_;
	mesh_ = nullptr;

	pickups_.~vector();
	ground_.~vector();
} // !GameRelease

#ifdef _WIN32 
// Only on windows platforms
void Game::KeyboardController(Camera* camera, float frame_time)
{
	// if there is a keyboard, check the arrow keys to control the direction of the character
	gef::Keyboard* keyboard = input_manager_->keyboard();
	if (keyboard)
	{
		//const gef::KeyboardD3D11* keyboard_d3d11 = (const gef::KeyboardD3D11*)keyboard;
		float camera_speed = 10.0f;

		// keyboard input
		if (keyboard->IsKeyDown(gef::Keyboard::KC_W))
			camera->MoveForward(frame_time * camera_speed);
		if (keyboard->IsKeyDown(gef::Keyboard::KC_S))
			camera->MoveBackwards(frame_time * camera_speed);
		if (keyboard->IsKeyDown(gef::Keyboard::KC_A))
			camera->MoveSideLeft(frame_time * camera_speed);
		if (keyboard->IsKeyDown(gef::Keyboard::KC_D))
			camera->MoveSideRight(frame_time * camera_speed);
		if (keyboard->IsKeyDown(gef::Keyboard::KC_UP))
			camera->AddPitch(camera_speed * camera_speed, frame_time);
		if (keyboard->IsKeyDown(gef::Keyboard::KC_DOWN))
			camera->subtractPitch(camera_speed * camera_speed, frame_time);
		if (keyboard->IsKeyDown(gef::Keyboard::KC_LEFT))
			camera->subtractYaw(camera_speed * camera_speed, frame_time);
		if (keyboard->IsKeyDown(gef::Keyboard::KC_RIGHT))
			camera->AddYaw(camera_speed * camera_speed, frame_time);
		if (keyboard->IsKeyDown(gef::Keyboard::KC_R) || keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD8))
			camera->MoveUp(frame_time * camera_speed);
		if (keyboard->IsKeyDown(gef::Keyboard::KC_F) || keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD2))
			camera->MoveDown(frame_time * camera_speed);
	} // keyboard

	  // mouse input
	const gef::TouchInputManager* touch_input = input_manager_->touch_manager();
	if (touch_input)
	{
		// initialise the mouse position
		gef::Vector2 mouse_position(0.0f, 0.0f); // left upper corner of the window

												 // get a pointer to the d3d11 implementation of the TouchInputManager
		const gef::TouchInputManagerD3D11* touch_input_d3d11 = (const gef::TouchInputManagerD3D11*)touch_input;

		// get the mouse position
		mouse_position = touch_input_d3d11->mouse_position();

		if (touch_input_d3d11->is_button_down(0))
		{
			//SetCursorPos(480, 272);	
		}

		//gef::DebugOut("Mouse position x, y: %f %f\n", mouse_position.x, mouse_position.y);
	} // touch_input (mouse)
}
#endif // !_WIN32

void Game::SonyController(const gef::SonyController* controller)
{
	if (controller)
	{
		if (controller->buttons_pressed() & gef_SONY_CTRL_START)
		{
			// update the current state for the game state machine
			(*gamestate_) = FRONTEND; // get the object that gamestate points to
			//GameInit();
		}

		if (controller->buttons_pressed() & gef_SONY_CTRL_SELECT)
		{
			quit_ = true;
		}

		// trigger a sound effect
		if (audio_manager_)
		{
			if (controller->buttons_pressed() & gef_SONY_CTRL_CIRCLE)
			{
				audio_manager_->StopPlayingSampleVoice(sfx_voice_id_);
				sfx_voice_id_ = -1;
			}
		}
	} // !audio_manager_
} // !SonyController

void Game::UpdatePickups()
{
	if (!(contact_listener_->dying_pickups_scheduled_for_removal_.empty()))
	{
		for (Pickup* dying_pick_up : contact_listener_->dying_pickups_scheduled_for_removal_)
		{
			// remove the pickup from the rendering list
			std::vector<Pickup*>::iterator it = std::find(pickups_.begin(), pickups_.end(), dying_pick_up);
			if (it != pickups_.end())
			{
				pickups_.erase(it);
			}
			// TODO is it okay to call it before iterator?
			// pickup's physics body is destroyed here
			delete dying_pick_up;

			contact_listener_->dying_pickups_scheduled_for_removal_.erase(dying_pick_up);

		}
		//clear the set for the next time
		contact_listener_->dying_pickups_scheduled_for_removal_.clear();
	}
} // !UpdatePickups

void Game::UpdateSimulation(float frame_time)
{
	fps_ = 1.0f / frame_time;

	// update physics world
	float32 timeStep = 1.0f / 60.0f;

	// detail of the simulation
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world_->Step(timeStep, velocityIterations, positionIterations);

	// update object visuals from simulation data
	player_->UpdateFromSimulation(player_->GetBody());
	/*
	don't have to update the ground visuals as it is static
	*/

	// collision detection
	if (player_->IsContacting() > 0)
	{
		// is current ground set
		if (contact_listener_->current_ground_)
		{
			// if current ground is different colour than the player - game over
			if (contact_listener_->current_ground_->GetGameObjectColour() != player_->GetGameObjectColour())
			{
				player_->DeadPlayer();
			}
		}
	}

	UpdatePickups();

	// set camera to follow the player
	if (player_->IsAlive())
	{
		camera_->SetCameraPosition(90.0f, -12.0f, 0.0f, gef::Vector4(player_->GetBody()->GetPosition().x - 8.0f, 3.5f, 0.0f));
	}
} // !UpdateSimulation

void Game::GameUpdate(float frame_time)
{
	// get the latest date from the input devices
	if (input_manager_)
	{
		input_manager_->Update();

		// get controller input data and read controller data for controler 0
		const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);

		camera_->CameraController(frame_time, controller);
		player_->PlayerController(controller);
		SonyController(controller);


#ifdef _WIN32 // Only on windows platforms
		// if there is a keyboard, check the arrow keys to control the direction of the character
		KeyboardController(camera_, frame_time);
#endif // !_WIN32
	} // !input_manager_

	UpdateSimulation(frame_time);
} // !GameUpdate

void Game::GameRender()
{
	// setup camera
	// projection
	float fov = gef::DegToRad(45.0f);
	float aspect_ratio = (float)platform_.width() / (float)platform_.height();
	gef::Matrix44 projection_matrix;
	projection_matrix = platform_.PerspectiveProjectionFov(fov, aspect_ratio, 0.1f, 100.0f);
	renderer_3d_->set_projection_matrix(projection_matrix);

	// view
	gef::Vector4 camera_eye(camera_->GetPositionVector().x(), camera_->GetPositionVector().y(), camera_->GetPositionVector().z());
	gef::Vector4 camera_lookat(camera_->GetLookAtVector().x(), camera_->GetLookAtVector().y(), camera_->GetLookAtVector().z());
	gef::Vector4 camera_up(camera_->GetUpVector().x(), camera_->GetUpVector().y(), camera_->GetUpVector().z());
	gef::Matrix44 view_matrix;
	view_matrix.LookAt(camera_eye, camera_lookat, camera_up);
	renderer_3d_->set_view_matrix(view_matrix);

	// draw 3d geometry
	renderer_3d_->Begin();
	{
		// draw player
		// set texture
		if (player_->PlayerIsRed())
			renderer_3d_->set_override_material(&primitive_builder_->red_material());
		else
			renderer_3d_->set_override_material(&primitive_builder_->blue_material());
		// draw texture
		renderer_3d_->DrawMesh(*player_);
		// unmount texture
		renderer_3d_->set_override_material(nullptr);

		// draw ground
		for (auto ground : ground_)
		{
			// set texture
			if (ground->GetGameObjectColour() == RED)
				renderer_3d_->set_override_material(&primitive_builder_->red_material());
			else if (ground->GetGameObjectColour() == BLUE)
				renderer_3d_->set_override_material(&primitive_builder_->blue_material());
			else
				renderer_3d_->set_override_material(&texture_material_);
			// draw texture
			renderer_3d_->DrawMesh(*ground);
			// unmount texture
			renderer_3d_->set_override_material(nullptr);

			// TODO set texture
			// set texture
			//renderer_3d_->set_override_material(&texture_material_);
			//renderer_3d_->DrawMesh(*ground);
			// unmount texture
			//renderer_3d_->set_override_material(nullptr);
		}

		// draw pickups
		for (auto pickup : pickups_)
		{
			// set texture
			//renderer_3d_->set_override_material(&texture_material_);
			renderer_3d_->DrawMesh(*pickup);
			// unmount texture
			//renderer_3d_->set_override_material(nullptr);
		}
	}
	renderer_3d_->End();

	// start drawing sprites, but don't clear the frame buffer
	sprite_renderer_->Begin(false);
	{
		DrawHUD();
	}
	sprite_renderer_->End();
} // !GameRenderer