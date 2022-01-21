#ifndef	DYNAMIC_ARRAY_IMPL
#define	DYNAMIC_ARRAY_IMPL
#include "array-impl.h"
#include "initializer-list.h"
#define	DEFAULT_ARRAYSIZE 100
_STDLIB_BEGIN

template	<class _ElemType>
class		stack : public arr<_ElemType>
{
public:
	// constructors
	constexpr						stack<_ElemType> ()					noexcept;
	constexpr						stack<_ElemType> ( size_t reservedSpace )		noexcept;
	constexpr						stack ( stack<_ElemType>& copiedArr )			noexcept;

	template <typename... UList , REQUIRES ( nonarrow_convertible<_ElemType , UList...>::value )>
	stack ( UList &&... vs )
	{
		this->_ElemSize = sizeof ( _ElemType );
		this->_ArraySize = 0;
		this->_ArrayLocation = nullptr;
		this->reserve ( sizeof...( vs ) );
		process ( forward<UList> ( vs )... );
	}


	// push/pop
	bool			push ( _ElemType item )									noexcept;
	_ElemType& pop ()												noexcept;
	bool			reserve ( size_t newSize );


private:
	_NODISCARD		bool			realloc ();
	template <typename U , typename... UList>
	void process ( U&& v , UList &&... vs )
	{
		this->push ( forward<U> ( v ) );
		process ( forward<UList> ( vs )... );
	}
	void process () {}


private:
	size_t		_ArrayAvailableSize;
};


template<class _ElemType>
constexpr stack<_ElemType>::stack () noexcept
{
	this->_ArrayLocation = ( _ElemType* ) malloc ( sizeof ( _ElemType ) * DEFAULT_ARRAYSIZE );
	this->_ArraySize = 0;
	this->_ArrayAvailableSize = DEFAULT_ARRAYSIZE;
}



template<class _ElemType>
constexpr stack<_ElemType>::stack ( size_t elements ) noexcept
{
	this->_ArrayLocation = ( _ElemType* ) malloc ( sizeof ( _ElemType ) * elements * 2 );
	this->_ArraySize = 0;

}

template<class _ElemType>
constexpr  stack<_ElemType>::stack ( stack<_ElemType>& copiedArr ) noexcept
{
	this->_ArrayLocation = ( _ElemType* ) malloc ( sizeof ( _ElemType ) * copiedArr._ArraySize );
	this->_ArraySize = copiedArr._ArraySize;
	this->_ArrayAvailableSize = copiedArr._ArrayAvailableSize;
	rsize_t copySize = copiedArr._ArraySize;

	memcpy_s (
		this->_ArrayLocation ,
		copySize ,
		copiedArr._ArrayLocation ,
		copySize
	);
}

/*
template<class _ElemType>
constexpr  stack<_ElemType>::stack ( std::initializer_list<_ElemType> _List ) noexcept
{
	this->_ArrayLocation = ( _ElemType* ) malloc ( sizeof ( _ElemType ) * _List.size () );
	this->_ArraySize = _List.size ();
	this->_ArrayAvailableSize = DEFAULT_ARRAYSIZE > _List.size () ? DEFAULT_ARRAYSIZE : _List.size () * 2;

	for ( size_t i = 0; i < _List.size (); i++ )
	{
		_ElemType temp = _ElemType ( *( _List.begin () + i ) );
		rsize_t elemSize = sizeof ( _ElemType );

		memcpy_s (
			( _ElemType* ) this->_ArrayLocation + i ,
			elemSize ,
			&temp ,
			elemSize
		);
	}
}
*/
template<class _ElemType>
inline bool stack<_ElemType>::push ( _ElemType item ) noexcept
{
	assert ( this->_ArrayLocation != nullptr );

	if ( this->_ArraySize >= ( this->_ArrayAvailableSize - 1 ) )
		if ( !realloc () )
			return false;

	_ElemType* pushloc = ( this->_ArrayLocation ) + this->_ArraySize;
	*pushloc = _ElemType ( item );

	this->_ArraySize++;
	return true;
}

template<class _ElemType>
inline _ElemType& stack<_ElemType>::pop () noexcept
{
	this->_ArraySize--;
	return ( _ElemType ) * ( this->_ArrayLocation + this->_ArraySize - 1 );
}

template<class _ElemType>
inline bool stack<_ElemType>::reserve ( size_t newSize )
{
	_ElemType* newLocation = nullptr;
	newLocation = ( _ElemType* ) ( malloc ( sizeof ( _ElemType ) * newSize ) );

	if ( newLocation == nullptr )
		return false;

	memcpy_s (
		newLocation ,
		sizeof ( _ElemType ) * this->_ArraySize ,
		this->_ArrayLocation ,
		sizeof ( _ElemType ) * this->_ArraySize
	);

	free ( this->_ArrayLocation );
	this->_ArrayLocation = newLocation;
	this->_ArrayAvailableSize = newSize;
	return true;
}

/*
* Finds a new chunk of memory which the array can be placed in.
*/
template<class _ElemType>
_NODISCARD inline bool stack<_ElemType>::realloc ()
{
	_ElemType* newLocation = nullptr;
	newLocation = ( _ElemType* ) ( malloc ( sizeof ( _ElemType ) * this->_ArrayAvailableSize * 2 ) );

	if ( newLocation == nullptr )
		newLocation = ( _ElemType* ) ( malloc ( sizeof ( _ElemType ) * this->_ArrayAvailableSize + 1 ) );

	if ( newLocation == nullptr )
		return false;

	memcpy_s (
		newLocation ,
		sizeof ( _ElemType ) * this->_ArrayAvailableSize * 2 ,
		this->_ArrayLocation ,
		sizeof ( _ElemType ) * this->_ArraySize
	);

	free ( this->_ArrayLocation );
	this->_ArrayLocation = newLocation;
	this->_ArrayAvailableSize *= 2;
	return true;
}

_STDLIB_END
#endif /* !DYNAMIC_ARRAY_IMPL*/
