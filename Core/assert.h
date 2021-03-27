// pbrendel (c) 2019-21

#pragma once

void AssertHandler( const char *cond, const char *func, const char *file, int line );


#if _DEBUG
#define assert( cond ) if ( !( cond ) ) { AssertHandler( #cond, __FUNCTION__, __FILE__, __LINE__ ); __debugbreak(); }
#define assertex( cond, msg ) if ( !( cond ) ) { AssertHandler( msg, __FUNCTION__, __FILE__, __LINE__ ); __debugbreak(); }
#else // #if _DEBUG
#define assert( cond ) if ( !( cond ) ) AssertHandler( #cond, __FUNCTION__, __FILE__, __LINE__ );
#define assertex( cond, msg ) if ( !( cond ) ) AssertHandler( msg, __FUNCTION__, __FILE__, __LINE__ );
#endif // #else // #if _DEBUG

