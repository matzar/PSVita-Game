#include "game.h"
// gef headers
#include <system/platform.h>
#include <system/debug_log.h>

#include <graphics/font.h>

#include <graphics/material.h>
#include <graphics/sprite_renderer.h>

#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <graphics/scene.h>

#include <maths/math_utils.h>
#include <maths/vector2.h>

#include <input/input_manager.h>
#include <input/sony_controller_input_manager.h>

#include <audio/audio_manager.h>
// extra headers
#include "load_texture.h"
#include "primitive_builder.h"
// my headers
#include "contact_listener.h"
#include "free_camera.h"
#include "camera_enum.h"
#include "game_state_enum.h"
#include "difficulty_enum.h"
#include "player.h"
#include "ground.h"
#include "pickup.h"
// box2D headers
#include <box2d/Box2D.h>

Game::Game(gef::Platform& platform, GAMESTATE* gamestate, unsigned* camera_count, unsigned* difficulty_count, gef::AudioManager* audio_manager) :
	platform_(platform),
	gamestate_(gamestate),
	camera_count_(camera_count),
	difficulty_count_(difficulty_count),
	x_velocity(5.0f), // initialize by default to EASY
	y_velocity(7.5f), // initialize by default to EASY
	font_(nullptr),
	texture_(nullptr),
	sprite_renderer_(nullptr),
	input_manager_(nullptr),
	audio_manager_(audio_manager),
	renderer_3d_(nullptr),
	primitive_builder_(nullptr),
	camera_(nullptr),
	contact_listener_(nullptr),
	world_(nullptr),
	player_(nullptr),
	model_scene_(nullptr),
	mesh_(nullptr),
	quit_(false),
	pause_(true),
	sprite_width_(190.0f),
	sprite_height(38.0f),
	player_init_x_(-4.0f),
	player_init_y_(4.0f),
	fps_(0),
	pickup_sfx_id_(-1),
	pickups_count_(0),
	number_of_grounds_(10)
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

void Game::CleanFont()
{
	delete font_;
	font_ = nullptr;
} // !CleanFont

void Game::DrawHUD()
{
	if (font_)
	{
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 5.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "PICKUP: %d", pickups_count_);
	}
} // !DrawHUD

void Game::InitTextures()
{
	texture_ = CreateTextureFromPNG("nauticalTile_160.png", platform_);
	texture_material_ = new gef::Material();
	texture_material_->set_texture(texture_);
} // !InitTextures

void Game::CleanTextures()
{
	delete texture_;
	texture_ = nullptr;
} // !ClenupTextures

void Game::InitSprites()
{
	// initlalise sprite renderer
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

	// menu box sprite
	menu_box_sprite_.set_position(platform_.width() * 0.5f, platform_.height() * 0.5f + sprite_height * 1.5f, 0.0f);
	menu_box_sprite_.set_width(sprite_width_);
	menu_box_sprite_.set_height(sprite_height);

	sprite_init_position_y_ = menu_box_sprite_.position().y();
} // !InitSprites

void Game::CleanSprites()
{
	delete sprite_renderer_;
	sprite_renderer_ = nullptr;
} // !CleanSprites

void Game::InitText()
{
	// menu text vectors init
	float height_correction = 2.0f;

	// set "RESUME" vector
	menu_text_1_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() - 0.5 * sprite_height + height_correction, -0.99f);
	// set "RESTART" vector
	menu_text_2_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() + 1.5 * sprite_height + height_correction, -0.99f);
	// set "MENU" vector
	menu_text_3_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() + sprite_height * 3.5f + height_correction, -0.99f);
} // InitText()

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

void Game::InitCamera()
{
	camera_ = new FreeCamera;
	camera_->Update();
	camera_->DisplayCameraPosition();
} // !InitCamera

void Game::CleanCamera()
{
	// clean up camera
	delete camera_;
	camera_ = nullptr;
} // !CleanCamera

