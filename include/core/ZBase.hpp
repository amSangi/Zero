#pragma once

#include "ZPlatform.hpp"

namespace zero {

	/*********** Types ***********/
	using int8   = signed   char;
	using uint8  = unsigned char;

	using int16  = signed   short;
	using uint16 = unsigned short;

	using int32  = signed   int;
	using uint32 = unsigned int;

#ifdef PLATFORM_WINDOWS
	using int64  = signed   __int64
	using uint64 = unsigned __int64
#else
	using int64  = signed   long long;
	using uint64 = unsigned long long;
#endif

	using size_t = uint32;

} // namespace zero