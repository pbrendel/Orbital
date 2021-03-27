/// pbrendel (c) 2018-21

#include "perfCounter.h"
#include "assert.h"

#include <iostream>
#include <windows.h>


PerfCounter::PerfCounter()
	: m_frequency( 0.0 )
{
	LARGE_INTEGER li;
    if ( QueryPerformanceFrequency( &li ) )
	{
		m_frequency = 1.0 / static_cast< double >( li.QuadPart );
	}
	m_counter = 0;
	Reset();
}

double PerfCounter::Reset()
{
	assert( m_frequency != 0.0 );

	LARGE_INTEGER li;
    QueryPerformanceCounter( &li );
	const CounterType counter = static_cast< CounterType >( li.QuadPart );
	const double time = ( counter - m_counter ) * m_frequency;
	m_counter = counter;
	return time;
}

double PerfCounter::ResetAndPrint( const char *msg /* = nullptr */ )
{
	const double time = Reset();
	if ( msg != nullptr )
	{
		std::cout << msg;
	}
	std::cout << time << std::endl;
	return time;
}