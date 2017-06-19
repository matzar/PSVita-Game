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
	void* bodyData = contact->GetFixtureA()->GetBody()->GetUserData();

	if (bodyData)
	{
		static_cast<Player*>(bodyData)->startContact();
	}
}

void ContactListener::EndContact(b2Contact * contact)
{
}
