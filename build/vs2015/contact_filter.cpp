#include "contact_filter.h"

bool ContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{


	return (fixtureA->GetFilterData.maskBits & fixtureB->GetFilterData.categoryBits) != 0 &&
	       (fixtureA->GetFilterData.categoryBits & fixtureB->GetFilterData.maskBits) != 0;
}
