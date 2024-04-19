#ifndef NSN_HDR_META_HAS_DFLT_CTOR
#	define NSN_HDR_META_HAS_DFLT_CTOR

#	include "../meta/same.hpp"

namespace nsn::meta {
	template<typename type_>
	concept has_dflt_ctor = requires {
		{ type_() } -> nsn::meta::same<type_>;
	};
}

#endif
