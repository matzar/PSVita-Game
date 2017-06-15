#pragma once

enum GAMESTATE
{
	FRONTEND,
	GAME
}; // GAMESTATE

<<<<<<< HEAD
//__declspec(selectany) GAMESTATE gamestate_;
<<<<<<< HEAD

//static GAMESTATE gamestate_;
=======
namespace globals
{
	static GAMESTATE gamestate_;
=======

namespace globals
{
	__declspec(selectany) GAMESTATE gamestate;
	//static GAMESTATE gamestate;
>>>>>>> gamestate enum kept as global
}
>>>>>>> gamestate enum kept as global
