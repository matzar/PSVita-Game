#include "contact_listener.h"
#include <typeinfo> //typeid
#include "player.h"
#include "ground.h"

ContactListener::ContactListener()
{
}

ContactListener::~ContactListener()
{
}

//void ContactListener::BeginContact(b2Contact * contact)
//{
//	// a pointer from a physics object to an entity in the game
//	void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
//	OBJECT_TYPE typeA = ((GameObject*)(bodyUserDataA))->GetGameObjectType();
//
//	// a pointer from a physics object to an entity in the game
//	void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
//	OBJECT_TYPE typeB = ((GameObject*)(bodyUserDataA))->GetGameObjectType();
//
//	Player* player = nullptr;
//	GameObject* other_object = nullptr;
//
//	//check if body A was the player
//	if (typeA == PLAYER)
//	{
//		//void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
//		player = static_cast<Player*>(bodyUserDataA);
//		other_object = static_cast<GameObject*>(bodyUserDataB);
//		//other_object = static_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData());
//	}
//	else if (typeB == PLAYER)
//	{
//		//void* bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
//		other_object = static_cast<GameObject*>(bodyUserDataA);
//		player = static_cast<Player*>(bodyUserDataB);
//		//other_object = static_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
//	}
//
//
//	if (player)
//	{
//		player->DecrementHealth();
//
//		if (other_object)
//		{
//			if (other_object->GetGameObjectType() == GROUND)
//			{
//
//			}
//		}
//	}
//}
//
//void ContactListener::EndContact(b2Contact * contact)
//{
//	/*b2Fixture* fixtureA = contact->GetFixtureA();
//	b2Fixture* fixtureB = contact->GetFixtureB();
//
//	b2Body* bodyA = contact->GetFixtureA()->GetBody();
//	b2Body* bodyB = contact->GetFixtureB()->GetBody();*/
//
//	void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
//	OBJECT_TYPE typeA = ((GameObject*)(bodyUserDataA))->GetGameObjectType();
//
//	void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
//	OBJECT_TYPE typeB = ((GameObject*)(bodyUserDataA))->GetGameObjectType();
//
//	Player* player = nullptr;
//	GameObject* other_object = nullptr;
//
//	//check if body A was the player
//	if (typeA == PLAYER)
//	{
//		//void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
//		player = static_cast<Player*>(bodyUserDataA);
//		other_object = static_cast<GameObject*>(bodyUserDataB);
//		//other_object = static_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData());
//	}
//	else if (typeB == PLAYER)
//	{
//		//void* bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
//		player = static_cast<Player*>(bodyUserDataA);
//		other_object = static_cast<GameObject*>(bodyUserDataB);
//		//other_object = static_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
//	}
//
//
//	/*if (player)
//	{
//		//player->DecrementHealth();
//
//		if (other_object)
//		{
//			if (other_object->type() == GROUND)
//			{
//
//			}
//		}
//	}*/
//
//
//
//	////check if body B was the player
//	////bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
//	//if (typeB == GROUND)
//	//{
//	//	static_cast<Player*>(bodyUserData)->EndContact();
//	//}
//}

void ContactListener::BeginContact(b2Contact * contact)
{
	//Player* player = nullptr;
	//GameObject* game_object = nullptr;


	////check if fixture A is the player
	//void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	//OBJECT_TYPE typeA = ((GameObject*)(bodyUserDataA))->GetGameObjectType();

	//void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	//OBJECT_TYPE typeB = ((GameObject*)(bodyUserDataB))->GetGameObjectType();

	//if (typeA == PLAYER)
	//{
	//	player = static_cast<Player*>(bodyUserDataA);
	//	game_object = static_cast<GameObject*>(bodyUserDataB);
	//}

	//else if (typeB == PLAYER)
	//{
	//	player = static_cast<Player*>(bodyUserDataB);
	//	game_object = static_cast<GameObject*>(bodyUserDataA);
	//}

	//if (player)
	//{
	//	player->StartContact();
	//}
	

}

void ContactListener::EndContact(b2Contact * contact)
{
	//check if fixture A was the player
	//void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();

	//if (bodyUserData)
	//	static_cast<Player*>(bodyUserData)->EndContact();

	////check if fixture B was the player
	//bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();

	//if (bodyUserData)
	//	static_cast<Player*>(bodyUserData)->EndContact();
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


//if (typeid(bodyUserData).name() == typeid(Player*).name())
//static_cast<Player*>(bodyUserData)->StartContact();
//
////check if fixture B is the player
//bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
//if (typeid(bodyUserData).name() == typeid(Player*).name())
//static_cast<Player*>(bodyUserData)->StartContact();