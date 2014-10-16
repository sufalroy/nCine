#include <cstdlib> // for exit()
#include "ncTextureLoaderSDL.h"
#include "ncServiceLocator.h"

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

ncTextureLoaderSDL::ncTextureLoaderSDL(const char *filename)
	: ncITextureLoader(filename), sdlSurface_(NULL)
{
	init();
}

ncTextureLoaderSDL::ncTextureLoaderSDL(ncIFile *fileHandle)
	: ncITextureLoader(fileHandle), sdlSurface_(NULL)
{
	init();
}

///////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////////////////////////

void ncTextureLoaderSDL::init()
{
	ncServiceLocator::logger().write(ncILogger::LOG_INFO, (const char *)"ncTextureLoaderSDL::init - Loading \"%s\"", fileHandle_->filename());
	sdlSurface_ = (SDL_Surface *)IMG_Load(fileHandle_->filename());
	if (!sdlSurface_)
	{
		ncServiceLocator::logger().write(ncILogger::LOG_FATAL, (const char *)"ncTextureLoaderSDL::init - Cannot load \"%s\"", fileHandle_->filename());
		exit(-1);
	}

	bpp_ = sdlSurface_->format->BitsPerPixel;
	if (bpp_ == 32)
	{
		texFormat_ = ncTextureFormat(GL_RGBA8);
	}
	else if (bpp_  == 24)
	{
		texFormat_ = ncTextureFormat(GL_RGB8);
	}
	else if (bpp_ == 8)
	{
		texFormat_ = ncTextureFormat(GL_ALPHA8);
	}
	else
	{
		ncServiceLocator::logger().write(ncILogger::LOG_FATAL, (const char *)"ncTextureLoaderSDL::init - Not a true color or alpha image: %d", bpp_);
		exit(-1);
	}

	if (sdlSurface_->format->Rmask != 0x000000ff)
	{
		texFormat_.bgrFormat();
	}

	width_ = sdlSurface_->w;
	height_ = sdlSurface_->h;
	mipMapCount_ = 1; // No MIP Mapping
	pixels_ = static_cast<GLubyte*>(sdlSurface_->pixels);
}

ncTextureLoaderSDL::~ncTextureLoaderSDL()
{
	if (sdlSurface_)
	{
		SDL_FreeSurface(sdlSurface_);
		pixels_ = NULL;
	}
}
