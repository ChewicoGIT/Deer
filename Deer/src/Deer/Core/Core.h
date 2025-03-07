#pragma once
#include <stdint.h>

namespace Deer {
	// Unique identifier
	using uid = uint32_t;

	namespace Core {
		extern int argc;
		extern char **argv;
	}
}