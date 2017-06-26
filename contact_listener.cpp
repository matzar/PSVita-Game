#include "contact_listener.h"
#include "system\debug_log.h"
#include "player.h"
#include "ground.h"
#include "pickup.h"

ContactListener::ContactListener()
{
}

ContactListener::~ContactListener()
{
}

void ContactListener::BeginContact(b2Contact * contact)
{
	//Player* player = nullptr;
	//GameObject* game_object = nullptr;

	//GameObject* gameObjectA = nullptr;
	//GameObject* gameObjectB = nullptr;

	//b2Body* bodyA = contact->GetFixtureA()->GetBody();
	//b2Body* bodyB = contact->GetFixtureB()->GetBody();

	//gameObjectA = (GameObject*)bodyA->GetUserData();
	//gameObjectB = (GameObject*)bodyB->GetUserData();

	//if (gameObjectA->type() == PLAYER)
	//{
	//	player = (Player*)gameObjectA;

	//}


	// pointers for body A and body B
	Player* player = nullptr;
	GameObject* game_object = nullptr;
	//Pickup* pickup = nullptr;

	// a pointer from the physics object to the entity in the game
	void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	// get body's A type
	// we cast body's A user data to GameObject pointer to be able to access its type
	uint16 typeA = ((GameObject*)(bodyUserDataA))->GetGameObjectType();

	// a pointer from the physics object to the entity in the game
	void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	// get body's B type
	// we cast body's B user data to GameObject pointer to be able to access its type
	uint16 typeB = ((GameObject*)(bodyUserDataB))->GetGameObjectType();

	// if body A is of the type PLAYER
	if (typeA == PLAYER)
	{
		player = static_cast<Player*>(bodyUserDataA);
		game_object = static_cast<GameObject*>(bodyUserDataB);
	}
	// if body B is of the type PLAYER
	else if (typeB == PLAYER)
	{
		player = static_cast<Player*>(bodyUserDataB);
		game_object = static_cast<GameObject*>(bodyUserDataA);
	}

	// collision response
	if (player)
	{
		player->StartContact();

		if (game_object->GetGameObjectType() == GROUND)
		{
			current_ground_ = (Ground*)(game_object);
		}
		//
		if (player->GetGameObjectColour() == game_object->GetGameObjectColour())
		{
			// reset jump
			player->RestartJump();
			gef::DebugOut("Same colour\n");
		}
		//
		if (game_object->GetGameObjectType() == GROUND && player->GetGameObjectColour() != game_object->GetGameObjectColour())
		{
			// destroy player
			player->DeadPlayer();

			// restart screen
			gef::DebugOut("Different colours\n");
		}

		if (game_object)
		{
			//
			if (game_object->GetGameObjectType() == PICKUP)
			{
				// set pickup to not active
				dying_pickups_scheduled_for_removal_.insert(game_object);

				gef::DebugOut("Pick this shit up!\n");
			}
		}
	}

	/*if (game_object)
	{
		if (game_object->GetGameObjectType() == GROUND)
		{

		}
	}*/
}

void ContactListener::EndContact(b2Contact * contact)
{
	// Pointers for body A and body B
	Player* player = nullptr;
	GameObject* game_object = nullptr;

	// a pointer from the physics object to the entity in the game
	void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	// get body's A type
	// we cast body's A user data to GameObject pointer to be able to access its type
	OBJECT_TYPE typeA = ((GameObject*)(bodyUserDataA))->GetGameObjectType();

	// a pointer from the physics object to the entity in the game
	void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	// get body's B type
	// we cast body's B user data to GameObject pointer to be able to access its type
	OBJECT_TYPE typeB = ((GameObject*)(bodyUserDataB))->GetGameObjectType();

	// if body A is of the type PLAYER
	if (typeA == PLAYER)
	{
		// set player pointer to the player entity in the game
		player = static_cast<Player*>(bodyUserDataA);
		game_object = static_cast<GameObject*>(bodyUserDataB);
	}
	// if body B is of the type PLAYER
	else if (typeB == PLAYER)
	{
		player = static_cast<Player*>(bodyUserDataB);
		game_object = static_cast<GameObject*>(bodyUserDataA);
	}

	// collision response
	if (player)
	{
		player->EndContact();
		//
		//if (player->GetGameObjectColour() == game_object->GetGameObjectColour())
		//{
		//	// reset jump
		//	gef::DebugOut("Same colour\n");
		//}
		//
		//if (game_object->GetGameObjectType() == GROUND && player->GetGameObjectColour() != game_object->GetGameObjectColour())
		//{
		//	// destroy player
		//	player->DeadPlayer();

		//	// restart screen
		//	gef::DebugOut("Different colours\n");
		//}

		//if (game_object)
		//{
		//	//
		//	if (game_object->GetGameObjectType() == PICKUP)
		//	{
		//		// set pickup to not active
		//		dying_pickups_scheduled_for_removal_.insert(game_object);

		//		gef::DebugOut("Pick this shit up!\n");
		//	}
		//}
	}
}