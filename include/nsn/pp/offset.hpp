#ifndef NSN_HEADER_PP_OFFSET
#	define NSN_HEADER_PP_OFFSET

#	define NSN_OFFSET(type_, member_) __builtin_offsetof(type_, member_)

#endif
