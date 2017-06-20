#include "contact_listener.h"
// my headers
#include "player.h"
#include "object_type_enum.h"
// std headers
#include <typeinfo>	 // for 'typeid'

ContactListener::ContactListener()
{
}

ContactListener::~ContactListener()
{
}

void ContactListener::BeginContact(b2Contact * contact)
{

	//check if fixture A was a ball
	//if (bodyUserData)
	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	if (contact->GetFixtureA()->GetFilterData().groupIndex == PLAYER)
	{
		static_cast<Player*>(bodyUserData)->StartContact();
	}

	//check if fixture B was a ball
	//if (bodyUserData)
	bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
	if (contact->GetFixtureA()->GetFilterData().groupIndex == PLAYER)
	{
		static_cast<Player*>(bodyUserData)->StartContact();
	}
}

void ContactListener::EndContact(b2Contact * contact)
{
	//check if fixture A was a ball
	//if (bodyUserData)
	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	if (contact->GetFixtureA()->GetFilterData().groupIndex == PLAYER)
	{
		static_cast<Player*>(bodyUserData)->EndContact();
	}

	//check if fixture B was a ball
	bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
	//if (bodyUserData)
	if (contact->GetFixtureA()->GetFilterData().groupIndex == PLAYER)
	{
		static_cast<Player*>(bodyUserData)->EndContact();
	}
}
