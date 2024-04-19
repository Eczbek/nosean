#ifndef NSN_HDR_TYPE
#	define NSN_HDR_TYPE

#	include "./pp/cplr.hpp"

namespace nsn {
	using size = decltype(sizeof(0));

	using ptrdiff = decltype(static_cast<int*>(nullptr) - static_cast<int*>(nullptr));

	enum struct byte
	: unsigned char {};

#	ifdef __STDCPP_FLOAT16_T__

	using f16 = decltype(0.0f16);

#	endif

#	ifdef __STDCPP_FLOAT32_T__

	using f32 = decltype(0.0f32);

#	endif

#	ifdef __STDCPP_FLOAT64_T__

	using f64 = decltype(0.0f64);

#	endif

#	ifdef __STDCPP_FLOAT128_T__

	using f128 = decltype(0.0f128);

#	endif

#	ifdef __STDCPP_BFLOAT16_T__

	using fb16 = decltype(0.0bf16);

#	endif

#	if NSN_CPLR_TYPE_GCC || NSN_CPLR_TYPE_CLANG
#		ifdef __INT8_TYPE__

	using int8 = __INT8_TYPE__;

#		endif

#		ifdef __INT16_TYPE__

	using int16 = __INT16_TYPE__;

#		endif

#		ifdef __INT32_TYPE__

	using int32 = __INT32_TYPE__;

#		endif

#		ifdef __INT64_TYPE__

	using int64 = __INT64_TYPE__;

#		endif

#		ifdef __UINT8_TYPE__

	using uint8 = __UINT8_TYPE__;

#		endif

#		ifdef __UINT16_TYPE__

	using uint16 = __UINT16_TYPE__;

#		endif

#		ifdef __UINT32_TYPE__

	using uint32 = __UINT32_TYPE__;

#		endif

#		ifdef __UINT64_TYPE__

	using uint64 = __UINT64_TYPE__;

#		endif

	using int_least8 = __INT_LEAST8_TYPE__;
	using int_least16 = __INT_LEAST16_TYPE__;
	using int_least32 = __INT_LEAST32_TYPE__;
	using int_least64 = __INT_LEAST64_TYPE__;
	using uint_least8 = __UINT_LEAST8_TYPE__;
	using uint_least16 = __UINT_LEAST16_TYPE__;
	using uint_least32 = __UINT_LEAST32_TYPE__;
	using uint_least64 = __UINT_LEAST64_TYPE__;

	using int_fast8 = __INT_FAST8_TYPE__;
	using int_fast16 = __INT_FAST16_TYPE__;
	using int_fast32 = __INT_FAST32_TYPE__;
	using int_fast64 = __INT_FAST64_TYPE__;
	using uin_fastt8 = __UINT_FAST8_TYPE__;
	using uint_fast16 = __UINT_FAST16_TYPE__;
	using uint_fast32 = __UINT_FAST32_TYPE__;
	using uint_fast64 = __UINT_FAST64_TYPE__;

	using intmax = __INTMAX_TYPE__;
	using uintmax = __UINTMAX_TYPE__;

#		ifdef __INTPTR_TYPE__

	using intptr = __INTPTR_TYPE__;

#		endif

#		ifdef __UINTPTR_TYPE__

	using uintptr = __UINTPTR_TYPE__;

#		endif

	struct max_align {
		alignas(alignof(long long int)) long long int NSN_DETAIL_MAX_ALIGN_INTEGRAL;
		alignas(alignof(long double)) long double NSN_DETAIL_MAX_ALIGN_FRACTIONAL;
	};

#	elif NSN_CPLR_TYPE_MSVC

	using int_least8 = signed char;
	using int_least16 = short int;
	using int_least32 = int;
	using int_least64 = long long int;
	using uint_least8 = unsigned char;
	using uint_least16 = unsigned short int;
	using uint_least32 = unsigned int;
	using uint_least64 = unsigned long long int;

	using int_fast8 = signed char;
	using int_fast16 = int;
	using int_fast32 = int;
	using int_fast64 = long long int;
	using uint_fast8 = unsigned char;
	using uint_fast16 = unsigned int;
	using uint_fast32 = unsigned int;
	using uint_fast64 = unsigned long long int;

	using int8 = signed char;
	using int16 = short int;
	using int32 = int;
	using int64 = long long int;
	using uint8 = unsigned char;
	using uint16 = unsigned short int;
	using uint32 = unsigned int;
	using uint64 = unsigned long long int;

	using intmax = long long int;
	using uintmax = unsigned long long int;

#		ifdef _WIN64

	using intptr = long long int;
	using uintptr = unsigned long long int;

#		else

	using intptr = int;
	using uintptr = unsigned int;

#		endif

	using max_align = double;

#	endif

}

#endif
