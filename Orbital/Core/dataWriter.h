// pbrendel (c) 2021

#pragma once

#include "dynArray.h"

#include <ostream>


template< typename T >
std::ostream &operator<<( std::ostream &str, const o::DynArray<T> &arr )
{
	for ( typename o::DynArray<T>::ConstIterator i = arr.Begin(); i != arr.End(); ++i )
	{
		if ( i != arr.Begin() )
		{
			str << " ";
		}
		str << *i;
	}
	return str;
}
