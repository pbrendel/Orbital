// pbrendel (c) 2019-21

#pragma once


template< typename T, typename DSTR >
class D3DHandle
{
public:
	
	D3DHandle()
		: m_ptr( nullptr )
	{}

	D3DHandle( T *ptr )
		: m_ptr( ptr )
	{}

	D3DHandle( D3DHandle &&other )
		: m_ptr( other.m_ptr )
	{
		other.m_ptr = nullptr;
	}

	~D3DHandle()
	{
		Reset();
	}
	
	D3DHandle &operator=( D3DHandle &&other )
	{
		this->m_ptr = other.m_ptr;
		other.m_ptr = nullptr;
		return *this;
	}

	constexpr T* Get() const { return m_ptr; };
	constexpr T* operator->() const { return m_ptr; };
	constexpr operator T*() const { return m_ptr; }
	constexpr operator bool() const { return m_ptr != nullptr; };

	void Reset()
	{
		DSTR dstr;
		dstr( m_ptr );
		m_ptr = nullptr;
	}

protected:

	D3DHandle( const D3DHandle &other ) {};
	D3DHandle &operator=( const D3DHandle &other ) { return *this; };

	T*	m_ptr;
};


#define D3D_DECLARE_HANDLE( HandleType, StoredType ) \
struct HandleType##_Destructor						\
{													\
	void operator()( StoredType *ptr )				\
	{												\
		if ( ptr ) ptr->Release();					\
		ptr = nullptr;								\
	}												\
};													\
typedef D3DHandle< StoredType, HandleType##_Destructor > HandleType;