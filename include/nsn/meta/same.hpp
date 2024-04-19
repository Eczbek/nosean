#ifndef NSN_HDR_META_SAME
#	define NSN_HDR_META_SAME

namespace nsn::detail {
	template<typename, typename>
	struct same {
		static constexpr bool value = false;
	};

	template<typename type_>
	struct same<type_, type_> {
		static constexpr bool value = true;
	};
}

namespace nsn::meta {
	template<typename first_, typename second_>
	concept same = nsn::detail::same<first_, second_>::value;
}

#endif