void Game::InitWorld()
{
	// initialise the physics world
	b2Vec2 gravity(0.0f, -9.81f);
	world_ = new b2World(gravity);

	contact_listener_ = new ContactListener();
	// possible to pass custom ContactListener class becuase it has inherited from b2ContactListener class
	world_->SetContactListener(contact_listener_);
} // !InitWorld

void Game::CleanWorld()
{
	delete world_;
	world_ = nullptr;
} // CleanWorld

void Game::InitLevel()
{
	//// create a new scene object and read in the data from the file
	//// no meshes or materials are created yet
	//// we're not making any assumptions about what the data may be loaded in for
	//model_scene_ = new gef::Scene();
	//model_scene_->ReadSceneFromFile(platform_, "triceratop.scn");

	//// we do want to render the data stored in the scene file so lets create the materials from the material data present in the scene file
	//model_scene_->CreateMaterials(platform_);

	//// now check to see if there is any mesh data in the file, if so lets create a mesh from it
	//if (model_scene_->meshes.size() > 0)
	//	mesh_ = model_scene_->CreateMesh(platform_, model_scene_->meshes.front());

	// will be used for grounds intervals
	float interval = 3.0f;
	float32 colour_ground_x = 5.0f;
	float32 texture_ground_x = 3.0f;
	b2Vec2 start_position(0.0f, 0.0f);

	for (int i = 0; i < number_of_grounds_; ++i)
	{
		ground_.push_back( new Ground());
		//pickups_.push_back(new Pickup());

		// GOLD GROUND
		if (i == number_of_grounds_ - 1)
		{
			start_position.x -= interval / 2.0f + 0.5f;
			ground_.at(i)->InitGround(
				primitive_builder_,                   // primitive builder
				world_, 							  // world
				start_position, 		              // position
				gef::Vector4(texture_ground_x / 2.0f, 0.5f, 0.5f), 	  // ground half dimensions
				GROUND, 							  // I am...
				PLAYER | PICKUP, 					  // ..and I collide with
				1, 									  // group index (objects with the same positive index collide with each other)
				GROUND, 							  // type
				GOLD);							      // colour

			start_position.x += (texture_ground_x + interval);
		}
		// RED GROUND
		else if (i % 2 == 0) 
		{
			// change height of the ground
			if (i % 4 == 0)
				start_position.y += 2.0f;
			else
				start_position.y = 0.0f;

			b2Vec2 pickup_start_position = start_position;
			pickup_start_position.y += 1.0f;

			/*pickups_.at(i)->InitPickup(
				primitive_builder_, 
				world_, 
				pickup_start_position, 
				0.2f, 
				mesh_, 
				PICKUP, 
				PLAYER | GROUND, 
				1, 
				PICKUP);*/

			ground_.at(i)->InitGround(
				primitive_builder_,                   // primitive builder
				world_,                               // world
				start_position,                       // position
				gef::Vector4(colour_ground_x / 2.0f, 0.5f, 0.5f),       // ground half dimensions
				GROUND,                               // I am...
				PLAYER | PICKUP,                      // ..and I collide with
				1,                                    // group index (objects with the same positive index collide with each other)
				GROUND,                               // type
				BLUE);                                 // colour

			start_position.x += (colour_ground_x + interval);

		}
		// TEXTURED GROUND
		else if (i % 3 == 0)
		{
			start_position.x -= interval / 2.0f + 0.5f;
			ground_.at(i)->InitGround(
				primitive_builder_,                   // primitive builder
				world_, 							  // world
				start_position, 		              // position
				gef::Vector4(texture_ground_x / 2.0f, 0.5f, 0.5f), 	  // ground half dimensions
				GROUND, 							  // I am...
				PLAYER | PICKUP, 					  // ..and I collide with
				1, 									  // group index (objects with the same positive index collide with each other)
				GROUND, 							  // type
				NO_COL);							  // colour

			start_position.x += (texture_ground_x + interval);
		}
		else // BLUE GROUND
		{
			// change height of the ground
			if (i % 6 == 0)
				start_position.y += 2.0f;
			else
				start_position.y = 0.0f;
			
			b2Vec2 pickup_start_position = start_position;
			pickup_start_position.y += 1.0f;

			/*pickups_.at(i)->InitPickup(
				primitive_builder_,
				world_,
				pickup_start_position,
				0.2f,
				mesh_,
				PICKUP,
				PLAYER | GROUND,
				1,
				PICKUP);*/

			ground_.at(i)->InitGround(
				primitive_builder_,                   // primitive builder
				world_, 							  // world
				start_position, 		              // position
				gef::Vector4(colour_ground_x / 2.0f, 0.5f, 0.5f), 	  // ground half dimensions
				GROUND, 							  // I am...
				PLAYER | PICKUP, 					  // ..and I collide with
				1, 									  // group index (objects with the same positive index collide with each other)
				GROUND, 							  // type
				RED);								  // colour

			start_position.x += (colour_ground_x + interval);
		}
	}
} // !InitLevel

