// pbrendel (c) 2021

#pragma once

#include "assert.h"
#include "types.h"


namespace o
{

template< typename T>
class Ptr
{
public:

	Ptr() : m_ptr( nullptr ) {}
	Ptr( T *ptr ) : m_ptr( ptr ) {}
	Ptr( Ptr &&other ) : m_ptr( other.m_ptr ) { other.m_ptr = nullptr; }
	~Ptr() { delete m_ptr; }

	inline Ptr &operator=( Ptr &&other );
	inline Ptr &operator=( T *ptr );

	T *Get() { return m_ptr; }
	const T *Get() const { return m_ptr; }

	inline T &operator*();
	inline const T &operator*() const;
	inline T *operator->();
	inline const T *operator->() const;

	inline T &operator[]( uint index );
	inline const T &operator[]( uint index ) const;

	operator bool() const { return m_ptr != nullptr; }

private:

	T *m_ptr;
};

////////////////////////////////////////////////////////////////////////////////

template< typename T >
inline Ptr<T> &Ptr<T>::operator=( Ptr &&other )
{
	delete m_ptr;
	m_ptr = other.m_ptr;
	other.m_ptr = nullptr;
	return *this;
}


template< typename T >
inline Ptr<T> &Ptr<T>::operator=( T *ptr )
{
	delete m_ptr;
	m_ptr = ptr;
	return *this;
}


template< typename T >
inline T &Ptr<T>::operator*()
{
	assert( m_ptr != nullptr );
	return *m_ptr;
}


template< typename T >
inline const T &Ptr<T>::operator*() const
{
	assert( m_ptr != nullptr );
	return *m_ptr;
}


template< typename T >
inline T *Ptr<T>::operator->()
{
	assert( m_ptr != nullptr );
	return m_ptr;
}


template< typename T >
inline const T *Ptr<T>::operator->() const
{
	assert( m_ptr != nullptr );
	return m_ptr;
}


template< typename T >
inline T &Ptr<T>::operator[]( uint index )
{
	assert( m_ptr != nullptr );
	return m_ptr[index];
}


template< typename T >
inline const T &Ptr<T>::operator[]( uint index ) const
{
	assert( m_ptr != nullptr );
	return m_ptr[index];
}

} // namespace o
