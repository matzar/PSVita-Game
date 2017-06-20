#pragma once
#include "box2d\Box2D.h"

class ContactFilter : public b2ContactFilter
{
	bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
};
