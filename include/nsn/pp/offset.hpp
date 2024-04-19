#ifndef NSN_HDR_PP_OFFSET
#	define NSN_HDR_PP_OFFSET

#	define NSN_OFFSET(type_, member_) __builtin_offsetof(type_, member_)

#endif
