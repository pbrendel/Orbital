// pbrendel (c) 2019-21

#include "assert.h"

#include <iostream>


void AssertHandler( const char *cond, const char *func, const char *file, int line )
{
	std::cerr<<"Assertion failed: '"<<cond<<"' while calling '"<<func<<"'."<<std::endl;
	std::cerr<<file<<": "<<line<<std::endl;
#if !_DEBUG
	exit( 0 );
#endif // #if !_DEBUG
}