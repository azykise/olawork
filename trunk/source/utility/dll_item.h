//////////////////////////////////////////////////////////////////////////
// dll utility for open close dll, and get function from related handle
//////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDE_DLL_UTILITY_H__
#define __INCLUDE_DLL_UTILITY_H__

#include <windows.h>

// fixme: platform related : win32
class Dll_Item
{
	HINSTANCE m_handle;	//dll handle when created
public:

	//load dll when created
	Dll_Item(const wchar_t* filename)
	{
		m_handle = LoadLibraryW(filename); 
	}

	Dll_Item(const char* filename)
	{		
		m_handle = LoadLibraryA(filename); 
	}

	//release dll when destoryed
	~Dll_Item()
	{
		if(m_handle)
			FreeLibrary(m_handle);
	}

	//is dll been loaded
	bool dllLegal() { return m_handle != 0; }

	//get dll export function 
	template <class T>
	T getFunction(const char* funname)
	{
		return dllLegal() ? (T)GetProcAddress(m_handle, funname) : 0 ;
	}
};


#endif 