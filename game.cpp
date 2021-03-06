#include "game.h"
// gef headers
#include <system/platform.h>

#include <graphics/font.h>

#include <graphics/material.h>
#include <graphics/sprite_renderer.h>

#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <graphics/scene.h>

#include <maths/math_utils.h>
#include <maths/vector2.h>

#include <input/input_manager.h>
#include <input/touch_input_manager.h>
#include <input/sony_controller_input_manager.h>

#ifdef _WIN32
#include <input/keyboard.h>  
#endif // _WIN32

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
// std headers
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
// box2D headers
#include <box2d/Box2D.h>

Game::Game(gef::Platform& platform, 
           gef::InputManager* input_manager,
	       gef::AudioManager* audio_manager,
	       GAMESTATE* gamestate, 
	       unsigned* camera_count, 
	       unsigned* difficulty_count, 
	       unsigned* number_of_grounds,
	       int* pickup_sfx_id) :
	platform_(platform),
	input_manager_(input_manager),
	audio_manager_(audio_manager),
	gamestate_(gamestate),
	camera_count_(camera_count),
	difficulty_count_(difficulty_count),
	number_of_grounds_(number_of_grounds),
	pickup_sfx_id_(pickup_sfx_id),
	x_velocity(5.0f), // initialize by default to EASY
	y_velocity(7.5f), // initialize by default to EASY
	font_(nullptr),
	brown_texture_(nullptr),
	brown_texture_material_(nullptr),
	finish_texture_(nullptr),
	finish_ground_texture_material_(nullptr),
	sprite_renderer_(nullptr),
	renderer_3d_(nullptr),
	primitive_builder_(nullptr),
	camera_(nullptr),
	contact_listener_(nullptr),
	world_(nullptr),
	player_(nullptr),
	model_scene_(nullptr),
	mesh_(nullptr),
	resume_(true),
	quit_(false),
	pause_(true),
	pause_button_x_(935.0f),
	pause_button_y_(25.0f),
	pause_texture_(nullptr),
	pause_texture_material_(nullptr),
	dev_(false),
	sprite_width_(190.0f),
	sprite_height(38.0f),
	height_correction_(3.0f),
	player_init_x_(-4.0f),
	player_init_y_(4.0f),
	fps_(0),
	pickups_count_(0),
	active_touch_id_(-1)
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
		// if dev mode on...
		if (dev_)
		{
			// ...display frame rate
			font_->RenderText(sprite_renderer_,
				gef::Vector4(850.0f, 510.0f, -0.9f),
				1.0f,
				0xffffffff,
				gef::TJ_LEFT,
				"FPS: %.1f",
				fps_);
		}

		gef::Sprite title;
		title.set_texture(pause_texture_);
		title.set_position(gef::Vector4(pause_button_x_, pause_button_y_, -0.99f));
		title.set_height(38.0f);
		title.set_width(38.0f);
		sprite_renderer_->DrawSprite(title);
	}
} // !DrawHUD

void Game::InitTextures()
{
	brown_texture_ = CreateTextureFromPNG("png/nauticalTile_160.png", platform_);
	brown_texture_material_ = new gef::Material();
	brown_texture_material_->set_texture(brown_texture_);

	finish_texture_ = CreateTextureFromPNG("png/finish_line.png", platform_);
	finish_ground_texture_material_ = new gef::Material();
	finish_ground_texture_material_->set_texture(finish_texture_);

	pause_texture_ = CreateTextureFromPNG("png/pause_button.png", platform_);
	pause_texture_material_ = new gef::Material();
	pause_texture_material_->set_texture(pause_texture_);
} // !InitTextures

void Game::CleanTextures()
{
	delete brown_texture_;
	brown_texture_ = nullptr;

	delete finish_texture_;
	finish_texture_ = nullptr;

	delete pause_texture_;
	pause_texture_ = nullptr;
} // !ClenupTextures

void Game::InitSprites()
{
	// initlalise sprite renderer
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

	// menu box sprite
	menu_box_sprite_.set_position(platform_.width() * 0.5f, platform_.height() * 0.5f + sprite_height * 1.5f, 0.0f);
	menu_box_sprite_.set_width(sprite_width_);
	menu_box_sprite_.set_height(sprite_height);
} // !InitSprites

void Game::CleanSprites()
{
	delete sprite_renderer_;
	sprite_renderer_ = nullptr;
} // !CleanSprites

