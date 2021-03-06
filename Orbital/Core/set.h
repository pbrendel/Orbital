// pbrendel (c) 2021

#pragma once

#include "types.h"
#include "assert.h"

#include <set>


namespace o
{

template< typename T >
class Set
{
public:

	typedef T DataType;
	typedef std::set<T> Data;
	typedef typename Data::iterator Iterator;
	typedef typename Data::const_iterator ConstIterator;

	Set() {}
	Set( const Set &other ) : m_data( other.m_data ) {}
	Set( Set &&other ) : m_data( std::move( other.m_data ) ) {}

	inline Set &operator=( const Set &other );
	inline Set &operator=( Set &&other );

	uint GetSize() const { return static_cast<uint>( m_data.size() ); }
	bool IsEmpty() const { return m_data.empty(); }
	bool Contains( const T &element ) const { return std::find( m_data.begin(), m_data.end(), element ) != m_data.end(); }

	void Clear() { m_data.clear(); }
	void Insert( const T &element )	{ m_data.insert( element );	}
	void Insert( T &&element ) { m_data.insert( std::move( element ) );	}

	Iterator Begin() { return m_data.begin(); }
	Iterator End() { return m_data.end(); }
	ConstIterator Begin() const	{ return m_data.begin(); }
	ConstIterator End() const { return m_data.end(); }

private:

	Data m_data;
};

////////////////////////////////////////////////////////////////////////////////

template< typename T >
inline Set<T> &Set<T>::operator=( const Set &other )
{
	m_data = other.m_data;
	return *this;
}


template< typename T >
inline Set<T> &Set<T>::operator=( Set &&other )
{
	m_data = std::move( other.m_data );
	return *this;
}

} // namespace o