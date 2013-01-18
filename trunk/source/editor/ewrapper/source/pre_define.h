#pragma once

#include <stdlib.h>
#include "../../../interface/ola_engine.h"
#using "../../../bin/plugins/editor/Sharp3D.Math.dll"
using namespace Sharp3D::Math::Core;
using namespace Sharp3D::Math::Geometry3D;

using namespace ola;

/*---------------------------------------------------------------------------------*/
//-----------------------
//托管和非托管数组之间的转换
//-----------------------

namespace Tool
{
	//从非托管数组拷贝到托管的数组
	//param：非托管的数组 从非托管数组中拷贝的元素个数
	//return：返回一个新的托管数组
	template<typename T>
	array<T>^ unsafeToArray(T* unsafe_ptr,int element_count)
	{	
		array<T>^ dest = gcnew array<T>(element_count);
		System::Runtime::InteropServices::Marshal::Copy((System::IntPtr)unsafe_ptr,dest,0,element_count);
		return dest;
	}

	//从托管数组拷贝内容到非托管数组
	//param：托管数组 非托管数组
	//desc： 将托管数组中所有元素都拷贝到非托管数组中
	template<typename T>
	void arrayToUnSafe(T _array, void* unsafe_ptr)
	{
		System::Runtime::InteropServices::Marshal::Copy(_array,0,(System::IntPtr)unsafe_ptr,_array->Length);
	}

	inline char* getStringPtr(System::String^ str)
	{
		return static_cast<char*>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str).ToPointer());
	}
}


/*---------------------------------------------------------------------------------*/