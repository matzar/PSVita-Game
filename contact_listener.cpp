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
	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();

	if (bodyUserData)
	{
		static_cast<Player*>(bodyUserData)->startContact();
	}
}

void ContactListener::EndContact(b2Contact * contact)
{
	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();

	if (bodyUserData)
	{
		static_cast<Player*>(bodyUserData)->endContact();
	}
}