void Game::CleanLevel()
{

} // !CleanLevel

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

	// will be used for grounds intervals
	float interval = 3.0f;
	float32 colour_ground_x = 5.0f;
	float32 texture_ground_x = 3.0f;
	b2Vec2 start_position(0.0f, 0.0f);

	for (int i = 0; i < number_of_grounds_ - 1; ++i)
	{
		pickups_.push_back(new Pickup());

		// RED GROUND
		if (i % 2 == 0)
		{
			// change height of the ground
			if (i % 4 == 0)
				start_position.y += 2.0f;
			else
				start_position.y = 0.0f;

			b2Vec2 pickup_start_position = start_position;
			pickup_start_position.y += 1.0f;

			pickups_.at(i)->InitPickup(
				primitive_builder_,
				world_,
				pickup_start_position,
				0.2f,
				mesh_,
				PICKUP,
				PLAYER | GROUND,
				1,
				PICKUP);

			start_position.x += (colour_ground_x + interval);
		}
		else if (i % 3 == 0)
		{
			start_position.x -= interval / 2.0f + 0.5f;

			start_position.x += (texture_ground_x + interval);
		}
		else // BLUE GROUND
		{
			// change height of the ground
			if (i % 6 == 0)
				start_position.y += 2.0f;
			else
				start_position.y = 0.0f;

			b2Vec2 pickup_start_position = start_position;
			pickup_start_position.y += 1.0f;

			pickups_.at(i)->InitPickup(
				primitive_builder_,
				world_,
				pickup_start_position,
				0.2f,
				mesh_,
				PICKUP,
				PLAYER | GROUND,
				1,
				PICKUP);

			start_position.x += (colour_ground_x + interval);
		}
	}
} // !InitPickups

void Game::CleanPickups()
{
	//process list for deletion
	//std::vector<Pickup*>::iterator it = pickups_.begin();
	//std::vector<Pickup*>::iterator end = pickups_.end();
	//for (; it != end; ++it)
	//{
	//	//Pickup* dying_pickup = *it;

	//	//delete pickup... physics body is destroyed here
	//	//(*it)->GetBody()->GetWorld()->DestroyBody((*it)->GetBody());;
	//	if (it != pickups_.end())
	//	{
	//		pickups_.erase(it);
	//	}
	//}
} // !CleanPickups



void Game::InitPlayer()
{
	// set difficulty
	switch (*difficulty_count_)
	{
	case EASY:
	{
		x_velocity = 5.0f;
		y_velocity = 7.5f;
	} // !EASY
	break;

	case HARD:
	{
		x_velocity = 7.0f;
		y_velocity = 5.5f;
	} // !HARD
	break;
	} // !difficulty_count_

	  // create Player player_ class
	player_ = new Player(&x_velocity, &y_velocity);
	if (ground_.at(0)->GetGameObjectColour() == RED)
	{
		player_->InitPlayer(primitive_builder_, world_, b2Vec2(player_init_x_, player_init_y_), 0.5f, PLAYER, GROUND | PICKUP, 1, PLAYER, RED);
		player_->RedPlayer(true);
	}
	else
	{
		player_->InitPlayer(primitive_builder_, world_, b2Vec2(player_init_x_, player_init_y_), 0.5f, PLAYER, GROUND | PICKUP, 1, PLAYER, BLUE);
		player_->RedPlayer(false);
	}
} // !InitPlayer										   

