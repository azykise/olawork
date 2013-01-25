#include "ola_device.h"

#include "./gl/ola_gl.h"

unsigned int G_DEVICE_ENVIRONMENT_DATA[128];

OlaRenderDevice* SpawnGLRenderDevice()
{
	return new OlaGLDevice();
}

OlaRenderDevice* SpawnDXRenderDevice()
{
	return 0;
}
