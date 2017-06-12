#include "scene_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <maths/math_utils.h>
#include <input/sony_controller_input_manager.h>

#ifdef _WIN32
// only on windows platforms
#include <Windows.h>
#include <platform/d3d11/input/keyboard_d3d11.h>
#include <platform/d3d11/input/touch_input_manager_d3d11.h>
//#include <freeglut.h>
//#include <SFML/Window/Mouse.hpp>
//#include <SFML/Window.hpp>
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
	camera_(NULL)
{
}

void SceneApp::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	input_manager_ = gef::InputManager::Create(platform_);

	// create the renderer for draw 3D geometry
	renderer_3d_ = gef::Renderer3D::Create(platform_);

	// initialise primitive builder to make create some 3D geometry easier
	primitive_builder_ = new PrimitiveBuilder(platform_);

	InitFont();
	SetupLights();

	// initialise the physics world
	b2Vec2 gravity(0.0f, -9.81f);
	world_ = new b2World(gravity);

	InitPlayer();
	InitGround();

	camera_ = new free_camera;
}

void SceneApp::CleanUp()
{
	// destroying the physics world also destroys all the objects within it
	delete world_;
	world_ = NULL;

	delete ground_mesh_;
	ground_mesh_ = NULL;

	CleanUpFont();

	delete primitive_builder_;
	primitive_builder_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	// input manager
	delete input_manager_;
	input_manager_ = NULL;

	// clean up camera
	delete camera_;
	camera_ = NULL;

}

bool SceneApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;

	// update physics world
	float32 timeStep = 1.0f / 60.0f;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world_->Step(timeStep, velocityIterations, positionIterations);

	// update object visuals from simulation data
	player_.UpdateFromSimulation(player_body_);

	// don't have to update the ground visuals as it is static

	// collision detection // class MyContactListener other way of doing collision detection //
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
			Player* player = NULL;

			GameObject* gameObjectA = NULL;
			GameObject* gameObjectB = NULL;

			gameObjectA = (GameObject*)bodyA->GetUserData(); // cast to GameObject* pointer
			gameObjectB = (GameObject*)bodyB->GetUserData(); // cast to GameObject* pointer

			if (gameObjectA)
			{
				if (gameObjectA->type() == PLAYER)
				{
					player = (Player*)bodyA->GetUserData(); // cast to GameObject* pointer
				}
			}

			if (gameObjectB)
			{
				if (gameObjectB->type() == PLAYER)
				{
					player = (Player*)bodyB->GetUserData(); // cast to GameObject* pointer
				}
			}

			if (player)
			{
				player->DecrementHealth();
			}

			//if (bodyA->GetUserData())
			//{
			//	player = (GameObject*)bodyA->GetUserData(); // cast to GameObject* pointer
			//}
			//if (bodyB->GetUserData())
			//{
			//	player = (GameObject*)bodyB->GetUserData(); // cast to GameObject* pointer
			//}

			//if (player)
			//{
			//	player->MyCollisionResponse();
			//}

			// bouncing body
			//b2Body* player_body = NULL;

			////if (bodyA->GetType() == b2_dynamicBody) {
			//if (bodyA == player_body_) {
			//	player_body = bodyA;
			//} else {
			//	player_body = bodyB;
			//}

			//bodyA->ApplyForceToCenter(b2Vec2(0.0f, 200.0f), true);
			//bodyB->ApplyForceToCenter(b2Vec2(0.0f, 200.0f), true);
		}

		// Get next contact point
		contact = contact->GetNext();
	}

	
	// get the latest date from the input devices
	if (input_manager_)
	{
		input_manager_->Update();

		// get controller input data for all controllers
		//gef::SonyControllerInputManager* controller_manager = input_manager_->controller_input();
		const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);
		//if (controller_manager)
		//{
			// read controller data for controler 0
			//const gef::SonyController* controller = controller_manager->GetController(0);
			
			if (controller)
			{
				float camera_speed = 10.0f;
				float camera_speed_multiplier_1 = 8.0f;
				float camera_speed_multiplier_2 = 1.5f;

				// handle input
				if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS)
				{
					camera_->moveForward(timeStep);
				}
				float left_horizontal_input = controller->left_stick_x_axis();
				gef::DebugOut("left horizontal : %f\n", left_horizontal_input);
				if (controller->left_stick_x_axis() < 0)
				{
					camera_->addYaw(frame_time, -camera_speed);
				}
				if (controller->left_stick_x_axis() > 0)
				{
					camera_->addYaw(frame_time, camera_speed);
				}
			} // controller
		//} // controller_manager

