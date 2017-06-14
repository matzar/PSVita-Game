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
	Application(platform),
	sprite_renderer_(NULL),
	renderer_3d_(NULL),
	primitive_builder_(NULL),
	font_(NULL),
	world_(NULL),
	player_body_(NULL),
	input_manager_(NULL),
	audio_manager_(NULL),
	button_icon_(NULL),
	camera_(NULL),
	sfx_voice_id_(-1),
	sfx_id_(-1)
{
}

void SceneApp::Init()
{
	// initialise the game state machine
	game_state_ = FRONTEND;

	// initlalise sprite renderer
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	// initialise input manager
	input_manager_ = gef::InputManager::Create(platform_);
	// initialise audio manager
	audio_manager_ = gef::AudioManager::Create();

	InitFont();

	FrontendInit();
}

void SceneApp::CleanUp()
{
	// audio manager
	delete audio_manager_;
	audio_manager_ = NULL;

	// input manager
	delete input_manager_;
	input_manager_ = NULL;

	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	CleanUpFont();
}

bool SceneApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;

	// get the latest date from the input devices
	if (input_manager_)
	{
		input_manager_->Update();

		// get controller input data and read controller data for controler 0
		const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);
		
		if (controller)
		{
			
		}
	}

	switch (game_state_)
	{
	case FRONTEND:
	{
		FrontendUpdate(frame_time);
	}
	break;

	case GAME:
	{
		GameUpdate(frame_time);
	}
	break;
	}

	return true;
}

