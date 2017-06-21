#include "game.h"
// gef headers
#include <system/platform.h>
#include <system/debug_log.h>

#include <graphics/font.h>

#include <graphics/sprite_renderer.h>
#include <graphics/sprite.h>

#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
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
#include "contact_filter.h"
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
	sprite_renderer_(nullptr),
	input_manager_(nullptr),
	audio_manager_(nullptr),
	renderer_3d_(nullptr),
	primitive_builder_(nullptr),
	camera_(nullptr),
	contact_listener_(nullptr),
	contact_filter_(nullptr),
	world_(nullptr),
	player_(nullptr),
	//ground_,
	fps_(0),
	sfx_id_(-1),
	sfx_voice_id_(-1)
{
	ground_.reserve(5);
}

Game::~Game()
{
}

void Game::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void Game::CleanUpFont()
{
	delete font_;
	font_ = nullptr;
}

void Game::DrawHUD()
{
	if (font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}

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
}

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
	contact_filter_ = new ContactFilter();
	world_->SetContactFilter(contact_filter_);
} // !InitWorld

void Game::InitGround()
{
	/*ground_.reserve(2);*/
	// create Ground ground_ class
	float j = 0.0f;

	//for (auto ground : ground_)
	for (int i = 0; i < 5; ++i)
	{
		ground_.push_back( new Ground());
		ground_.at(i)->InitGround(primitive_builder_, world_, b2Vec2(0.0f + j, 0.0f), GROUND, PICKUP);
		j += 15.0f;
	}
	//ground_ = new Ground();
	//ground_->InitGround(primitive_builder_, world_, b2Vec2(0.0f, 0.0f));
} // !InitGround

void Game::InitPlayer()
{
	// create Player player_ class
	player_ = new Player();
	player_->InitPlayer(primitive_builder_, world_, PLAYER, PICKUP);
} // !InitPlayer


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

	InitCamera();

	InitWorld();

	InitGround();

	InitPlayer();
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

	delete primitive_builder_;
	primitive_builder_ = nullptr;

	delete contact_listener_;
	contact_listener_ = nullptr;

	// destroying the physics world also destroys all the objects within it
	// shapes and joints are destroyed in b2World::Destroy 
	// not need to explicitly delete player_body_ and ground_body_
	delete world_;
	world_ = nullptr;

	delete player_;
	player_ = nullptr;

	//delete ground_mesh_;
	//ground_mesh_ = nullptr;

	for (auto ground : ground_)
	{
		delete ground;
		ground = nullptr;
		/*delete ground_;
		ground_ = nullptr;*/
	}

	// clean up camera
	delete camera_;
	camera_ = nullptr;
}

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
			// release any resources for the frontend
			//GameRelease();

			// update the current state for the game state machine
			(*gamestate_) = FRONTEND; // get the object that gamestate points to
			//GameInit();
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

//void Game::UpdateSimulation(float frame_time)
//{
//	fps_ = 1.0f / frame_time;
//
//	// update physics world
//	float32 timeStep = 1.0f / 60.0f;
//
//	// detail of the simulation
//	int32 velocityIterations = 6;
//	int32 positionIterations = 2;
//
//	world_->Step(timeStep, velocityIterations, positionIterations);
//
//	// update object visuals from simulation data
//	player_->UpdateFromSimulation(player_->GetPlayerBody());
//
//	// don't have to update the ground visuals as it is static
//
//	// collision detection
//	// get the head of the contact list
//	b2Contact* contact = world_->GetContactList();
//	// get contact count
//	int contact_count = world_->GetContactCount();
//
//	for (int contact_num = 0; contact_num < contact_count; ++contact_num)
//	{
//		if (contact->IsTouching())
//		{
//			// get the colliding bodies
//			b2Body* bodyA = contact->GetFixtureA()->GetBody();
//			b2Body* bodyB = contact->GetFixtureB()->GetBody();
//
//			// DO COLLISION RESPONSE HERE
//			Player* player = nullptr;
//			Ground* ground = nullptr;
//
//			GameObject* gameObjectA = nullptr;
//			GameObject* gameObjectB = nullptr;
//
//			gameObjectA = (GameObject*)bodyA->GetUserData();
//			gameObjectB = (GameObject*)bodyB->GetUserData();
//
//			if (gameObjectA)
//			{
//				if (gameObjectA->type() == PLAYER)
//				{
//					player = (Player*)bodyA->GetUserData();
//				}
//			}
//
//			if (gameObjectB)
//			{
//				if (gameObjectB->type() == PLAYER)
//				{
//					player = (Player*)bodyB->GetUserData();
//				}
//			}
//
//			if (player)
//			{
//				player->DecrementHealth();
//			}
//		}
//
//		// Get next contact point
//		contact = contact->GetNext();
//	}
//} // !UpdateSimulation

void Game::UpdateSimulation(float frame_time)
{
	fps_ = 1.0f / frame_time;

	// update physics world
	float32 timeStep = 1.0f / 60.0f;

	// detail of the simulation
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world_->Step(timeStep, velocityIterations, positionIterations);

	// move the player
	// set player forward
	//b2Vec2 new_pos = player_->GetPlayerBody()->GetPosition();
	//new_pos += b2Vec2((2.0f * frame_time), 0); // multiply speed by frame time to create consistent movement
	//player_->GetPlayerBody()->SetTransform(new_pos, 0.0f);
	
	b2Vec2 vel = player_->GetPlayerBody()->GetLinearVelocity();
	vel.x = 5; 
	player_->GetPlayerBody()->SetLinearVelocity(vel);

	/*case MS_LEFT:  vel.x = b2Max(vel.x - 0.1f, -5.0f); break;
	case MS_STOP:  vel.x *= 0.98; break;
	case MS_RIGHT: vel.x = b2Min(vel.x + 0.1f, 5.0f); break;*/
	
	/*player_->GetPlayerBody()->ApplyForce(b2Vec2(4.f, 0.0f), player_->GetPlayerBody()->GetWorldCenter(), true);*/
	//player_->GetPlayerBody()->SetLinearVelocity(b2Vec2(4.0f, 0.0f));
	//player_->GetPlayerBody()->GetTransform().Set(position_, 0.0f);

	// update object visuals from simulation data
	player_->UpdateFromSimulation(player_->GetPlayerBody());
	/*
	don't have to update the ground visuals as it is static
	*/

	// collision detection
	/*if (player_->IsContacting() > 0)
	{
		player_->DecrementHealth();
	}
	else
	{
		gef::DebugOut("End Contact\n");
	}*/

	if (contact_filter_->ShouldCollide(player_->GetPlayerBody()->GetFixtureList(), ground_.at(0)->GetGroundBody()->GetFixtureList()))
	{
		player_->DecrementHealth();
	}
	else
	{
		gef::DebugOut("End Contact\n");
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
}

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
		// draw ground
		for (auto ground : ground_)
		{
			renderer_3d_->DrawMesh(*ground);
		}

		// draw player
		renderer_3d_->set_override_material(&primitive_builder_->red_material());
		renderer_3d_->DrawMesh(*player_);
		renderer_3d_->set_override_material(nullptr);
	}
	renderer_3d_->End();

	// start drawing sprites, but don't clear the frame buffer
	sprite_renderer_->Begin(false);
	{
		DrawHUD();
	}
	sprite_renderer_->End();
}