#ifdef _WIN32 // Only on windows platforms
		// if there is a keyboard, check the arrow keys to control the direction of the character
		gef::Keyboard* keyboard = input_manager_->keyboard();
		if (keyboard)
		{
			//const gef::KeyboardD3D11* keyboard_d3d11 = (const gef::KeyboardD3D11*)keyboard;
			float camera_speed = 10.0f;
			float camera_speed_multiplier_1 = 8.0f;
			float camera_speed_multiplier_2 = 1.5f;

			// keyboard input
			if (keyboard->IsKeyDown(gef::Keyboard::KC_W))
				camera_->moveForward(frame_time * camera_speed);
			if (keyboard->IsKeyDown(gef::Keyboard::KC_S))
				camera_->moveBackwards(frame_time * camera_speed);
			if (keyboard->IsKeyDown(gef::Keyboard::KC_A))
				camera_->moveSideLeft(frame_time * camera_speed);
			if (keyboard->IsKeyDown(gef::Keyboard::KC_D))
				camera_->moveSideRight(frame_time * camera_speed);
			if (keyboard->IsKeyDown(gef::Keyboard::KC_UP))
				camera_->addPitch(frame_time, camera_speed * camera_speed_multiplier_1);
			if (keyboard->IsKeyDown(gef::Keyboard::KC_DOWN))
				camera_->subtractPitch(frame_time, camera_speed * camera_speed_multiplier_1);
			if (keyboard->IsKeyDown(gef::Keyboard::KC_LEFT))
				camera_->subtractYaw(frame_time, camera_speed * camera_speed_multiplier_1);
			if(keyboard->IsKeyDown(gef::Keyboard::KC_RIGHT))
				camera_->addYaw(frame_time * camera_speed, camera_speed * camera_speed_multiplier_2);
			if (keyboard->IsKeyDown(gef::Keyboard::KC_R) || keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD8))
				camera_->moveUp(frame_time * camera_speed);
			if (keyboard->IsKeyDown(gef::Keyboard::KC_F) || keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD2))
				camera_->moveDown(frame_time * camera_speed);

			/*gef::DebugOut("Yaw %f\n", camera_->getYaw());
			gef::DebugOut("Pitch %f\n", camera_->getPitch());
			gef::DebugOut("ForwardX %f\n", camera_->getForwardX());
			gef::DebugOut("ForwardY %f\n", camera_->getForwardY());
			gef::DebugOut("ForwardZ %f\n", camera_->getForwardZ());
			gef::DebugOut("LookAtX %f\n", camera_->getLookAtX());
			gef::DebugOut("LookAtY %f\n", camera_->getLookAtY());
			gef::DebugOut("LookAtZ %f\n", camera_->getLookAtZ());
			gef::DebugOut("SideX %f\n", camera_->getSideX());
			gef::DebugOut("SideY %f\n", camera_->getSideY());
			gef::DebugOut("SideZ %f\n", camera_->getSideZ());
			gef::DebugOut("UpX %f\n", camera_->getUpX());
			gef::DebugOut("UpY %f\n", camera_->getUpY());
			gef::DebugOut("UpZ %f\n", camera_->getUpZ());*/
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

			gef::DebugOut("Mouse position x, y: %f %f\n", mouse_position.x, mouse_position.y);
		} // touch_input (mouse)
#endif // !_WIN32
	} // input_manager_
	
	//camera_->moveSideLeft(frame_time);
	//camera_->moveUp(frame_time);
	camera_->update();
	
	return true;
}

void SceneApp::Render()
{
	// setup camera

	// projection
	float fov = gef::DegToRad(45.0f);
	float aspect_ratio = (float)platform_.width() / (float)platform_.height();
	gef::Matrix44 projection_matrix;
	projection_matrix = platform_.PerspectiveProjectionFov(fov, aspect_ratio, 0.1f, 100.0f);
	renderer_3d_->set_projection_matrix(projection_matrix);

	// view
	gef::Vector4 camera_eye(camera_->getPositionX(), camera_->getPositionY(), camera_->getPositionZ());
	gef::Vector4 camera_lookat(camera_->getLookAtX(), camera_->getLookAtY(), camera_->getLookAtZ());
	gef::Vector4 camera_up(camera_->getUpX(), camera_->getUpY(), camera_->getUpZ());
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
