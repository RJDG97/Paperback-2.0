#pragma once

#include <memory>
#include <cstdint>
#include <functional>

#define BIT(x) (1 << x)
#define TO_STRING(x) #x
#define DEBUG_BREAK() __debugbreak()
#define PPB_BIND(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define PPB_BASIC_BIND(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Paperback
{
	// Type declarations
	using u8 = uint8_t;					// Max Integral Count = 255
	using u16 = uint16_t;				// Max Integral Count = 65535
	using u32 = uint32_t;				// Max Integral Count = 4294967295
	using u64 = uint64_t;				// Max Integral Count = 18446744073709551615

	using EntityID = u32;				// ID of Entity
	using ComponentType = u8;			// "ENUM" of a Component
	using ComponentIndex = u32;			// Index of Component mapped to Entity

	// Unique ptr
	template <typename T>
	using Scope = std::shared_ptr<T>;
	template <typename T, typename ...Args>
	constexpr Scope<T> CreateScope(Args&& ...args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	// Shared ptr
	template <typename T>
	using Ref = std::shared_ptr<T>;
	template <typename T, typename ...Args>
	constexpr Ref<T> CreateRef(Args&& ...args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	// No-Return fn
	template <typename... Args>
	using Action = std::function<void(Args...)>;

	// Return fn
	template <typename Result, typename... Args>
	using Func = std::function<Result(Args...)>;
}