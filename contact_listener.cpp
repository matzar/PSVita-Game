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
	/*b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();*/

	auto typeA = contact->GetFixtureA()->GetBody()->GetType();
	auto typeB = contact->GetFixtureB()->GetBody()->GetType();

	// check if body A is the player
	if (typeA == PLAYER)
	//void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	{
		static_cast<Player*>(bodyUserData)->StartContact();
	}

	//check if body B is the player
	//bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
	if (typeB == PLAYER)
	{
		static_cast<Player*>(bodyUserData)->StartContact();
	}
}

void ContactListener::EndContact(b2Contact * contact)
{
	/*b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();*/

	auto typeA = contact->GetFixtureA()->GetBody()->GetType();
	auto typeB = contact->GetFixtureB()->GetBody()->GetType();

	//check if body A was the player
	//void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	if (typeA == PLAYER)
	{
		static_cast<Player*>(bodyUserData)->EndContact();
	}

	//check if body B was the player
	//bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
	if (typeB == PLAYER)
	{
		static_cast<Player*>(bodyUserData)->EndContact();
	}
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	/*b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);
	b2PointState state1[2], state2[2];
	b2GetPointStates(state1, state2, oldManifold, contact->GetManifold());
	if (state2[0] == b2_addState)
	{
		const b2Body* bodyA = contact->GetFixtureA()->GetBody();
		const b2Body* bodyB = contact->GetFixtureB()->GetBody();
		b2Vec2 point = worldManifold.points[0];
		b2Vec2 vA = bodyA->GetLinearVelocityFromWorldPoint(point);
		b2Vec2 vB = bodyB->GetLinearVelocityFromWorldPoint(point);
		float32 approachVelocity = b2Dot(vB – vA, worldManifold.normal);
		if (approachVelocity > 1.0f)
		{
		}
	}*/
}
