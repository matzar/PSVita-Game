#include "game.h"
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

Game::Game()
{
}


Game::~Game()
{
}

void Game::GameInit()
{
	// create the renderer for draw 3D geometry
	renderer_3d_ = gef::Renderer3D::Create(platform_);

	// initialise primitive builder to make create some 3D geometry easier
	primitive_builder_ = new PrimitiveBuilder(platform_);

	SetupLights();

	// initialise the physics world
	b2Vec2 gravity(0.0f, -9.81f);
	world_ = new b2World(gravity);

	InitPlayer();
	InitGround();

	camera_ = new FreeCamera;
	camera_->Update();
	camera_->DisplayCameraPosition();

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

void Game::GameRelease()
{
	// unload audio resources
	if (audio_manager_)
	{
		audio_manager_->StopMusic();
		audio_manager_->UnloadAllSamples();
		sfx_id_ = -1;
		sfx_voice_id_ = -1;
	}

	// destroying the physics world also destroys all the objects within it
	delete world_;
	world_ = NULL;

	delete ground_mesh_;
	ground_mesh_ = NULL;

	delete primitive_builder_;
	primitive_builder_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

	// clean up camera
	delete camera_;
	camera_ = NULL;
}

void Game::UpdateSimulation(float frame_time)
{
	// update physics world
	float32 timeStep = 1.0f / 60.0f;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world_->Step(timeStep, velocityIterations, positionIterations);

	// update object visuals from simulation data
	player_.UpdateFromSimulation(player_body_);

	// don't have to update the ground visuals as it is static

	// collision detection
	// get the head of the contact list
	b2Contact* contact = world_->GetContactList();
	// get contact count
	int contact_count = world_->GetContactCount();

	for (int contact_num = 0; contact_num<contact_count; ++contact_num)
	{
		if (contact->IsTouching())
		{
			// get the colliding bodies
			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();

			// DO COLLISION RESPONSE HERE
			Player* player = NULL;

			GameObject* gameObjectA = NULL;
			GameObject* gameObjectB = NULL;

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


	// get the latest date from the input devices
	if (input_manager_)
	{
		input_manager_->Update();

		// get controller input data and read controller data for controler 0
		const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);

		if (controller)
		{
			camera_->CameraControll(frame_time, controller);

			if (controller->buttons_pressed() & gef_SONY_CTRL_START)
			{
				GameRelease();

				game_state_ = FRONTEND;
				FrontendInit();
			}
			// trigger a sound effect
			if (audio_manager_)
			{
				if (controller->buttons_pressed() & gef_SONY_CTRL_CIRCLE)
					//if (cross)
				{
					if (sfx_voice_id_ == -1)
					{
						sfx_voice_id_ = audio_manager_->PlaySample(sfx_id_, true);

						gef::VolumeInfo volume_info;
						volume_info.volume = 0.5f;
						volume_info.pan = -1.0f;

						audio_manager_->SetSampleVoiceVolumeInfo(sfx_voice_id_, volume_info);

						audio_manager_->SetSamplePitch(sfx_voice_id_, 1.5f);
					}
				}
				if (controller->buttons_pressed() & gef_SONY_CTRL_TRIANGLE)
				{
					if (sfx_voice_id_ != -1)
					{
						audio_manager_->StopPlayingSampleVoice(sfx_voice_id_);
						sfx_voice_id_ = -1;
					}
				}
			} // audio_manager_

#ifdef _WIN32 // Only on windows platforms
			  // if there is a keyboard, check the arrow keys to control the direction of the character
			gef::Keyboard* keyboard = input_manager_->keyboard();
			if (keyboard)
			{
				//const gef::KeyboardD3D11* keyboard_d3d11 = (const gef::KeyboardD3D11*)keyboard;
				float camera_speed = 10.0f;

				// keyboard input
				if (keyboard->IsKeyDown(gef::Keyboard::KC_W))
					camera_->MoveForward(frame_time * camera_speed);
				if (keyboard->IsKeyDown(gef::Keyboard::KC_S))
					camera_->MoveBackwards(frame_time * camera_speed);
				if (keyboard->IsKeyDown(gef::Keyboard::KC_A))
					camera_->MoveSideLeft(frame_time * camera_speed);
				if (keyboard->IsKeyDown(gef::Keyboard::KC_D))
					camera_->MoveSideRight(frame_time * camera_speed);
				if (keyboard->IsKeyDown(gef::Keyboard::KC_UP))
					camera_->AddPitch(frame_time, camera_speed * camera_speed);
				if (keyboard->IsKeyDown(gef::Keyboard::KC_DOWN))
					camera_->subtractPitch(frame_time, camera_speed * camera_speed);
				if (keyboard->IsKeyDown(gef::Keyboard::KC_LEFT))
					camera_->subtractYaw(frame_time, camera_speed * camera_speed);
				if (keyboard->IsKeyDown(gef::Keyboard::KC_RIGHT))
					camera_->AddYaw(frame_time, camera_speed * camera_speed);
				if (keyboard->IsKeyDown(gef::Keyboard::KC_R) || keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD8))
					camera_->MoveUp(frame_time * camera_speed);
				if (keyboard->IsKeyDown(gef::Keyboard::KC_F) || keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD2))
					camera_->MoveDown(frame_time * camera_speed);
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
				// camera's Yaw mouse controll, last variable controlls speed
				//camera_->updateYaw(960, mouse_position.x, 2);
				// camera's Pitch mouse controll, last variable controlls speed
				//camera_->updatePitch(544, mouse_position.y, 2);

				if (touch_input_d3d11->is_button_down(0))
				{
					//SetCursorPos(480, 272);	
				}

				//gef::DebugOut("Mouse position x, y: %f %f\n", mouse_position.x, mouse_position.y);
			} // touch_input (mouse)
#endif // !_WIN32
		}
		// controller

		//camera_->moveSideLeft(frame_time);
		//camera_->moveUp(frame_time);
		//camera_->update();
	}
	// input_manager_
}
// UpdateSimulation

void Game::GameUpdate(float frame_time)
{
	// get controller input data and read controller data for controler 0
	const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);

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

	// draw ground
	renderer_3d_->DrawMesh(ground_);

	// draw player
	renderer_3d_->set_override_material(&primitive_builder_->red_material());
	renderer_3d_->DrawMesh(player_);
	renderer_3d_->set_override_material(NULL);

	renderer_3d_->End();

	// start drawing sprites, but don't clear the frame buffer
	sprite_renderer_->Begin(false);
	DrawHUD();
	sprite_renderer_->End();
}

