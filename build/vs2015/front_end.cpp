#include "front_end.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <graphics/sprite.h>
#include <maths/math_utils.h>
#include <input/sony_controller_input_manager.h>
#include "load_texture.h"

#ifdef _WIN32
// only on windows platforms
#include <Windows.h>
#include <platform/d3d11/input/keyboard_d3d11.h>
#include <platform/d3d11/input/touch_input_manager_d3d11.h>
//#include <freeglut.h>
#endif 

FrontEnd::FrontEnd()
{
}


FrontEnd::~FrontEnd()
{
}
