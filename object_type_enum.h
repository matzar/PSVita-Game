#ifndef _OBJECT_TYPE_ENUM_H
#define _OBJECT_TYPE_ENUM_H
// enum to be used with game objects to specify
// what type they are
// and what type they collide with
enum OBJECT_TYPE
{
	GROUND   = 0x0001,
	PLAYER   = 0x0002,
	PICKUP   = 0x0003,
	NO_TYPE  = 0x0004
};

#endif // !_OBJECT_TYPE_ENUM_H