void Game::InitText()
{
	// set "RESUME" vector
	menu_text_1_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() - 0.5 * sprite_height + height_correction_, -0.99f);
	// set "RESTART" vector
	menu_text_2_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() + 1.5 * sprite_height + height_correction_, -0.99f);
	// set "MENU" vector
	menu_text_3_.set_value(menu_box_sprite_.position().x(), menu_box_sprite_.position().y() + sprite_height * 3.5f + height_correction_, -0.99f);
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
	/* dev function 
	camera_->DisplayCameraPosition();
	*/
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

 // loaded models can be used for creating pickups' meshes
void Game::InitModels()
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
} // !InitModels

void Game::CleanModels()
{
	delete model_scene_;
	model_scene_ = nullptr;

	delete mesh_;
	mesh_ = nullptr;
} // !CleanModels

void Game::InitLevel()
{
	// initialize random seed
	srand((unsigned)time(NULL));
	// random start colour
	int start_colour = rand() % 2;

	// will be used for grounds intervals
	float32 interval = 3.0f;
	float32 pickup_radius = 0.2f;
	float32 colour_ground_x = 5.0f;
	float32 texture_ground_x = 3.0f;
	b2Vec2 start_position(0.0f, 0.0f);

	
	// procedural level generation
	for (unsigned i = 0; i < (*number_of_grounds_); ++i)
	{
		// create new ground
		ground_.push_back(new Ground());
		// create new pickup
		pickups_.push_back(new Pickup());

		// FINISH GROUND
		if (i == (*number_of_grounds_) - 1)
		{
			start_position.x -= interval / 2.0f + 0.5f;
			ground_.at(i)->InitGround(
				primitive_builder_,                                 // primitive builder
				world_, 							                // world
				start_position, 		                            // position
				gef::Vector4(texture_ground_x / 2.0f, 0.5f, 0.5f), 	// ground half dimensions
				GROUND, 							                // I am...
				PLAYER | PICKUP, 					                // ..and I collide with
				1, 									                // group index (objects with the same positive index collide with each other)
				GROUND, 							                // type
				FINISH);							                // colour
			// update start position
			start_position.x += (texture_ground_x + interval);
		} // !FINISH GROUND

		// FIRST COLOUR
		else if (i % 2 == 0)
		{
			// change height of the ground
			if (i % 4 == 0)
				start_position.y += 2.0f;
			else
				start_position.y = 0.0f;

			// use start position to place a pickup
			b2Vec2 pickup_start_position = start_position;
			pickup_start_position.y += 1.0f;

			// if pickup models were loaded in
			if (mesh_)
			{
				pickups_.at(i)->InitPickup(
					primitive_builder_,                                       // primitive builder
					world_, 												  // world
					pickup_start_position, 									  // position
					pickup_radius, 											  // pickup radius
					mesh_, 													  // model mesh
					PICKUP, 												  // I am...
					PLAYER | GROUND, 										  // ..and I collide with
					1, 														  // group index (objects with the same positive index collide with each other)
					PICKUP);												  // type
			} 
			else
			{
				pickups_.at(i)->InitPickup(
					primitive_builder_,                     // primitive builder
					world_,									// world
					pickup_start_position,					// position
					pickup_radius,							// pickup radius					
					PICKUP,									// I am...			
					PLAYER | GROUND,						// ..and I collide with			
					1,										// group index (objects with the same positive index collide with each other)			
					PICKUP);								// type			
			}

			if (start_colour == 0)
			{
				ground_.at(i)->InitGround(
					primitive_builder_,                                   // primitive builder
					world_,                                               // world
					start_position,                                       // position
					gef::Vector4(colour_ground_x / 2.0f, 0.5f, 0.5f),     // ground half dimensions
					GROUND,                                               // I am...
					PLAYER | PICKUP,                                      // ..and I collide with
					1,                                                    // group index (objects with the same positive index collide with each other)
					GROUND,                                               // type
					BLUE);                                                // colour
			}
			else
			{
				ground_.at(i)->InitGround(
					primitive_builder_,                                   // primitive builder
					world_,                                               // world
					start_position,                                       // position
					gef::Vector4(colour_ground_x / 2.0f, 0.5f, 0.5f),     // ground half dimensions
					GROUND,                                               // I am...
					PLAYER | PICKUP,                                      // ..and I collide with
					1,                                                    // group index (objects with the same positive index collide with each other)
					GROUND,                                               // type
					RED);                                                 // colour
			}
			// update start position
			start_position.x += (colour_ground_x + interval);
		} // !FIRST COLOUR

		// TEXTURED GROUND
		else if (i % 3 == 0)
		{
			start_position.x -= interval / 2.0f + 0.5f;
			ground_.at(i)->InitGround(
				primitive_builder_,                                       // primitive builder
				world_, 							                      // world
				start_position, 		                                  // position
				gef::Vector4(texture_ground_x / 2.0f, 0.5f, 0.5f), 	      // ground half dimensions
				GROUND, 							                      // I am...
				PLAYER | PICKUP, 					                      // ..and I collide with
				1, 									                      // group index (objects with the same positive index collide with each other)
				GROUND, 							                      // type
				NO_COL);							                      // colour
			// update start position
			start_position.x += (texture_ground_x + interval);
		} // !TEXTURED GROUND

		// SECOND COLOUR
		else
		{
			// change height of the ground
			if (i % 6 == 0)
				start_position.y += 2.0f;
			else
				start_position.y = 0.0f;

			// use start position to place a pickup
			b2Vec2 pickup_start_position = start_position;
			pickup_start_position.y += 1.0f;

			// if pickup models were loaded in
			if (mesh_)
			{
				pickups_.at(i)->InitPickup(
					primitive_builder_,                                       // primitive builder
					world_, 												  // world
					pickup_start_position, 									  // position
					pickup_radius, 											  // pickup radius
					mesh_, 													  // model mesh
					PICKUP, 												  // I am...
					PLAYER | GROUND, 										  // ..and I collide with
					1, 														  // group index (objects with the same positive index collide with each other)
					PICKUP);												  // type
			}
			else
			{
				pickups_.at(i)->InitPickup(
					primitive_builder_,                     // primitive builder
					world_,									// world
					pickup_start_position,					// position
					pickup_radius,							// pickup radius					
					PICKUP,									// I am...			
					PLAYER | GROUND,						// ..and I collide with			
					1,										// group index (objects with the same positive index collide with each other)			
					PICKUP);								// type			
			}

			if (start_colour == 0)
			{
				ground_.at(i)->InitGround(
					primitive_builder_,                                   // primitive builder
					world_, 							                  // world
					start_position, 		                              // position
					gef::Vector4(colour_ground_x / 2.0f, 0.5f, 0.5f), 	  // ground half dimensions
					GROUND, 							                  // I am...
					PLAYER | PICKUP, 					                  // ..and I collide with
					1, 									                  // group index (objects with the same positive index collide with each other)
					GROUND, 							                  // type
					RED);								                  // colour
			}
			else
			{
				ground_.at(i)->InitGround(
					primitive_builder_,                                   // primitive builder
					world_, 							                  // world
					start_position, 		                              // position
					gef::Vector4(colour_ground_x / 2.0f, 0.5f, 0.5f), 	  // ground half dimensions
					GROUND, 							                  // I am...
					PLAYER | PICKUP, 					                  // ..and I collide with
					1, 									                  // group index (objects with the same positive index collide with each other)
					GROUND, 							                  // type
					BLUE);                                                // colour
			}
			// update start position
			start_position.x += (colour_ground_x + interval);
		} // !SECOND COLOUR
	} 
} // !InitLevel

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

