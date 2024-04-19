#ifndef NSN_HDR_META_COND
#	define NSN_HDR_META_COND

namespace nsn::detail {
	template<bool, typename, typename>
	struct cond;

	template<typename first_, typename second_>
	struct cond<true, first_, second_> {
		using type = first_;
	};

	template<typename first_, typename second_>
	struct cond<false, first_, second_> {
		using type = second_;
	};
}

namespace nsn::meta {
	template<bool condition_, typename first_, typename second_>
	using cond = nsn::detail::cond<condition_, first_, second_>::type;
}

#endif
