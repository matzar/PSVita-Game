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
// extra headers
#include "load_texture.h"
#include "primitive_builder.h"
//#include "game_object.h"
#include "player.h"
#include "ground.h"
// my headers
#include "free_camera.h"
#include "game_state_enum.h"
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
	world_(nullptr),
	player_body_(nullptr),
	player_(nullptr),
	ground_mesh_(nullptr),
	ground_(nullptr),
	ground_body_(nullptr),
	camera_(nullptr),
	fps_(0),
	sfx_id_(-1),
	sfx_voice_id_(-1)
{
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

void Game::InitWorld()
{
	// initialise the physics world
	b2Vec2 gravity(0.0f, -9.81f);
	world_ = new b2World(gravity);
} // !InitWorld

void Game::InitGround()
{
	// create GameObject ground_ class
	ground_ = new Ground();
	// ground dimensions
	gef::Vector4 ground_half_dimensions(5.0f, 0.5f, 0.5f);

	// setup the mesh for the ground
	ground_mesh_ = primitive_builder_->CreateBoxMesh(ground_half_dimensions);
	ground_->set_mesh(ground_mesh_);

	// create a physics body
	b2BodyDef body_def;
	body_def.type = b2_staticBody;
	body_def.position = b2Vec2(0.0f, 0.0f);

	ground_body_ = world_->CreateBody(&body_def);

	// create the shape
	b2PolygonShape shape;
	shape.SetAsBox(ground_half_dimensions.x(), ground_half_dimensions.y());

	// create the fixture
	b2FixtureDef fixture_def;
	fixture_def.shape = &shape;

	// create the fixture on the rigid body
	ground_body_->CreateFixture(&fixture_def);

	// update visuals from simulation data
	ground_->UpdateFromSimulation(ground_body_);
}

void Game::InitPlayer()
{
	// create Player player_ class
	player_ = new Player();
	// setup the mesh for the player
	player_->set_mesh(primitive_builder_->GetDefaultCubeMesh());

	// create a physics body for the player
	b2BodyDef player_body_def;
	player_body_def.type = b2_dynamicBody;
	player_body_def.position = b2Vec2(0.0f, 4.0f);

	player_body_ = world_->CreateBody(&player_body_def);

	// create the shape for the player
	b2PolygonShape player_shape;
	// if cube 1x1, need to pass half of both dimensions
	player_shape.SetAsBox(0.5f, 0.5f);

	// create the fixture
	b2FixtureDef player_fixture_def;
	player_fixture_def.shape = &player_shape;
	player_fixture_def.density = 1.0f;

	// create the fixture on the rigid body
	player_body_->CreateFixture(&player_fixture_def);

	// update visuals from simulation data
	player_->UpdateFromSimulation(player_body_);

	// create a connection between the rigid body and GameObject
	player_body_->SetUserData(&player_);
}

void Game::InitAudio()
{
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
}

void Game::InitCamera()
{
	camera_ = new FreeCamera;
	camera_->Update();
	camera_->DisplayCameraPosition();
}

void Game::GameInit()
{
	// initialise input manager
	input_manager_ = gef::InputManager::Create(platform_);

	// audio manager
	audio_manager_ = gef::AudioManager::Create();

	// initlalise sprite renderer
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

	// create the renderer for draw 3D geometry
	renderer_3d_ = gef::Renderer3D::Create(platform_);

	// initialise primitive builder to make create some 3D geometry easier
	primitive_builder_ = new PrimitiveBuilder(platform_);

	InitFont();
	SetupLights();

	InitWorld();
	InitGround();
	InitPlayer();

	InitAudio();

	InitCamera();
}

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

	// destroying the physics world also destroys all the objects within it
	// shapes and joints are destroyed in b2World::Destroy 
	// not need to explicitly delete player_body_ and ground_body_
	delete world_;
	world_ = nullptr;

	delete player_;
	player_ = nullptr;

	delete ground_mesh_;
	ground_mesh_ = nullptr;

	delete ground_;
	ground_ = nullptr;

	// clean up camera
	delete camera_;
	camera_ = nullptr;
}

#ifdef _WIN32 // Only on windows platforms
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
		//if (controller->buttons_pressed() & gef_SONY_CTRL_START)
		//{
			//GameRelease();

			//game_state_ = FRONTEND;
			//FrontendInit();
		//}

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
	player_->UpdateFromSimulation(player_body_);

	// don't have to update the ground visuals as it is static

	// collision detection
	// get the head of the contact list
	b2Contact* contact = world_->GetContactList();
	// get contact count
	int contact_count = world_->GetContactCount();

	for (int contact_num = 0; contact_num < contact_count; ++contact_num)
	{
		if (contact->IsTouching())
		{
			// get the colliding bodies
			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();

			// DO COLLISION RESPONSE HERE
			Player* player = nullptr;

			GameObject* gameObjectA = nullptr;
			GameObject* gameObjectB = nullptr;

			gameObjectA = (GameObject*)bodyA->GetUserData();
			gameObjectB = (GameObject*)bodyB->GetUserData();

			if (gameObjectA)
			{
				if (gameObjectA->type() == PLAYER)
				{
					player = (Player*)bodyA->GetUserData();
				}
			}

			if (gameObjectB)
			{
				if (gameObjectB->type() == PLAYER)
				{
					player = (Player*)bodyB->GetUserData();
				}
			}

			if (player)
			{
				player->DecrementHealth();
			}
		}

		// Get next contact point
		contact = contact->GetNext();
	}
} // !UpdateSimulation

void Game::GameUpdate(float frame_time)
{
	UpdateSimulation(frame_time);

	// get the latest date from the input devices
	if (input_manager_)
	{
		input_manager_->Update();

		// get controller input data and read controller data for controler 0
		const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);

		camera_->CameraControll(frame_time, controller);
		SonyController(controller);

#ifdef _WIN32 // Only on windows platforms
		// if there is a keyboard, check the arrow keys to control the direction of the character
		KeyboardController(camera_, frame_time);
#endif // !_WIN32
	} // !input_manager_
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
		renderer_3d_->DrawMesh(*ground_);

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

