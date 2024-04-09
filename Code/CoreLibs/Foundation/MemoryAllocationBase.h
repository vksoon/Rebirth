//------------------------------------------------------------
// Copyright (c) 2021 Eric Kim								  
//------------------------------------------------------------
#pragma once

#include "PlatformDefines.h"
#include "StandardHeaders.h"

RB_NAMESPACE_BEGIN

/* pointer type (64/32 platforms are different) */
#if defined(WIN32)
typedef unsigned __int32 uptr_t;
#else
typedef unsigned __int64 uptr_t;
#endif

static std::atomic<uint32_t> number_of_heap_allocations{ 0 };
static std::atomic<size_t> size_of_heap_allocations{ 0 };

class MemoryAllocationBase
{
public:
	static void* Allocate(size_t size
#if RB_MEM_USE_TRACE == 1
		, const char* source, uint32_t line, uint32_t category)
#else 
		)
#endif
	{
		void* ptr = ::malloc(size + sizeof(size_t));
		if (!ptr)
			return nullptr;

		*((size_t*)ptr) = size;
		number_of_heap_allocations.fetch_add(1);
		size_of_heap_allocations.fetch_add(size);
		return ((uint8_t*)ptr + sizeof(size_t));
	}

	static void* AllocateAligned(size_t size, uint8_t alignment
#if RB_MEM_USE_TRACE == 1 
		, const char* source, uint32_t line, uint32_t category)
#else 
		)
#endif
	{
		size_t ns = size + alignment;
		uptr_t rawAddr = (uptr_t)Allocate(ns
#if RB_MEM_USE_TRACE == 1 
			, source, line, category);
#else 
			);
#endif

		if (!rawAddr)
			return nullptr;

		uptr_t misAlign = rawAddr & (alignment - 1);
		uint8_t adjust = alignment - (uint8_t)misAlign;
		uptr_t alignedAddr = rawAddr + adjust;
		uint8_t* a = (uint8_t*)(alignedAddr - sizeof(uint8_t));
		*a = adjust;

		number_of_heap_allocations.fetch_add(1);
		size_of_heap_allocations.fetch_add(ns);
		return (void*)alignedAddr;
	}

	static void* AllocateAligned16(size_t bytes
#if RB_MEM_USE_TRACE == 1 
		, const char* source, uint32_t line, uint32_t category)
#else 
		)
#endif
	{
		return AllocateAligned(bytes, 16
#if RB_MEM_USE_TRACE == 1
			, source, line, category);
#else 
			);
#endif
	}

	static void Deallocate(void* ptr)
	{
		void* ptrReal = ((uint8_t*)ptr - sizeof(size_t));
		::free(ptrReal);
	}

	static void DeallocateAligned(void* ptr)
	{
		uptr_t alignedAddr = (uptr_t)ptr;
		uint8_t adjust = *((uint8_t*)(alignedAddr - sizeof(uint8_t)));
		uptr_t rawAddr = alignedAddr - adjust;
		Deallocate((void*)rawAddr);
	}

	static void DeallocateAligned16(void* ptr)
	{
		DeallocateAligned(ptr);
	}
};

inline void* RB_Allocate(size_t size
#if RB_MEM_USE_TRACE == 1
	, uint32_t memCategory = 0, void* userData = nullptr)
#else 
)
#endif
{
	return MemoryAllocationBase::Allocate(size
#if RB_MEM_USE_TRACE == 1
		, __FILE__, __LINE__, 0);
#else 
		);
#endif
}

template<class _Type>
_Type* RB_AllocateTyped(
#if RB_MEM_USE_TRACE == 1
	uint32_t memCategory = 0, void* userData = nullptr)
#else  
)
#endif
{
	return (_Type*)MemoryAllocationBase::Allocate(sizeof(_Type)
#if RB_MEM_USE_TRACE == 1
		, __FILE__, __LINE__, 0);
#else 
		);
#endif
}

template<class _Type>
_Type* RB_AllocateA(size_t count
#if RB_MEM_USE_TRACE == 1
	, uint32_t memCategory = 0, void* userData = nullptr)
#else  
)
#endif
{
	return (_Type*)MemoryAllocationBase::Allocate(count * sizeof(_Type)
#if RB_MEM_USE_TRACE == 1
		, __FILE__, __LINE__, 0);
#else 
		);
#endif
}

inline void RB_Free(void* ptr)
{
	MemoryAllocationBase::Deallocate(ptr);
}

template<class _Type, class... _Args>
_Type* RB_New(_Args&&... args)
{
	return ::new (RB_AllocateTyped<_Type>()) _Type(std::forward<_Args>(args)...);
}