void Game::CleanPlayer()
{
} // !CleanPlayer

void Game::InitAudio()
{
	// load audio assets
	if (audio_manager_)
	{
		// load a sound effect
		pickup_sfx_id_ = audio_manager_->LoadSample("box_collected.wav", platform_);

		// load in music
		audio_manager_->LoadMusic("music.wav", platform_);

		// play music
		audio_manager_->PlayMusic();
	}
} // !InitAudio

void Game::CleanAudio()
{
	if (audio_manager_)
	{
		// TODO
		//audio_manager_->StopMusic();
		//audio_manager_->UnloadAllSamples();
		pickup_sfx_id_ = -1;
	}
} // !CleanAudio

void Game::GameInit()
{

	// initialise input manager
	input_manager_ = gef::InputManager::Create(platform_);

	// create the renderer for draw 3D geometry
	renderer_3d_ = gef::Renderer3D::Create(platform_);

	// initialise primitive builder to make 3D geometry creation easier
	primitive_builder_ = new PrimitiveBuilder(platform_);

	SetupLights();

	InitFont();

	InitTextures();

	InitSprites();

	InitText();

	InitCamera();

	InitWorld();

	InitLevel();

	InitPickups();

	InitPlayer();

	InitAudio();
} // !GameInit

void Game::GameRelease()
{
	delete  input_manager_;
	input_manager_ = nullptr;

	delete renderer_3d_;
	renderer_3d_ = nullptr;

	primitive_builder_->CleanUp();
	delete primitive_builder_;
	primitive_builder_ = nullptr;

	// destroying the physics world also destroys all the objects within it
	// shapes and joints are destroyed in b2World::Destroy 
	// no need to explicitly delete player, ground or pickups
	// contact listener part of the world as well, no need to explicitly delete either
	CleanWorld();

	CleanCamera();

	delete model_scene_;
	model_scene_ = nullptr;

	delete mesh_;
	mesh_ = nullptr;

	pickups_.~vector();
	ground_.~vector();

	CleanFont();

	CleanTextures();

	CleanSprites();

	CleanAudio();
} // !GameRelease

