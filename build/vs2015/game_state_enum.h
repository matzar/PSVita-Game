#pragma once

enum GAMESTATE
{
	FRONTEND,
	GAME
}; // GAMESTATE

//__declspec(selectany) GAMESTATE gamestate_;
namespace globals
{
	static GAMESTATE gamestate_;
}