#ifndef NSN_HDR_META_HAS_COPY_CTOR
#	define NSN_HDR_META_HAS_COPY_CTOR

#	include "../meta/same.hpp"

namespace nsn::meta {
	template<typename type_>
	inline constexpr bool has_copy_ctor = requires(type_ value) {
		{ type_(value) } -> nsn::meta::same<type_>;
	};
}

#endif