void Game::SonyController(const gef::SonyController* controller)
{
	if (controller)
	{
		if (pause_)
		{
			// D-pad up
			if (controller->buttons_pressed() & gef_SONY_CTRL_UP &&
				sprite_init_position_y_ - sprite_height <= menu_box_sprite_.position().y() - sprite_height * 2.0f)
			{
				// lerp menu box sprite
				gef::Vector4 menu_box_sprite_end_position_to_lerp;
				menu_box_sprite_end_position_to_lerp.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() - sprite_height * 2.0f, 0.0);

				gef::Vector4 menu_box_sprite_lerp;
				menu_box_sprite_.set_position(menu_box_sprite_lerp.LerpReturnVector(menu_box_sprite_.position(), menu_box_sprite_end_position_to_lerp, 1.0));
			}
			// D-pad down
			if (controller->buttons_pressed() & gef_SONY_CTRL_DOWN &&
				sprite_init_position_y_ + sprite_height * 4.0f >= menu_box_sprite_.position().y() + sprite_height * 2.0f)
			{
				// lerp menu box sprite
				gef::Vector4 menu_box_sprite_end_position_to_lerp;
				menu_box_sprite_end_position_to_lerp.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() + sprite_height * 2.0f, 0.0);

				gef::Vector4 menu_box_sprite_lerp;
				menu_box_sprite_.set_position(menu_box_sprite_lerp.LerpReturnVector(menu_box_sprite_.position(), menu_box_sprite_end_position_to_lerp, 1.0));
			}
			// RESUME button press
			if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
				menu_box_sprite_.position().y() > (menu_text_1_.y() - sprite_height * 0.5f) &&
				menu_box_sprite_.position().y() < (menu_text_1_.y() + sprite_height))
			{
				pause_ = !pause_;
			}
			// RESTART BUTTON press
			if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
				menu_box_sprite_.position().y() > (menu_text_2_.y() - sprite_height * 0.5f) &&
				menu_box_sprite_.position().y() < (menu_text_2_.y() + sprite_height))
			{
				if (ground_.at(0)->GetGameObjectColour() == RED)
					player_->SetGameObjectColour(RED);
				else
					player_->SetGameObjectColour(BLUE);

				player_->ReloadPlayer();

				CleanPickups();
				InitPickups();

				pickups_count_ = 0;
				pause_ = false;
			}
			// MENU press
			if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
				menu_box_sprite_.position().y() > (menu_text_3_.y() - sprite_height * 0.5f) &&
				menu_box_sprite_.position().y() < (menu_text_3_.y() + sprite_height))
			{
				// update the current state of the game state machine
				// get the value that the gamestate points to and change it
				(*gamestate_) = FRONTEND;
			}
		} // !pause_

		// TRIANGLE press
		if (controller->buttons_pressed() & gef_SONY_CTRL_TRIANGLE)
		{
			(*camera_count_)++;

			if ((*camera_count_) >= 3)
				(*camera_count_) = 0;
		}
		// toggle pause menu
		if (controller->buttons_pressed() & gef_SONY_CTRL_SELECT)
		{
			pause_ = !pause_;
		}
	} 
} // !SonyController

