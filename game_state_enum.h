#ifndef _GAME_STATE_ENUM_H
#define _GAME_STATE_ENUM_H

// gamestate enum used for game state machine and later passed to its classes by reference
enum GAMESTATE
{
	FRONTEND,
	SETTINGS,
	GAME
}; // GAMESTATE

// how to use global gamestate variable on...
// Windows and...
//__declspec(selectany) GAMESTATE gamestate;

// ...PSVita
//__attribute__((weak)) GAMESTATE gamestate;

#endif // !_GAME_STATE_ENUM_H

