#ifndef NSN_HDR_META_HAS_DFLT_CTOR
#	define NSN_HDR_META_HAS_DFLT_CTOR

#	include "../meta/same.hpp"

namespace nsn::meta {
	template<typename type_>
	inline constexpr bool has_dflt_ctor = requires {
		{ type_() } -> nsn::meta::same<type_>;
	};
}

#endif
