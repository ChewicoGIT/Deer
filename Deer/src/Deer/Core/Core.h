#pragma once
#include <memory>
#include <stdint.h>

namespace Deer {
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using WeakRef = std::weak_ptr<T>;

	// Unique identifier
	using uid = uint32_t;

	namespace Core {
		extern int argc;
		extern char **argv;
	}
}