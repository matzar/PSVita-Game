#include "load_texture.h"
// gef headers
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/texture.h>
// std headers
#include <cstdlib>

gef::Texture* CreateTextureFromPNG(const char* png_filename, gef::Platform& platform)
{
	gef::PNGLoader png_loader;
	gef::ImageData image_data;
	gef::Texture* texture = NULL;

	// load image data from PNG file 
	png_loader.Load(png_filename, platform, image_data);

	// if the image data is valid, then create a texture from it
	if (image_data.image() != NULL)
		texture = gef::Texture::Create(platform, image_data);

	return texture;
}
