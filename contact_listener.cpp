#include "contact_listener.h"
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
//	/*b2Fixture* fixtureA = contact->GetFixtureA();
//	b2Fixture* fixtureB = contact->GetFixtureB();
//
//	b2Body* bodyA = contact->GetFixtureA()->GetBody();
//	b2Body* bodyB = contact->GetFixtureB()->GetBody();*/
//
//	Player* player = nullptr;
//	Ground* ground = nullptr;
//
//	GameObject* gameObjectA = nullptr;
//	GameObject* gameObjectB = nullptr;
//
//	gameObjectA = (GameObject*)bodyA->GetUserData();
//	gameObjectB = (GameObject*)bodyB->GetUserData();
//
//	auto typeA = contact->GetFixtureA()->GetBody()->GetType();
//	auto typeB = contact->GetFixtureB()->GetBody()->GetType();
//
//	// check if body A is the player
//	if (typeA == PLAYER)
//	//void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
//	{
//		static_cast<Player*>(bodyUserData)->StartContact();
//	}
//
//	//check if body B is the player
//	//bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
//	if (typeB == PLAYER)
//	{
//		static_cast<Player*>(bodyUserData)->StartContact();
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
//	auto typeA = contact->GetFixtureA()->GetBody()->GetType();
//	auto typeB = contact->GetFixtureB()->GetBody()->GetType();
//
//	//check if body A was the player
//	if (typeA == PLAYER)
//	{
//		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
//		static_cast<Player*>(bodyUserData)->EndContact();
//	}
//
//	//check if body B was the player
//	//bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
//	if (typeB == PLAYER)
//	{
//		static_cast<Player*>(bodyUserData)->EndContact();
//	}
//}

void ContactListener::BeginContact(b2Contact * contact)
{
	//check if fixture A is the player
	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	if (bodyUserData)
		static_cast<Player*>(bodyUserData)->StartContact();

	//check if fixture B is the player
	bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
	if (bodyUserData)
		static_cast<Player*>(bodyUserData)->StartContact();
}

void ContactListener::EndContact(b2Contact * contact)
{
	//check if fixture A was the player
	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	if (bodyUserData)
		static_cast<Player*>(bodyUserData)->EndContact();

	//check if fixture B was the player
	bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
	if (bodyUserData)
		static_cast<Player*>(bodyUserData)->EndContact();
}
