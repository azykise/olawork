#pragma once

#include <stdlib.h>
#include "../../../interface/ola_engine.h"
#using "../../../bin/plugins/editor/Sharp3D.Math.dll"
using namespace Sharp3D::Math::Core;
using namespace Sharp3D::Math::Geometry3D;

using namespace ola;

/*---------------------------------------------------------------------------------*/
//-----------------------
//�йܺͷ��й�����֮���ת��
//-----------------------

namespace Tool
{
	//�ӷ��й����鿽�����йܵ�����
	//param�����йܵ����� �ӷ��й������п�����Ԫ�ظ���
	//return������һ���µ��й�����
	template<typename T>
	array<T>^ unsafeToArray(T* unsafe_ptr,int element_count)
	{	
		array<T>^ dest = gcnew array<T>(element_count);
		System::Runtime::InteropServices::Marshal::Copy((System::IntPtr)unsafe_ptr,dest,0,element_count);
		return dest;
	}

	//���й����鿽�����ݵ����й�����
	//param���й����� ���й�����
	//desc�� ���й�����������Ԫ�ض����������й�������
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