#ifndef NSN_HDR_PP_CPLR
#	define NSN_HDR_PP_CPLR

#	define NSN_CPLR_TYPE_GCC 0
#	define NSN_CPLR_VER_MAJOR_GCC 0
#	define NSN_CPLR_VER_MINOR_GCC 0
#	define NSN_CPLR_VER_PATCH_GCC 0

#	define NSN_CPLR_TYPE_CLANG 0
#	define NSN_CPLR_VER_MAJOR_CLANG 0
#	define NSN_CPLR_VER_MINOR_CLANG 0
#	define NSN_CPLR_VER_PATCH_CLANG 0

#	define NSN_CPLR_TYPE_MSVC 0
#	define NSN_CPLR_VER_MAJOR_MSVC 0
#	define NSN_CPLR_VER_MINOR_MSVC 0
#	define NSN_CPLR_VER_PATCH_MSVC 0

#	define NSN_CPLR_VER_LEAST(type_, major_, minor_, patch_) (NSN_CPLR_TYPE_##type_ && ((NSN_CPLR_VER_MAJOR_##type_ > (major_)) || (NSN_CPLR_VER_MAJOR_##type_ == (major_)) && ((NSN_CPLR_VER_MINOR_##type_ > (minor_)) || (NSN_CPLR_VER_MINOR_##type_ == (minor_)) && (NSN_CPLR_VER_PATCH_##type_ >= (patch_)))))
#	define NSN_CPLR_VER_MOST(type_, major_, minor_, patch_) (NSN_CPLR_TYPE_##type_ && ((NSN_CPLR_VER_MAJOR_##type_ < (major_)) || (NSN_CPLR_VER_MAJOR_##type_ == (major_)) && ((NSN_CPLR_VER_MINOR_##type_ < (minor_)) || (NSN_CPLR_VER_MINOR_##type_ == (minor_)) && (NSN_CPLR_VER_PATCH_##type_ <= (patch_)))))

#	ifdef __GNUC__
#		undef NSN_CPLR_TYPE_GCC
#		define NSN_CPLR_TYPE_GCC 1

#		undef NSN_CPLR_VER_MAJOR_GCC
#		define NSN_CPLR_VER_MAJOR_GCC __GNUC__

#		undef NSN_CPLR_VER_MINOR_GCC
#		define NSN_CPLR_VER_MINOR_GCC __GNUC_MINOR__

#		undef NSN_CPLR_VER_PATCH_GCC
#		define NSN_CPLR_VER_PATCH_GCC __GNUC_PATCHLEVEL__
#	endif

#	ifdef __clang__
#		undef NSN_CPLR_TYPE_CLANG
#		define NSN_CPLR_TYPE_CLANG 1

#		undef NSN_CPLR_VER_MAJOR_CLANG
#		define NSN_CPLR_VER_MAJOR_CLANG __clang_major__

#		undef NSN_CPLR_VER_MINOR_CLANG
#		define NSN_CPLR_VER_MINOR_CLANG __clang_minor__

#		undef NSN_CPLR_VER_PATCH_CLANG
#		define NSN_CPLR_VER_PATCH_CLANG __clang_patchlevel__
#	endif


#	ifdef _MSC_VER
#		undef NSN_CPLR_TYPE_MSVC
#		define NSN_CPLR_TYPE_MSVC 1

#		undef NSN_CPLR_VER_MAJOR_MSVC
#		define NSN_CPLR_VER_MAJOR_MSVC (_MSC_FULL_VER / 10000000)

#		undef NSN_CPLR_VER_MINOR_MSVC
#		define NSN_CPLR_VER_MINOR_MSVC (_MSC_FULL_VER % 10000000 / 100000)

#		undef NSN_CPLR_VER_PATCH_MSVC
#		define NSN_CPLR_VER_PATCH_MSVC (_MSC_FULL_VER % 100000)
#	endif
#endif
