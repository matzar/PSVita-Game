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
<<<<<<< HEAD
	__declspec(selectany) GAMESTATE gamestate;
	//static GAMESTATE gamestate;
>>>>>>> gamestate enum kept as global
=======

	//__attribute__((weak)) GAMESTATE gamestate;
	GAMESTATE gamestate;
	//__declspec(selectany) GAMESTATE gamestate;



>>>>>>> __declspec(selectany) GAMESTATE gamestate = __attribute__(weak)) GAMESTATE gamestate
}>>>>>>> gamestate enum kept as global
