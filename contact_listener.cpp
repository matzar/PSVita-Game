#include "contact_listener.h"
#include "player.h"

ContactListener::ContactListener()
{
}

ContactListener::~ContactListener()
{
}

void ContactListener::BeginContact(b2Contact * contact)
{
	/*b2Fixture* bodyA = contact->GetFixtureA();
	b2Fixture* bodyB = contact->GetFixtureA();*/

	//check if fixture A was a ball
	//void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	//if (bodyUserData)
	//{
	//	static_cast<Player*>(bodyUserData)->StartContact();
	//}

	////check if fixture B was a ball
	//bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
	//if (bodyUserData)
	//{
	//	static_cast<Player*>(bodyUserData)->StartContact();
	//}
}

void ContactListener::EndContact(b2Contact * contact)
{
	//check if fixture A was a ball
	//void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	//if (bodyUserData)
	//{
	//	static_cast<Player*>(bodyUserData)->EndContact();
	//}

	////check if fixture B was a ball
	//bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
	//if (bodyUserData)
	//{
	//	static_cast<Player*>(bodyUserData)->EndContact();
	//}
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);
	if (worldManifold.normal.y < -0.5f)
	{
		contact->SetEnabled(false);
	}
}