//bool SceneApp::Update(float frame_time)
//{
//	fps_ = 1.0f / frame_time;
//	/////////////////////////////////////////////////////////////
//	// update physics world
//	float32 timeStep = 1.0f / 60.0f;
//
//	int32 velocityIterations = 6;
//	int32 positionIterations = 2;
//
//	world_->Step(timeStep, velocityIterations, positionIterations);
//
//	// update object visuals from simulation data
//	player_.UpdateFromSimulation(player_body_);
//
//	// don't have to update the ground visuals as it is static
//
//	// collision detection // class MyContactListener other way of doing collision detection //
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
//			Player* player = NULL;
//
//			GameObject* gameObjectA = NULL;
//			GameObject* gameObjectB = NULL;
//
//			gameObjectA = (GameObject*)bodyA->GetUserData(); // cast to GameObject* pointer
//			gameObjectB = (GameObject*)bodyB->GetUserData(); // cast to GameObject* pointer
//
//			if (gameObjectA)
//			{
//				if (gameObjectA->type() == PLAYER)
//				{
//					player = (Player*)bodyA->GetUserData(); // cast to GameObject* pointer
//				}
//			}
//
//			if (gameObjectB)
//			{
//				if (gameObjectB->type() == PLAYER)
//				{
//					player = (Player*)bodyB->GetUserData(); // cast to GameObject* pointer
//				}
//			}
//
//			if (player)
//			{
//				player->DecrementHealth();
//			}
//
//			//if (bodyA->GetUserData())
//			//{
//			//	player = (GameObject*)bodyA->GetUserData(); // cast to GameObject* pointer
//			//}
//			//if (bodyB->GetUserData())
//			//{
//			//	player = (GameObject*)bodyB->GetUserData(); // cast to GameObject* pointer
//			//}
//
//			//if (player)
//			//{
//			//	player->MyCollisionResponse();
//			//}
//
//			// bouncing body
//			//b2Body* player_body = NULL;
//
//			////if (bodyA->GetType() == b2_dynamicBody) {
//			//if (bodyA == player_body_) {
//			//	player_body = bodyA;
//			//} else {
//			//	player_body = bodyB;
//			//}
//
//			//bodyA->ApplyForceToCenter(b2Vec2(0.0f, 200.0f), true);
//			//bodyB->ApplyForceToCenter(b2Vec2(0.0f, 200.0f), true);
//		}
//
//		// Get next contact point
//		contact = contact->GetNext();
//	}
//
//	
//	// get the latest date from the input devices
//	if (input_manager_)
//	{
//		input_manager_->Update();
//
//		// get controller input data for all controllers
//		//gef::SonyControllerInputManager* controller_manager = input_manager_->controller_input();
//		//get controller input data and read controller data for controler 0
//		const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);
//		//if (controller_manager)
//		//{
//			// read controller data for controler 0
//			//const gef::SonyController* controller = controller_manager->GetController(0);
//			
//			if (controller)
//			{
//				float camera_speed = 10.0f;
//
//				float left_horizontal_input = controller->left_stick_x_axis();
//				float left_vertical_input = controller->left_stick_y_axis();
//
//				float right_horizontal_input = controller->right_stick_x_axis();
//				float right_vertical_input = controller->right_stick_y_axis();
//				// left stick - pan controll
//				if (controller->left_stick_x_axis() < 0)
//				{
//					camera_->MoveSideLeft(frame_time * camera_speed);
//				}
//				if (controller->left_stick_x_axis() > 0)
//				{
//					camera_->MoveSideRight(frame_time * camera_speed);
//				}
//				if (controller->left_stick_y_axis() < 0)
//				{
//					camera_->MoveForward(frame_time * camera_speed);
//				}
//				if (controller->left_stick_y_axis() > 0)
//				{
//					camera_->MoveBackwards(frame_time * camera_speed);
//				}
//				// right stick - yaw and pitch controll
//				if (controller->right_stick_x_axis() < 0)
//				{
//					camera_->subtractYaw(frame_time, camera_speed * camera_speed);
//				}
//				if (controller->right_stick_x_axis() > 0)
//				{
//					camera_->AddYaw(frame_time, camera_speed * camera_speed);
//					
//				}
//				if (controller->right_stick_y_axis() < 0)
//				{
//					camera_->AddPitch(frame_time, camera_speed * camera_speed);
//				}
//				if (controller->right_stick_y_axis() > 0)
//				{
//					camera_->subtractPitch(frame_time, camera_speed * camera_speed);
//				}
//				// buttons
//				// handle input
//				if (controller->buttons_down() & gef_SONY_CTRL_R2)
//				{
//					camera_->MoveUp(timeStep * camera_speed);
//				}
//				if (controller->buttons_down() & gef_SONY_CTRL_L2)
//				{
//					camera_->MoveDown(timeStep * camera_speed);
//				}
//				if (controller->buttons_pressed() & gef_SONY_CTRL_SELECT)
//				{
//					camera_->DisplayCameraPosition();
//				}
//				if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS)
//				{
//					camera_->SetCameraPosition(90.0f, -12.0f, 0.0f, gef::Vector4(-8.0f, 3.5f, 0.0f));
//				}
//			} // controller
//		//} // controller_manager
//
//#ifdef _WIN32 // Only on windows platforms
//		// if there is a keyboard, check the arrow keys to control the direction of the character
//		gef::Keyboard* keyboard = input_manager_->keyboard();
//		if (keyboard)
//		{
//			//const gef::KeyboardD3D11* keyboard_d3d11 = (const gef::KeyboardD3D11*)keyboard;
//			float camera_speed = 10.0f;
//
//			// keyboard input
//			if (keyboard->IsKeyDown(gef::Keyboard::KC_W))
//				camera_->MoveForward(frame_time * camera_speed);
//			if (keyboard->IsKeyDown(gef::Keyboard::KC_S))
//				camera_->MoveBackwards(frame_time * camera_speed);
//			if (keyboard->IsKeyDown(gef::Keyboard::KC_A))
//				camera_->MoveSideLeft(frame_time * camera_speed);
//			if (keyboard->IsKeyDown(gef::Keyboard::KC_D))
//				camera_->MoveSideRight(frame_time * camera_speed);
//			if (keyboard->IsKeyDown(gef::Keyboard::KC_UP))
//				camera_->AddPitch(frame_time, camera_speed * camera_speed);
//			if (keyboard->IsKeyDown(gef::Keyboard::KC_DOWN))
//				camera_->subtractPitch(frame_time, camera_speed * camera_speed);
//			if (keyboard->IsKeyDown(gef::Keyboard::KC_LEFT))
//				camera_->subtractYaw(frame_time, camera_speed * camera_speed);
//			if(keyboard->IsKeyDown(gef::Keyboard::KC_RIGHT))
//				camera_->AddYaw(frame_time, camera_speed * camera_speed);
//			if (keyboard->IsKeyDown(gef::Keyboard::KC_R) || keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD8))
//				camera_->MoveUp(frame_time * camera_speed);
//			if (keyboard->IsKeyDown(gef::Keyboard::KC_F) || keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD2))
//				camera_->MoveDown(frame_time * camera_speed);
//		} // keyboard
//
//		// mouse input
//		const gef::TouchInputManager* touch_input = input_manager_->touch_manager();
//		if (touch_input)
//		{
//			// initialise the mouse position
//			gef::Vector2 mouse_position(0.0f, 0.0f); // left upper corner of the window
//
//			// get a pointer to the d3d11 implementation of the TouchInputManager
//			const gef::TouchInputManagerD3D11* touch_input_d3d11 = (const gef::TouchInputManagerD3D11*)touch_input;
//
//			// get the mouse position
//			mouse_position = touch_input_d3d11->mouse_position();
//			// camera's Yaw mouse controll, last variable controlls speed
//			//camera_->updateYaw(960, mouse_position.x, 2);
//			// camera's Pitch mouse controll, last variable controlls speed
//			//camera_->updatePitch(544, mouse_position.y, 2);
//			
//			if (touch_input_d3d11->is_button_down(0))
//			{
//				//SetCursorPos(480, 272);	
//			}
//
//			//gef::DebugOut("Mouse position x, y: %f %f\n", mouse_position.x, mouse_position.y);
//		} // touch_input (mouse)
//#endif // !_WIN32
//	} // input_manager_
//	
//	//camera_->moveSideLeft(frame_time);
//	//camera_->moveUp(frame_time);
//	//camera_->update();
//	
//	return true;
//}

