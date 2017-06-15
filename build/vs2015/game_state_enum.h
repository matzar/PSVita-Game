#pragma once

enum GAMESTATE
{
	FRONTEND,
	GAME
}; // GAMESTATE

//__declspec(selectany) GAMESTATE gamestate_;
namespace NastyGlobals
{
	static GAMESTATE gamestate_;
}