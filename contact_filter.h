#ifndef _CONTACT_FILTER_H
#define _CONTACT_FILTER_H

// box2d headers
#include "box2d\Box2D.h"

class ContactFilter : public b2ContactFilter
{
public:
	bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
};

#endif // !_CONTACT_FILTER_H
