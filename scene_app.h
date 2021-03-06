#ifndef _SCENE_APP_H
#define _SCENE_APP_H

// gef headers
#include <system/application.h>
// my headers
#include "game_state_enum.h"

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class InputManager;
	class AudioManager;
} // !gef
// My class forward declarations
class Frontend;
class Settings;
class Game;

class SceneApp : public gef::Application
{
public:
	SceneApp(gef::Platform& platform);

	// Application virtual template classes - need to be defined
	virtual void Init();
	virtual void CleanUp();
	virtual bool Update(float frame_time);
	virtual void Render();
private:
	GAMESTATE gamestate_;

	// camera state holder
	unsigned camera_count_;

	// difficulty
	unsigned difficulty_count_;
	// number of grounds in the level
	unsigned number_of_grounds_;

	// input manager
	gef::InputManager* input_manager_;

	// audio manager
	gef::AudioManager* audio_manager_;
	// audio variables
	int pickup_sfx_id_;

	// classes pointers
	Frontend* frontend_;
	Settings* settings_;
	Game* game_;

	void InitAudio();
	void CleanAudio();
}; // !SceneApp

#endif // !_SCENE_APP_H