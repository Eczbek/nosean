#ifndef NSN_HDR_META_WRAP
#	define NSN_HDR_META_WRAP

namespace nsn::meta {
	template<typename type_>
	struct wrap {
		using type = type_;
	};
}

#endif
