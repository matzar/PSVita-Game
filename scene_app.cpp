#include "scene_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <maths/math_utils.h>
#include <input/sony_controller_input_manager.h>
#include <input/keyboard.h>


SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(nullptr),
	renderer_3d_(nullptr),
	primitive_builder_(nullptr),
	font_(nullptr),
	world_(nullptr),
	player_body_(nullptr),
	input_manager_(nullptr),
	camera_(nullptr)
{
}

void SceneApp::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

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
	world_ = nullptr;

	delete ground_mesh_;
	ground_mesh_ = nullptr;

	CleanUpFont();

	delete primitive_builder_;
	primitive_builder_ = nullptr;

	delete renderer_3d_;
	renderer_3d_ = nullptr;

	delete sprite_renderer_;
	sprite_renderer_ = nullptr;

	// input manager
	delete input_manager_;
	input_manager_ = NULL;

	// clean up camera
	delete camera_;
	camera_ = nullptr;

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
			Player* player = nullptr;

			GameObject* gameObjectA = nullptr;
			GameObject* gameObjectB = nullptr;

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
		gef::SonyControllerInputManager* controller_input = input_manager_->controller_input();
		if (controller_input)
		{
			// read controller data for controler 0
			const gef::SonyController* controller = controller_input->GetController(0);
			
			// handle input
			if (controller->buttons_pressed() & gef_SONY_CTRL_UP)
			{
				camera_->moveForward(frame_time);
			}

			//if (controller->left_stick_y_axis < 0)
			//{

			//}
		}

		// if there is a keyboard, check the arrow keys to control the direction of the character
		gef::Keyboard* keyboard = input_manager_->keyboard();
		//float speed = 10;
		if (keyboard)
		{
			if (keyboard->IsKeyDown(gef::Keyboard::KC_UP))
			{
				//speed += 1.0f;
				gef::DebugOut("Up arraw press.\n");
				camera_->moveForward(frame_time);
			}

			camera_->update();
			//else if (keyboard->IsKeyDown(gef::Keyboard::KC_LEFT))
			//	forward = -1.0f;
		}

	}
	
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
	renderer_3d_->set_override_material(nullptr);

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
	font_ = nullptr;
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
