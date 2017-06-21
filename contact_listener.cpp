#include "contact_listener.h"
#include "system\debug_log.h"
#include "player.h"
#include "ground.h"

ContactListener::ContactListener()
{
}

ContactListener::~ContactListener()
{
}

void ContactListener::BeginContact(b2Contact * contact)
{
	// pointers for body A and body B
	Player* player = nullptr;
	GameObject* game_object = nullptr;

	// a pointer from the physics object to the entity in the game
	void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	// get body's A type
	OBJECT_TYPE typeA = ((GameObject*)(bodyUserDataA))->GetGameObjectType();

	// a pointer from the physics object to the entity in the game
	void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	// get body's B type
	OBJECT_TYPE typeB = ((GameObject*)(bodyUserDataB))->GetGameObjectType();

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
		if (player->GetGameObjectColour() == game_object->GetGameObjectColour())
		{
			player->StartContact();
			gef::DebugOut("Same colour\n");
		}
		else
		{
			gef::DebugOut("Different colours\n");
		}
	}

	if (game_object)
	{
		if (game_object->GetGameObjectType() == GROUND)
		{

		}
	}
}

void ContactListener::EndContact(b2Contact * contact)
{
	// Pointers for body A and body B
	Player* player = nullptr;
	GameObject* game_object = nullptr;

	// a pointer from the physics object to the entity in the game
	void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	// get body's A type
	OBJECT_TYPE typeA = ((GameObject*)(bodyUserDataA))->GetGameObjectType();

	// a pointer from the physics object to the entity in the game
	void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	// get body's B type
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
	}

	if (game_object)
	{
		if (game_object->GetGameObjectType() == GROUND)
		{

		}
	}
}

//void ContactListener::BeginContact(b2Contact * contact)
//{
//	//check if fixture A is the player
//	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
//	if (bodyUserData)
//		static_cast<Player*>(bodyUserData)->StartContact();
//
//	//check if fixture B is the player
//	bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
//	if (bodyUserData)
//		static_cast<Player*>(bodyUserData)->StartContact();
//}
//
//void ContactListener::EndContact(b2Contact * contact)
//{
//	//check if fixture A was the player
//	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
//	if (bodyUserData)
//		static_cast<Player*>(bodyUserData)->EndContact();
//
//	//check if fixture B was the player
//	bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
//	if (bodyUserData)
//		static_cast<Player*>(bodyUserData)->EndContact();
//}