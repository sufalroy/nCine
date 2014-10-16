#include "ncIGfxDevice.h"
#include "ncServiceLocator.h"

#if defined(__ANDROID__)
	#include <GLES/gl.h>
	#include <GLES/glext.h>
#elif defined(WITH_GLEW)
	#include <GL/glew.h>
#elif defined(__APPLE__)
	#include <OpenGL/gl.h>
	#include <OpenGL/glext.h>
#else
	#include <GL/gl.h>
	#include <GL/glext.h>
#endif

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

/// Clears the screen
void ncIGfxDevice::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

///////////////////////////////////////////////////////////
// PROTECTED FUNCTIONS
///////////////////////////////////////////////////////////

/// Initilizes starting OpenGL state
void ncIGfxDevice::initGL()
{
	ncGfxCapabilities& gfxCaps = const_cast<ncGfxCapabilities&>(ncServiceLocator::gfxCapabilities());
	gfxCaps.init();

	glDisable(GL_DITHER);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef WITH_DEPTH_TEST
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glAlphaFunc(GL_GREATER, 0.0f);
#endif

	glMatrixMode(GL_PROJECTION);
#ifndef __ANDROID__
	glOrtho(0, width_, 0, height_, -1, 1);
#else
	glOrthof(0, width_, 0, height_, -1, 1);
#endif
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
