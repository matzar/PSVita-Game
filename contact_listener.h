#ifndef _CONTACT_LISTENER_H
#define _CONTACT_LISTENER_H

#include <box2d/Box2D.h>
#include <set>

class GameObject;

class ContactListener : public b2ContactListener
{
public:
	ContactListener();
	~ContactListener();

	// virtual functions from b2ContactListener to be overwritten 
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	//virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

	std::set<GameObject*> pickups_scheduled_for_removal_;
};

#endif // !_CONTACT_LISTENER_H

