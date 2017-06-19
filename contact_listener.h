#ifndef _CONTACT_LISTENER_H
#define _CONTACT_LISTENER_H

#include <box2d/Box2D.h>

class ContactListener : public b2ContactListener
{
public:
	ContactListener();
	~ContactListener();

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
private:

};

#endif // !_CONTACT_LISTENER_H

