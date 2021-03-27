// pbrendel (c) 2018-21

#pragma once

class PerfCounter
{
public:

	PerfCounter();
	
	double Reset();
	double ResetAndPrint( const char *msg = nullptr );

private:

	typedef __int64 CounterType;

	double m_frequency;
	CounterType m_counter;
};