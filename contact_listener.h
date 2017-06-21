#ifndef _CONTACT_LISTENER_H
#define _CONTACT_LISTENER_H

#include <box2d/Box2D.h>

class ContactListener : public b2ContactListener
{
public:
	ContactListener();
	~ContactListener();

	// virtual functions from b2ContactListener to be overwritten 
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
};

#endif // !_CONTACT_LISTENER_H

