// pbrendel (c) 2021

#pragma once

#include "types.h"

#include <deque>


namespace o
{

template< typename T >
class Deque
{
public:

	typedef T DataType;
	typedef std::deque<T> Data;

	Deque()	{}
	Deque( const Deque &other )	: m_data( other.m_data ) {}
	Deque( Deque &&other ) : m_data( std::move( other.m_data ) ) {}

	inline Deque &operator=( const Deque &other );
	inline Deque &operator=( Deque &&other );

	uint GetSize() const { return static_cast<uint>( m_data.size() ); }
	bool IsEmpty() const { return m_data.empty(); }

	void Clear() { m_data.clear(); }
	void PushBack( const T &element ) {	m_data.push_back( element ); }
	void PushBack( T &&element ) { m_data.push_back( std::move( element ) ); }
	void PopFront()	{ m_data.pop_front(); }

	const T& Front() const { return m_data.front();	}

private:

	Data m_data;
};

////////////////////////////////////////////////////////////////////////////////

template< typename T > 
inline Deque<T> &Deque<T>::operator=( const Deque &other )
{
	m_data = other.m_data;
	return *this;
}


template< typename T >
inline Deque<T> &Deque<T>::operator=( Deque &&other )
{
	m_data = std::move( other.m_data );
	return *this;
}

} // namespace o