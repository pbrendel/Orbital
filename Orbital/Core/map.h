// pbrendel (c) 2021

#pragma once

#include "types.h"
#include "assert.h"

#include <map>


namespace o
{

template< typename K, typename V >
class Map
{
public:

	typedef K KeyType;
	typedef V ValueType;
	typedef std::map<K, V> Data;
	typedef typename Data::iterator Iterator;
	typedef typename Data::const_iterator ConstIterator;

	Map() {}
	Map( const Map &other ) : m_data( other.m_data ) {}
	Map( Map &&other ) : m_data( std::move( other.m_data ) ) {}

	inline Map &operator=( const Map &other );
	inline Map &operator=( Map &&other );

	uint GetSize() const { return static_cast<uint>( m_data.size() ); }
	bool IsEmpty() const { return m_data.empty(); }

	void Clear() { m_data.clear(); }
	void Insert( const K &key, const V &val ) { m_data.insert( key, val ); }
	void Insert( const K &key, V &&val ) { m_data.insert( key, std::move( val ) ); }

	V &operator[]( const K &key ) {	return m_data[key];	}
	const V &operator[]( const K &key ) const {	return m_data.at( key ); }

	Iterator Begin() { return m_data.begin(); }
	Iterator End() { return m_data.end(); }
	ConstIterator Begin() const	{ return m_data.begin(); }
	ConstIterator End() const {	return m_data.end(); }

private:

	Data m_data;
};

////////////////////////////////////////////////////////////////////////////////

template< typename K, typename V>
inline Map<K, V> &Map<K, V>::operator=( const Map &other )
{
	m_data = other.m_data;
	return *this;
}


template< typename K, typename V>
inline Map<K, V> &Map<K, V>::operator=( Map &&other )
{
	m_data = std::move( other.m_data );
	return *this;
}

} // namespace o

namespace std
{

template< typename K, typename V>
inline typename o::Map<K, V>::Iterator begin( o::Map<K, V> &map )
{
	return map.Begin();
}


template< typename K, typename V>
inline typename o::Map<K, V>::Iterator end( o::Map<K, V> &map )
{
	return map.End();
}


template< typename K, typename V>
inline typename o::Map<K, V>::ConstIterator begin( const o::Map<K, V> &map )
{
	return map.Begin();
}


template< typename K, typename V>
inline typename o::Map<K, V>::ConstIterator end( const o::Map<K, V> &map )
{
	return map.End();
}

} // namespace std