#pragma once

#include "pre_define.h"

namespace Engine
{
	ref class Material
	{
	public:
		Material(ola::IMaterial* mat);
		~Material();
		!Material();

		System::String^ Filename;
		System::String^ Name;

		ola::IMaterial* MaterialPtr;
	};
}