void Game::GameInit()
{
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

	//InitModels();

	InitLevel();

	InitPlayer();
} // !GameInit

void Game::GameRelease()
{
	delete renderer_3d_;
	renderer_3d_ = nullptr;

	primitive_builder_->CleanUp();
	delete primitive_builder_;
	primitive_builder_ = nullptr;

	//CleanModels();

	// destroying the physics world also destroys all the objects within it
	// shapes and joints are destroyed in b2World::Destroy 
	// no need to explicitly delete player, ground or pickups
	// contact listener part of the world as well, no need to explicitly delete either
	CleanWorld();

	pickups_.~vector();
	ground_.~vector();

	CleanCamera();

	CleanFont();

	CleanTextures();

	CleanSprites();
} // !GameRelease

void Game::SonyController(const gef::SonyController* controller)
{
	if (controller)
	{
		if (pause_)
		{
			// menu box control when the resume button is active
			if (resume_)
			{
				// D-pad up or W key pressed
				if (controller->buttons_pressed() & gef_SONY_CTRL_UP &&
					// prevent the menu box from going higher than the top menu text
					menu_text_1_.y() <= menu_box_sprite_.position().y() - sprite_height)
				{
					// move down menu box sprite
					menu_box_sprite_.set_position(
						menu_box_sprite_.position().x(),
						menu_box_sprite_.position().y() - sprite_height * 2.0f,
						0.0f);
				}
				// D-pad down or S key pressed
				if (controller->buttons_pressed() & gef_SONY_CTRL_DOWN &&
					// prevent the menu box from going lower than the lowest menu text
					menu_text_3_.y() >= menu_box_sprite_.position().y() + sprite_height)
				{
					// move down menu box sprite
					menu_box_sprite_.set_position(
						menu_box_sprite_.position().x(),
						menu_box_sprite_.position().y() + sprite_height * 2.0f,
						0.0f);
				}
			}
			// menu box control when the resume button is NOT active
			else
			{
				// D-pad up or W key pressed
				if (controller->buttons_pressed() & gef_SONY_CTRL_UP &&
					// prevent the menu box from going higher than the top menu text
					menu_text_2_.y() <= menu_box_sprite_.position().y() - sprite_height)
				{
					// move down menu box sprite
					menu_box_sprite_.set_position(
						menu_box_sprite_.position().x(),
						menu_box_sprite_.position().y() - sprite_height * 2.0f,
						0.0f);
				}
				// D-pad down or S key pressed
				if (controller->buttons_pressed() & gef_SONY_CTRL_DOWN &&
					// prevent the menu box from going lower than the lowest menu text
					menu_text_3_.y() >= menu_box_sprite_.position().y() + sprite_height)
				{
					// move down menu box sprite
					menu_box_sprite_.set_position(
						menu_box_sprite_.position().x(),
						menu_box_sprite_.position().y() + sprite_height * 2.0f,
						0.0f);
				}
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
				pickups_count_ = 0;
				resume_ = true;

				GameRelease();
				GameInit();
				
				// future improvement - reload function
				/* 
				player_->ReloadPlayer();

				CleanPickups();
				InitPickups();

				pickups_count_ = 0;
				pause_ = false;
				*/
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
			// toggle fps display during pause
			if (controller->buttons_pressed() & gef_SONY_CTRL_CIRCLE)
			{
				dev_ = !dev_;
			}
		} // !pause_

		else
		{
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
			// toggle fps display during the game
			if (controller->buttons_pressed() & gef_SONY_CTRL_CIRCLE)
			{
				dev_ = !dev_;
			}
		}
	} 
} // !SonyController

void Game::MenuTouchInput()
{
	// there's no need for moving the menu box sprite because in this case 
	// because the functions get called quicker than the sprite moves

	// RESUME button press
	// becasue the resume button can still be touched even without it
	// being visible a check of the 'resume_' value must be performed
	if (resume_ &&
		touch_position_.y > (menu_text_1_.y() - sprite_height * 0.5f) &&
		touch_position_.y < (menu_text_1_.y() + sprite_height) &&
		touch_position_.x > (platform_.width() / 2 - sprite_width_ / 2) &&
		touch_position_.x < (platform_.width() / 2 + sprite_width_ / 2))
	{
		pause_ = !pause_;
	}

	// RESTART BUTTON press
	if (touch_position_.y > (menu_text_2_.y() - sprite_height * 0.5f) &&
		touch_position_.y < (menu_text_2_.y() + sprite_height) &&
		touch_position_.x > (platform_.width() / 2 - sprite_width_ / 2) &&
		touch_position_.x < (platform_.width() / 2 + sprite_width_ / 2))
	{
		pickups_count_ = 0;
		resume_ = true;

		GameRelease();
		GameInit();
	}

	// MENU press
	if (touch_position_.y > (menu_text_3_.y() - sprite_height * 0.5f) &&
		touch_position_.y < (menu_text_3_.y() + sprite_height) &&
		touch_position_.x > (platform_.width() / 2 - sprite_width_ / 2) &&
		touch_position_.x < (platform_.width() / 2 + sprite_width_ / 2))
	{
		// update the current state of the game state machine
		// get the value that the gamestate points to and change it
		(*gamestate_) = FRONTEND;
	}
}

void Game::TouchController(const gef::TouchInputManager* touch_input)
{
	if (touch_input && (touch_input->max_num_panels() > 0))
	{
		// get the active touches for this panel
		const gef::TouchContainer& panel_touches = touch_input->touches(0);

		// go through the touches
		for (gef::ConstTouchIterator touch = panel_touches.begin(); touch != panel_touches.end(); ++touch)
		{
			// if active touch id is -1, then we are not currently processing a touch
			if (active_touch_id_ == -1)
			{
				// check for the start of a new touch
				if (touch->type == gef::TT_NEW)
				{
					active_touch_id_ = touch->id;

					// we're just going to record the position of the touch
					touch_position_ = touch->position;

					if (resume_)
					{
						if (touch_position_.x > pause_button_x_ - 50.0f && touch_position_.y < pause_button_y_ + 50.0f)
						{
							pause_ = !pause_;
						}
					}
					// do any processing for a new touch here
					if (!pause_)
					{
						player_->PlayerTouchController(touch_position_);
					}
					else
					{
						MenuTouchInput();
					}
				}
			}
			else if (active_touch_id_ == touch->id)
			{
				// we are processing touch data with a matching id to the one we are looking for
				if (touch->type == gef::TT_ACTIVE)
				{
					// update an active touch here
					// do any processing for a new touch here
					

					// we're just going to record the position of the touch
					touch_position_ = touch->position;

				}
				else if (touch->type == gef::TT_RELEASED)
				{
					// the touch we are tracking has been released
					// perform any actions that need to happen when a touch is released here

					// we're not doing anything here apart from resetting the active touch id
					active_touch_id_ = -1;
				}
			}
		}
	}
	// reset active touch (it doesn't get fully reset in RELEASE)
	active_touch_id_ = -1;
} // !TouchController

void Game::KeyboardController(const gef::Keyboard* keyboard)
{
#ifdef _WIN32
	if (keyboard)
	{
		if (pause_)
		{
			// menu box control when the resume button is active
			if (resume_)
			{
				// D-pad up or W key pressed
				if (keyboard->IsKeyPressed(gef::Keyboard::KC_UP) &&
					// prevent the menu box from going higher than the top menu text
					menu_text_1_.y() <= menu_box_sprite_.position().y() - sprite_height)
				{
					// move down menu box sprite
					menu_box_sprite_.set_position(
						menu_box_sprite_.position().x(),
						menu_box_sprite_.position().y() - sprite_height * 2.0f,
						0.0f);
				}
				// D-pad down or S key pressed
				if (keyboard->IsKeyPressed(gef::Keyboard::KC_DOWN) &&
					// prevent the menu box from going lower than the lowest menu text
					menu_text_3_.y() >= menu_box_sprite_.position().y() + sprite_height)
				{
					// move down menu box sprite
					menu_box_sprite_.set_position(
						menu_box_sprite_.position().x(),
						menu_box_sprite_.position().y() + sprite_height * 2.0f,
						0.0f);
				}
			}
			// menu box control when the resume button is NOT active
			else
			{
				// D-pad up or W key pressed
				if (keyboard->IsKeyPressed(gef::Keyboard::KC_UP) &&
					// prevent the menu box from going higher than the top menu text
					menu_text_2_.y() <= menu_box_sprite_.position().y() - sprite_height)
				{
					// move down menu box sprite
					menu_box_sprite_.set_position(
						menu_box_sprite_.position().x(),
						menu_box_sprite_.position().y() - sprite_height * 2.0f,
						0.0f);
				}
				// D-pad down or S key pressed
				if (keyboard->IsKeyPressed(gef::Keyboard::KC_DOWN) &&
					// prevent the menu box from going lower than the lowest menu text
					menu_text_3_.y() >= menu_box_sprite_.position().y() + sprite_height)
				{
					// move down menu box sprite
					menu_box_sprite_.set_position(
						menu_box_sprite_.position().x(),
						menu_box_sprite_.position().y() + sprite_height * 2.0f,
						0.0f);
				}
			}
			// RESUME button press
			if (keyboard->IsKeyPressed(gef::Keyboard::KC_RETURN) &&
				//controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
				menu_box_sprite_.position().y() > (menu_text_1_.y() - sprite_height * 0.5f) &&
				menu_box_sprite_.position().y() < (menu_text_1_.y() + sprite_height))
			{
				pause_ = !pause_;
			}
			// RESTART BUTTON press
			if (keyboard->IsKeyPressed(gef::Keyboard::KC_RETURN) &&
				//controller->buttons_pressed() & gef_SONY_CTRL_CROSS &&
				menu_box_sprite_.position().y() > (menu_text_2_.y() - sprite_height * 0.5f) &&
				menu_box_sprite_.position().y() < (menu_text_2_.y() + sprite_height))
			{
				pickups_count_ = 0;
				resume_ = true;

				GameRelease();
				GameInit();

				// future improvement - reload function
				/*
				player_->ReloadPlayer();

				CleanPickups();
				InitPickups();

				pickups_count_ = 0;
				pause_ = false;
				*/
			}
			// MENU press
			if (keyboard->IsKeyPressed(gef::Keyboard::KC_RETURN) &&
				menu_box_sprite_.position().y() > (menu_text_3_.y() - sprite_height * 0.5f) &&
				menu_box_sprite_.position().y() < (menu_text_3_.y() + sprite_height))
			{
				// update the current state of the game state machine
				// get the value that the gamestate points to and change it
				(*gamestate_) = FRONTEND;
			}
			// toggle fps display during pause
			if (keyboard->IsKeyPressed(gef::Keyboard::KC_M))
				//if (controller->buttons_pressed() & gef_SONY_CTRL_CIRCLE)
			{
				dev_ = !dev_;
			}
		} // !pause_

		else
		{
			// TRIANGLE press
			if (keyboard->IsKeyPressed(gef::Keyboard::KC_C))
			{
				(*camera_count_)++;

				if ((*camera_count_) >= 3)
					(*camera_count_) = 0;
			}
			// toggle pause menu
			if (keyboard->IsKeyPressed(gef::Keyboard::KC_BACKSPACE) ||
				keyboard->IsKeyPressed(gef::Keyboard::KC_ESCAPE))
			{
				pause_ = !pause_;
			}
			// toggle fps display during the game
			if (keyboard->IsKeyPressed(gef::Keyboard::KC_M))
			{
				dev_ = !dev_;
			}
		}
	}
#endif // DEBUG

}

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
			if ((*pickup_sfx_id_) != -1)
			{
				int sfx_voice_id_ = audio_manager_->PlaySample(*pickup_sfx_id_);

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
			// if current ground is finish - player wins
			if (contact_listener_->current_ground_->GetGameObjectColour() == FINISH)
			{
				// set the menu box to be in a position of restart button
				menu_box_sprite_.set_position(
					platform_.width() * 0.5f,
					menu_text_2_.y() + sprite_height * 0.5f - height_correction_,
					0.0f);

				resume_ = false;
				pause_ = true;
				return;
			}
			// if current ground is different colour than the player - player loses
			if (contact_listener_->current_ground_->GetGameObjectColour() != player_->GetGameObjectColour())
			{
				menu_box_sprite_.set_position(
					platform_.width() * 0.5f, 
					menu_text_2_.y() + sprite_height * 0.5f - height_correction_,
					0.0f);

				player_->SetAlive(false);
				resume_ = false;
				pause_ = true;
			}
		}
	}

	UpdatePickups();

	if (player_->GetBody()->GetPosition().y < -8.5f)
	{
		menu_box_sprite_.set_position(platform_.width() * 0.5f, platform_.height() * 0.5f + sprite_height * 3.5f, 0.0f);
		player_->SetAlive(false);
		resume_ = false;
		pause_ = true;
	}

	// set camera to follow the player
	if (player_->IsAlive())
	{
		gef::Vector4 camera_start_yaw_pitch_roll;
		gef::Vector4 camera_start_position;
		gef::Vector4 update_yaw_pitch_roll_vector;
		gef::Vector4 update_position_vector;
		float lerp_speed = 0.2f;

		switch ((*camera_count_))
		{
		case CAM1:
			// get camera's initial yaw, pitch and roll
			camera_start_yaw_pitch_roll.set_value(camera_->GetYaw(), camera_->GetPitch(), camera_->GetRoll());
			// get camera's initial position
			camera_start_position.set_value(camera_->GetPositionVector().x(), camera_->GetPositionVector().y(), camera_->GetPositionVector().z());
			// lerp from intitial yaw, pitch and roll to a new position
			update_yaw_pitch_roll_vector.Lerp(camera_start_yaw_pitch_roll, gef::Vector4(63.4f, -8.6f, 0.0f), lerp_speed);
			// lerp from intitial position to a new position
			update_position_vector.Lerp(camera_start_position, gef::Vector4(player_->GetBody()->GetPosition().x - 9.3f, 3.3f, 7.3f), lerp_speed);
			// update camera's position
			camera_->SetCameraPosition(update_yaw_pitch_roll_vector.x(), update_yaw_pitch_roll_vector.y(), update_yaw_pitch_roll_vector.z(), update_position_vector);
			break;

		case CAM2:
			// get camera's initial yaw, pitch and roll
			camera_start_yaw_pitch_roll.set_value(camera_->GetYaw(), camera_->GetPitch(), camera_->GetRoll());
			// get camera's initial position
			camera_start_position.set_value(camera_->GetPositionVector().x(), camera_->GetPositionVector().y(), camera_->GetPositionVector().z());
			// lerp from intitial yaw, pitch and roll to a new position
			update_yaw_pitch_roll_vector.Lerp(camera_start_yaw_pitch_roll, gef::Vector4(73.6f, -13.6f, 0.0f), lerp_speed);
			// lerp from intitial position to a new position
			update_position_vector.Lerp(camera_start_position, gef::Vector4(player_->GetBody()->GetPosition().x - 9.8f, 6.1f, 8.7f), lerp_speed);
			// update camera's position
			camera_->SetCameraPosition(update_yaw_pitch_roll_vector.x(), update_yaw_pitch_roll_vector.y(), update_yaw_pitch_roll_vector.z(), update_position_vector);
			break;

		case CAM3:
			// get camera's initial yaw, pitch and roll
			camera_start_yaw_pitch_roll.set_value(camera_->GetYaw(), camera_->GetPitch(), camera_->GetRoll());
			// get camera's initial position
			camera_start_position.set_value(camera_->GetPositionVector().x(), camera_->GetPositionVector().y(), camera_->GetPositionVector().z());
			// lerp from intitial yaw, pitch and roll to a new position
			update_yaw_pitch_roll_vector.Lerp(camera_start_yaw_pitch_roll, gef::Vector4(126.7f, -13.6f, 0.0f), lerp_speed);
			// lerp from intitial position to a new position
			update_position_vector.Lerp(camera_start_position, gef::Vector4(player_->GetBody()->GetPosition().x - 5.9f, 4.0f, -8.8f), lerp_speed);
			// update camera's position
			camera_->SetCameraPosition(update_yaw_pitch_roll_vector.x(), update_yaw_pitch_roll_vector.y(), update_yaw_pitch_roll_vector.z(), update_position_vector);
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
		SonyController(controller);
		camera_->CameraController(frame_time, controller);

		// get touch input
		const gef::TouchInputManager* touch_input = input_manager_->touch_manager();
		TouchController(touch_input);

		// keyboard controller
#ifdef _WIN32
		gef::Keyboard* keyboard = input_manager_->keyboard();
		KeyboardController(keyboard);
		camera_->CameraController(frame_time, touch_input, keyboard);
#endif // _WIN32

		if (!pause_)
		{
			// player controller update and simulation update
			// must be under the same condition statement 
			// to keep the simulation consistent between restarts
			player_->PlayerController(controller);
#ifdef _WIN32
			player_->PlayerKeyboardController(keyboard);
#endif // _WIN32

			UpdateSimulation(frame_time);
		}
	} // !input_manager_
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
	// TODO skybox
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
		for (Ground* ground : ground_)
		{
			// set texture
			if (ground->GetGameObjectColour() == RED)
				renderer_3d_->set_override_material(&primitive_builder_->red_material());
			else if (ground->GetGameObjectColour() == BLUE)
				renderer_3d_->set_override_material(&primitive_builder_->blue_material());
			else if (ground->GetGameObjectColour() == FINISH)
				renderer_3d_->set_override_material(finish_ground_texture_material_);
			else
				renderer_3d_->set_override_material(brown_texture_material_);
			// draw texture
			renderer_3d_->DrawMesh(*ground);
			// unmount texture
			renderer_3d_->set_override_material(nullptr);
		}

		// draw pickups
		for (Pickup* pickup : pickups_)
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
					"YOU WIN!");
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
					"YOU LOSE!");
			}

			if (resume_)
			{
				// render "RESUME" text
				font_->RenderText(
					sprite_renderer_,
					gef::Vector4(menu_text_1_.x(), menu_text_1_.y(), -0.9f),
					1.0f,
					0xffffffff,
					gef::TJ_CENTRE,
					"RESUME");
			}
			
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