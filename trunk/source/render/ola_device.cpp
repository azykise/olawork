#include "ola_device.h"

#include "./gl/ola_gl.h"

OlaRenderDevice* SpawnGLRenderDevice()
{
	return new OlaGLDevice();
}

OlaRenderDevice* SpawnDXRenderDevice()
{
	return 0;
}