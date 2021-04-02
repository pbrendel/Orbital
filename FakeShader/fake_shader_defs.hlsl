// pbrendel (c) 2019-21

#ifndef __FAKE_SHADER_DEFS_HLSL__
#define __FAKE_SHADER_DEFS_HLSL__

#if FAKE_SHADER

#define FS_CB( index, variable ) s_cb##index.variable
#define FS_LOCALIZE_CB( index, type, name ) type &name = s_cb##index.name
#define FS_ASSERT	assert
#define FS_UNROLL

#else // #if FAKE_SHADER

#define FS_CB( index, variable ) variable
#define FS_LOCALIZE_CB( index, type, name )
#define FS_ASSERT
#define FS_UNROLL	[unroll]

#endif // #else // #if FAKE_SHADER

#endif // #ifndef __FAKE_SHADER_DEFS_HLSL__