void SceneApp::Render()
{
	switch (game_state_)
	{
	case FRONTEND:
	{
		FrontendRender();
	}
	break;

	case GAME:
	{
		GameRender();
	}
	break;
	}
}

void SceneApp::InitPlayer()
{
	// setup the mesh for the player
	player_.set_mesh(primitive_builder_->GetDefaultCubeMesh());

	// create a physics body for the player
	b2BodyDef player_body_def;
	player_body_def.type = b2_dynamicBody;
	player_body_def.position = b2Vec2(0.0f, 4.0f);

	player_body_ = world_->CreateBody(&player_body_def);

	// create the shape for the player
	b2PolygonShape player_shape;
	player_shape.SetAsBox(0.5f, 0.5f);

	// create the fixture
	b2FixtureDef player_fixture_def;
	player_fixture_def.shape = &player_shape;
	player_fixture_def.density = 1.0f;

	// create the fixture on the rigid body
	player_body_->CreateFixture(&player_fixture_def);

	// update visuals from simulation data
	player_.UpdateFromSimulation(player_body_);

	// create a connection between the rigid body and GameObject
	player_body_->SetUserData(&player_);
}

void SceneApp::InitGround()
{
	// ground dimensions
	gef::Vector4 ground_half_dimensions(5.0f, 0.5f, 0.5f);

	// setup the mesh for the ground
	ground_mesh_ = primitive_builder_->CreateBoxMesh(ground_half_dimensions);
	ground_.set_mesh(ground_mesh_);

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
	ground_.UpdateFromSimulation(ground_body_);
}

void SceneApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void SceneApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void SceneApp::DrawHUD()
{
	if(font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}

void SceneApp::SetupLights()
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

void SceneApp::UpdateSimulation(float frame_time)
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


