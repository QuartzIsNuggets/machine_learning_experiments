module;

#include <concepts>
#include <tuple>


export module combination;


import la_concepts;

namespace la {

namespace vec {

export template<
		std::size_t FR, std::size_t FC,
		la::scalar F, template<typename, std::size_t, std::size_t> typename Factors,
		std::size_t VR, std::size_t VC,
		la::scalar ...V, template<typename, std::size_t, std::size_t> typename ...Vectors
>
requires (std::common_with<F, V> && ...)
		&& la::vector<Factors<F, FR, FC>> && (la::vector<Vectors<V, VR, VC>> && ...)
		&& (la::vector_length_v<Factors<F, FR, FC>> == sizeof...(Vectors))
[[nodiscard]] constexpr auto linear_combination(
		const Factors<F, FR, FC> &factors,
		const Vectors<V, VR, VC> &...vectors
) noexcept -> Factors<std::common_type_t<F, V...>, VR, VC>;

}

template<la::vector Factors, la::vector ...Vectors, la::vector Res, std::size_t ...Idxs>
constexpr auto entry_combination(
		std::index_sequence<Idxs...>,
		Res &res, std::size_t index,
		const Factors &factors,
		const Vectors &...vectors
) noexcept -> void {
	la::get_vector_element(res, index)
			= ((la::get_vector_element(factors, Idxs) *
			    la::get_vector_element(vectors, index)) + ...);
}

template<
		std::size_t FR, std::size_t FC,
		la::scalar F, template<typename, std::size_t, std::size_t> typename Factors,
		std::size_t VR, std::size_t VC,
		la::scalar ...V, template<typename, std::size_t, std::size_t> typename ...Vectors
>
requires (std::common_with<F, V> && ...)
		&& la::vector<Factors<F, FR, FC>> && (la::vector<Vectors<V, VR, VC>> && ...)
		&& (la::vector_length_v<Factors<F, FR, FC>> == sizeof...(Vectors))
[[nodiscard]] constexpr auto vec::linear_combination(
		const Factors<F, FR, FC> &factors,
		const Vectors<V, VR, VC> &...vectors
) noexcept -> Factors<std::common_type_t<F, V...>, VR, VC> {
	auto res = Factors<std::common_type_t<F, V...>, VR, VC>{};
	for (std::size_t i = 0; i < std::array{la::vector_length_v<Vectors<V, VR, VC>>...}[0]; ++i) {
		entry_combination(std::index_sequence_for<V...>(), res, i, factors, vectors...);
	}
	return res;
}

}