template<class _Type>
void RB_Delete(_Type* ptr)
{
	(ptr)->~_Type();
	RB_Free(ptr);
}

template<class _Type>
struct Deleter
{
	constexpr Deleter() noexcept = default;

	template <class _Type2, std::enable_if_t<std::is_convertible<_Type2*, _Type*>::value, int32> = 0>
	constexpr Deleter(const Deleter<_Type2>& other) noexcept { }

	void operator()(_Type* ptr) const
	{
		RB_Delete<_Type>(ptr);
	}
};

template<class _Type>
_Type* RB_NewA(size_t count)
{
	_Type* ptr = (_Type*)RB_Allocate(sizeof(_Type) * count);
	for (size_t i = 0; i < count; ++i)
		::new (&ptr[i]) _Type;

	return ptr;
}

template<class _Type>
void RB_DeleteA(_Type* ptr, size_t count)
{
	for (size_t i = 0; i < count; ++i)
		ptr[i].~_Type();

	RB_Free(ptr);
}

template< class _Type >
class StdAlloc
{
public:
	using value_type = _Type;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	constexpr StdAlloc() = default;
	constexpr StdAlloc(StdAlloc&&) = default;
	constexpr StdAlloc(const StdAlloc&) = default;
	template<class _Type1> constexpr StdAlloc(const StdAlloc<_Type1>&) { };
	template<class _Type1> constexpr bool operator==(const StdAlloc<_Type1>&) const noexcept { return true; }
	template<class _Type1> constexpr bool operator!=(const StdAlloc<_Type1>&) const noexcept { return false; }

	template<class _Type1> class rebind { public: using other = StdAlloc<_Type1>; };

	_Type* allocate(const size_t num)
	{
		if (num == 0)
			return nullptr; // Error

		if (num > std::numeric_limits<size_t>::max() / sizeof(_Type))
			return nullptr; // Error

		void* const pAllocated = RB_Allocate(num * sizeof(_Type));
		if (!pAllocated)
			return nullptr; // Error

		return static_cast<_Type*>(pAllocated);
	}

	void deallocate(pointer p, size_type)
	{
		RB_Free(p);
	}

	static constexpr size_t max_size() { return std::numeric_limits<size_type>::max() / sizeof(_Type); }
	static constexpr void destroy(pointer p) { p->~_Type(); }

	template<class... Args>
	static void construct(pointer p, Args&&... args) { ::new(p) _Type(std::forward<Args>(args)...); }
};

template <typename _Type, typename... _Args>
SharedPtr<_Type> MakeShared(_Args &&... args)
{
	return std::allocate_shared<_Type>(StdAlloc<_Type>(), std::forward<_Args>(args)...);
}

template<typename _Type, typename Delete = Deleter<_Type>>
SharedPtr<_Type> AssignShared(_Type* data, Delete del = Delete())
{
	return SharedPtr<_Type>(data, std::move(del), StdAlloc<_Type>());
}

template<typename _Type1, typename _Type2>
SharedPtr<_Type1> StaticCastShared(const SharedPtr<_Type2>& other)
{
	// static_cast for shared_ptr that properly respects the reference count control block
	const auto ptr = static_cast<typename SharedPtr<_Type1>::element_type*>(other.get());
	return SharedPtr<_Type1>(other, ptr);
}

template<typename _Type, typename Delete = Deleter<_Type>>
UniquePtr<_Type, Delete> AssignUnique(_Type* data, Delete del = Delete())
{
	return std::unique_ptr<_Type, Delete>(data, std::move(del));
}

template<typename _Type, typename Delete = Deleter<_Type>, typename... _Args>
UniquePtr<_Type, Delete> MakeUnique(_Args &&... args)
{
	_Type* rawPtr = RB_New<_Type>(std::forward<_Args>(args)...);
	return AssignUnique<_Type, Delete>(rawPtr);
}


#define RB_ALLOCATE(Capacity) RB_Allocate(Capacity)
#define RB_FREE(Ptr) RB_Free(Ptr)

#define RB_NEW(Type) RB_New<Type>()
#define RB_NEW_ARGS(Type, Args) RB_New<Type>((Args))
#define RB_DELETE(Type, Ptr) RB_Delete<Type>(Ptr)

#define RB_NEW_ARRAY(Type, Num) RB_NewA<Type>(Num)
#define RB_DELETE_ARRAY(Type, Num) RB_DeleteA<Type>(Num)

RB_NAMESPACE_END

