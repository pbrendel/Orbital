// pbrendel (c) 2021

#pragma once

#include "types.h"
#include "assert.h"

#include <vector>


namespace o
{

template< typename T >
class DynArray
{
public:

	typedef T DataType;
	typedef std::vector<DataType> Data;
	typedef typename Data::iterator Iterator;
	typedef typename Data::const_iterator ConstIterator;

	DynArray() {}
	DynArray( uint size ) : m_data( size ) {}
	DynArray( const DynArray &other ) : m_data( other.m_data ) {}
	DynArray( DynArray &&other ) : m_data( std::move( other.m_data ) ) {}

	inline DynArray &operator=( const DynArray &other );
	inline DynArray &operator=( DynArray &&other );

	uint GetSize() const { return static_cast<uint>( m_data.size() ); }
	bool IsEmpty() const { return m_data.empty(); }

	void Clear() { m_data.clear(); }
	void Reserve( uint capacity ) { m_data.reserve( capacity ); }
	void Resize( uint size ) { m_data.resize( size ); }
	void PushBack( const T &elem ) { m_data.push_back( elem ); }
	void PushBack( T &&elem ) {	m_data.push_back( std::move( elem ) ); }
	void PopBack() { m_data.pop_back(); }
	void Erase( Iterator it ) {	m_data.erase( it );	}

	T &operator[]( uint index )	{ return m_data[index];	}
	const T &operator[]( uint index ) const { return m_data[index];	}

	T &Front() { return m_data.front();	}
	const T &Front() const { return m_data.front();	}
	T &Back() { return m_data.back(); }
	const T &Back() const { return m_data.back(); }

	Iterator Begin() { return m_data.begin(); }
	Iterator End() { return m_data.end(); }
	ConstIterator Begin() const { return m_data.begin(); }
	ConstIterator End() const { return m_data.end(); }

	const T *GetData() const { return m_data.data(); }

private:

	Data m_data;
};

////////////////////////////////////////////////////////////////////////////////

template< typename T >
inline DynArray<T> &DynArray<T>::operator=( const DynArray &other )
{
	m_data = other.m_data;
	return *this;
}


template< typename T >
inline DynArray<T> &DynArray<T>::operator=( DynArray &&other )
{
	m_data = std::move( other.m_data );
	return *this;
}

} // namespace o

namespace std
{

template< typename T >
inline typename o::DynArray<T>::Iterator begin( o::DynArray<T> &arr )
{
	return arr.Begin();
}


template< typename T >
inline typename o::DynArray<T>::Iterator end( o::DynArray<T> &arr )
{
	return arr.End();
}


template< typename T >
inline typename o::DynArray<T>::ConstIterator begin( const o::DynArray<T> &arr )
{
	return arr.Begin();
}


template< typename T >
inline typename o::DynArray<T>::ConstIterator end( const o::DynArray<T> &arr )
{
	return arr.End();
}

} // namespace std