void Game::UpdatePickups()
{
	//process list for deletion
	std::set<Pickup*>::iterator it = contact_listener_->dying_pickups_scheduled_for_removal_.begin();
	std::set<Pickup*>::iterator end = contact_listener_->dying_pickups_scheduled_for_removal_.end();
	for (; it != end; ++it)
	{
		/*if (audio_manager_)
		{
			audio_manager_->PlaySample(pickup_sfx_id_);
		}*/
		// play pickup sound
		if (audio_manager_)
		{
			if (pickup_sfx_id_ != -1)
			{
				int sfx_voice_id_ = audio_manager_->PlaySample(pickup_sfx_id_);

				gef::VolumeInfo volume_info;
				volume_info.volume = 0.5f;
				volume_info.pan = -1.0f;

				audio_manager_->SetSampleVoiceVolumeInfo(sfx_voice_id_, volume_info);

				audio_manager_->SetSamplePitch(sfx_voice_id_, 1.5f);

				audio_manager_->PlaySample(sfx_voice_id_);
			} // !pickup_sfx_id
		} // !audio_manager_

		Pickup* dying_pickup = *it;

		//delete pickup... physics body is destroyed here
		delete dying_pickup;

		//... and remove it from main list of pickups
		std::vector<Pickup*>::iterator it = std::find(pickups_.begin(), pickups_.end(), dying_pickup);
		if (it != pickups_.end())
		{
			pickups_.erase(it);
			pickups_.shrink_to_fit();
			pickups_count_++;
		}

	}

	//clear this list for next time
	contact_listener_->dying_pickups_scheduled_for_removal_.clear();
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
		if (contact_listener_)
		{
			// if current ground is gold - player wins
			if (contact_listener_->current_ground_->GetGameObjectColour() == GOLD)
			{
				pause_ = true;
				return;
			}
			// if current ground is different colour than the player - player loses
			if (contact_listener_->current_ground_->GetGameObjectColour() != player_->GetGameObjectColour())
			{
				player_->SetAlive(false);
				pause_ = true;
			}
		}
	}

	UpdatePickups();

	if (player_->GetBody()->GetPosition().y < -8.5f) // TODO if not working check here
	{
		player_->SetAlive(false);
		pause_ = true;
	}

	// set camera to follow the player
	if (player_->IsAlive())
	{
		switch ((*camera_count_))
		{
		case CAM1:
			camera_->SetCameraPosition(63.4f, -8.6f, 0.0f, gef::Vector4(player_->GetBody()->GetPosition().x - 9.3f, 3.3f, 7.3f));
			break;

		case CAM2:
			camera_->SetCameraPosition(73.6f, -13.6f, 0.0f, gef::Vector4(player_->GetBody()->GetPosition().x - 9.8f, 6.1f, 8.7f));
			break;

		case CAM3:
			camera_->SetCameraPosition(126.7f, -13.6f, 0.0f, gef::Vector4(player_->GetBody()->GetPosition().x -5.9f, 4.0f, -8.8f));
			break;
		}
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

		
		if (!pause_)
		{
			// player controller update and simulation update
			// must be under the same condition statement 
			// to keep the simulation consistent between restarts
			player_->PlayerController(controller);
			UpdateSimulation(frame_time);
		}

		SonyController(controller);
	} // !input_manager_

	/*gef::DebugOut("player colour: %d\n", player_->GetGameObjectColour());*/
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
		if (player_->GetGameObjectColour() == RED)
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
			else if (ground->GetGameObjectColour() == GOLD)
				renderer_3d_->set_override_material(&primitive_builder_->yellow_material());
			else
				renderer_3d_->set_override_material(texture_material_);
			// draw texture
			renderer_3d_->DrawMesh(*ground);
			// unmount texture
			renderer_3d_->set_override_material(nullptr);
		}

		// draw pickups
		for (auto pickup : pickups_)
		{
			renderer_3d_->DrawMesh(*pickup);
		}
	}
	renderer_3d_->End();
	
	// start drawing sprites, but don't clear the frame buffer
	sprite_renderer_->Begin(false);
	{
		if (pause_)
		{
			if (player_->Win())
			{
				// render "RESUME" text
				font_->RenderText(
					sprite_renderer_,
					gef::Vector4(menu_text_1_.x(), menu_text_1_.y() - 100.0f, -0.9f),
					1.0f,
					0xffffffff,
					gef::TJ_CENTRE,
					"YOU WIN");
			}
			if (!player_->Alive())
			{
				// render "RESUME" text
				font_->RenderText(
					sprite_renderer_,
					gef::Vector4(menu_text_1_.x(), menu_text_1_.y() - 100.0f, -0.9f),
					1.0f,
					0xffffffff,
					gef::TJ_CENTRE,
					"YOU LOSE");
			}

			// display frame rate
			font_->RenderText(sprite_renderer_, 
				gef::Vector4(850.0f, 510.0f, -0.9f), 
				1.0f, 
				0xffffffff, 
				gef::TJ_LEFT, 
				"FPS: %.1f", 
				fps_);

			// render "RESUME" text
			font_->RenderText(
				sprite_renderer_,
				gef::Vector4(menu_text_1_.x(), menu_text_1_.y(), -0.9f),
				1.0f,
				0xffffffff,
				gef::TJ_CENTRE,
				"RESUME");
			
			// render "RESTART" text
			font_->RenderText(
				sprite_renderer_,
				gef::Vector4(menu_text_2_.x(), menu_text_2_.y(), -0.9f),
				1.0f,
				0xffffffff,
				gef::TJ_CENTRE,
				"RESTART");

			// render "MENU" text
			font_->RenderText(
				sprite_renderer_,
				gef::Vector4(menu_text_3_.x(), menu_text_3_.y(), -0.9f),
				1.0f,
				0xffffffff,
				gef::TJ_CENTRE,
				"MENU");

			// render menu box sprite
			sprite_renderer_->DrawSprite(menu_box_sprite_);
		} // !pause_

		DrawHUD();
	}
	sprite_renderer_->End();
} // !GameRenderer