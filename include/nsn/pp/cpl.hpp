#ifndef NSN_HEADER_PP_CPL
#	define NSN_HEADER_PP_CPL

#	define NSN_CPL_TYPE_GCC 0
#	define NSN_CPL_VER_MAJOR_GCC 0
#	define NSN_CPL_VER_MINOR_GCC 0
#	define NSN_CPL_VER_PATCH_GCC 0

#	define NSN_CPL_TYPE_CLANG 0
#	define NSN_CPL_VER_MAJOR_CLANG 0
#	define NSN_CPL_VER_MINOR_CLANG 0
#	define NSN_CPL_VER_PATCH_CLANG 0

#	define NSN_CPL_TYPE_MSVC 0
#	define NSN_CPL_VER_MAJOR_MSVC 0
#	define NSN_CPL_VER_MINOR_MSVC 0
#	define NSN_CPL_VER_PATCH_MSVC 0

#	define NSN_CPL_VER_LEAST(compiler, major, minor, patch) (NSN_CPL_TYPE_##compiler && ((NSN_CPL_VER_MAJOR_##compiler > (major)) || (NSN_CPL_VER_MAJOR_##compiler == (major)) && ((NSN_CPL_VER_MINOR_##compiler > (minor)) || (NSN_CPL_VER_MINOR_##compiler == (minor)) && (NSN_CPL_VER_PATCH_##compiler >= (patch)))))
#	define NSN_CPL_VER_MOST(compiler, major, minor, patch) (NSN_CPL_TYPE_##compiler && ((NSN_CPL_VER_MAJOR_##compiler < (major)) || (NSN_CPL_VER_MAJOR_##compiler == (major)) && ((NSN_CPL_VER_MINOR_##compiler < (minor)) || (NSN_CPL_VER_MINOR_##compiler == (minor)) && (NSN_CPL_VER_PATCH_##compiler <= (patch)))))

#	ifdef __GNUC__
#		undef NSN_CPL_TYPE_GCC
#		define NSN_CPL_TYPE_GCC 1

#		undef NSN_CPL_VER_MAJOR_GCC
#		define NSN_CPL_VER_MAJOR_GCC __GNUC__

#		undef NSN_CPL_VER_MINOR_GCC
#		define NSN_CPL_VER_MINOR_GCC __GNUC_MINOR__

#		undef NSN_CPL_VER_PATCH_GCC
#		define NSN_CPL_VER_PATCH_GCC __GNUC_PATCHLEVEL__
#	endif

#	ifdef __clang__
#		undef NSN_CPL_TYPE_CLANG
#		define NSN_CPL_TYPE_CLANG 1

#		undef NSN_CPL_VER_MAJOR_CLANG
#		define NSN_CPL_VER_MAJOR_CLANG __clang_major__

#		undef NSN_CPL_VER_MINOR_CLANG
#		define NSN_CPL_VER_MINOR_CLANG __clang_minor__

#		undef NSN_CPL_VER_PATCH_CLANG
#		define NSN_CPL_VER_PATCH_CLANG __clang_patchlevel__
#	endif


#	ifdef _MSC_VER
#		undef NSN_CPL_TYPE_MSVC
#		define NSN_CPL_TYPE_MSVC 1

#		undef NSN_CPL_VER_MAJOR_MSVC
#		define NSN_CPL_VER_MAJOR_MSVC (_MSC_FULL_VER / 10000000)

#		undef NSN_CPL_VER_MINOR_MSVC
#		define NSN_CPL_VER_MINOR_MSVC (_MSC_FULL_VER % 10000000 / 100000)

#		undef NSN_CPL_VER_PATCH_MSVC
#		define NSN_CPL_VER_PATCH_MSVC (_MSC_FULL_VER % 100000)
#	endif
#endif
