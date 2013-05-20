#include "model.h"
#include "material.h"
#include "primitive.h"
#include "render_system.h"
#include "tool.h"

namespace Engine
{
	Material::Material(ola::IMaterial* mat)
	{
		MaterialPtr = mat;
		Filename = gcnew System::String(mat->filename());
		Name = gcnew System::String(mat->name());;
	}

	Material::~Material()
	{
		this->!Material();
	}

	Material::!Material()
	{

	}
}