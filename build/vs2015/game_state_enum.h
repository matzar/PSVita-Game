#pragma once

enum GAMESTATE
{
	FRONTEND,
	GAME
}; // GAMESTATE

<<<<<<< HEAD
<<<<<<< HEAD
//__declspec(selectany) GAMESTATE gamestate_;
<<<<<<< HEAD

//static GAMESTATE gamestate_;
=======
namespace globals
{
	static GAMESTATE gamestate_;
=======
=======
>>>>>>> gamestate global

namespace globals
{
	//__declspec(selectany) GAMESTATE gamestate;
<<<<<<< HEAD



>>>>>>> __declspec(selectany) GAMESTATE gamestate = __attribute__(weak)) GAMESTATE gamestate
}>>>>>>> gamestate enum kept as global
=======
	__attribute__((weak)) GAMESTATE gamestate;
}
>>>>>>> gamestate global
