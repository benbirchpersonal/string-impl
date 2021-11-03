#pragma once
#ifndef ARRAYIMPL_H
#define ARRAYIMPL_H

#include <memory>
#include <stdio.h>
#include <cassert>
#include "string-impl.h"
#include <initializer_list>
constexpr size_t MAX_ARRAYSIZE = 100;
static char ARRAY_OPERATION_BUFFER[MAX_ARRAYSIZE];


template <class _T>
class arr {
public:
	// constructors
	/*
	* defualt constructor, type specified in template argument list <here>()
	*/
	arr(size_t size);
	arr(std::initializer_list<_T> _List);
	~arr();
	// operators

	/*
	* index access
	*/
	_T& operator[](size_t i);
	// getters

	size_t count();

	_T& first();
	_T& last();



	int findFirstOf(_T search);
	int findLastOf(_T search);

	void swap(size_t index1, size_t index2);

private:
	size_t _ElemSize;
	size_t _ArraySize;
	void* _ArrayLocaton;
};



/*
* Constructor
*/
template<class _T>
inline arr<_T>::arr(size_t size)
{
	assert(size < MAX_ARRAYSIZE);
	_ElemSize = sizeof(_T);
	_ArraySize = size;
	_ArrayLocaton = malloc(sizeof(_T) * _ArraySize);
}

template<class _T>
inline arr<_T>::arr(std::initializer_list<_T> test)
{
	assert(test.size() < MAX_ARRAYSIZE);
	_ArraySize = test.size();
	_ArrayLocaton = malloc(sizeof(_T) * _ArraySize);
	for (size_t i = 0; i < _ArraySize; i++)
	{
		*((char*)_ArrayLocaton + (sizeof(_T) * i)) = _T(test.begin() + i);
	}
}


template<class _T>
inline arr<_T>::~arr()
{
	free(_ArrayLocaton);
}





template<class _T>
inline _T& arr<_T>::operator[](size_t i)
{
	assert(i < _ArraySize);
	return (_T&)*((char*)_ArrayLocaton + (sizeof(_T) * i));
}


template<class _T>
inline size_t arr<_T>::count()
{
	return _ArraySize;
}

template<class _T>
inline _T& arr<_T>::first()
{
	return (_T&)*((char*)_ArrayLocaton);
}


template<class _T>
inline _T& arr<_T>::last()
{
	return (_T&)*((char*)_ArrayLocaton + (sizeof(_T) * (_ArraySize - 1)));
}



/*
* Returns the index of the first occurance of a value in the array
* returns -1 if no occurances of item
* @param searchTerm
*/
template<class _T>
inline int arr<_T>::findFirstOf(_T search)
{
	for (size_t i = 0; i < _ArraySize; i++)
	{
		_T curr = *((char*)_ArrayLocaton + (sizeof(_T) * (i)));
		if (search == curr)
			return i;
	}

	return -1;
}


/*
* Returns the index of the last occurance of a value in the array
* returns -1 if no occurances of item
* @param searchTerm
*/
template<class _T>
inline int arr<_T>::findLastOf(_T search)
{
	for (size_t i = _ArraySize - 1; i >= 0; i--)
	{
		_T curr = *((char*)_ArrayLocaton + (sizeof(_T) * (i)));
		if (search == curr)
			return i;
	}
	return -1;
}
template<class _T>
void swapPointers(_T* p1, _T* p2) {
	_T* temp = malloc(sizeof(_T));
	memcpy(temp, p1, sizeof(_T));
	memcpy(p1, p2, sizeof(_T));
	memcpy(p2, temp, sizeof(_T));
}





template<class _T>
inline void arr<_T>::swap(size_t index1, size_t index2)
{
	_T* loc1 = (_T*)((char*)_ArrayLocaton + (sizeof(_T) * index1));
	_T* loc2 = (_T*)((char*)_ArrayLocaton + (sizeof(_T) * index2));
	_T* temp = reinterpret_cast<_T*>(malloc(sizeof(_T)));
	memcpy(temp, loc1, sizeof(_T));
	memcpy(loc1, loc2, sizeof(_T));
	memcpy(loc2, temp, sizeof(_T));
	free(temp);

}





#endif /* !ARRAYIMPL_H */
