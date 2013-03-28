#ifndef _OLA_INCLUDE__OLASTL__H__
#define _OLA_INCLUDE__OLASTL__H__

#include <algorithm>
#include <vector>
#include <string>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <hash_map>

template <class T>
class OlaArray:public std::vector<T>
{
public:
	OlaArray():std::vector<T>(){}
	//OlaArray(size_type _Count, const T& _Val):std::vector<T>(_Count,_Val){}

	inline bool contains(const T& _Val)
	{
		OlaArray<T>::iterator i = std::find(this->begin(),this->end(),_Val);
		return i != this->end();
	}

	inline void add(const T& _Val)
	{
		if (!contains(_Val))
			this->push_back(_Val);
	}

	inline void addRange(const OlaArray<T>& _arr)
	{
		for (OlaArray<T>::const_iterator i = _arr.begin(); i != _arr.end();++i)
			add(*i);
	}

	inline void remove(const T& _Val)
	{
		iterator i = std::remove(this->begin(),this->end(),_Val);
		this->erase(i,this->end());		
	}
};

template <class T1,class T2,class _pr = std::less<T1> >
class OlaMap:public std::map<T1,T2,_pr>
{
public:
	inline bool contains(T1& _Key)
	{
		OlaMap::iterator i = find(_Key);
		return i != end();
	}
};

#endif