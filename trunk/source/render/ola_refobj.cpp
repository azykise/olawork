#include "ola_refobj.h"

#include <assert.h>
IRefCounter::~IRefCounter()
{
	assert(mRefCounter == 0 && "reference count is not 0!");
}
