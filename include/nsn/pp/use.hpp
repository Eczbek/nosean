#ifndef NSN_HEADER_PP_USE
#	define NSN_HEADER_PP_USE

#	define NSN_USE(...) static_cast<void>(static_cast<void>(0) __VA_OPT__(,) __VA_ARGS__)

#endif
