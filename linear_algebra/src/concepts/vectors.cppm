module;

#include <algorithm>
#include <type_traits>


export module la_concepts:vectors;

import :matrices;

export namespace la {

template<typename T>
concept column_vector = la::matrix<T>
                        && (la::mat_col_count_v<T> == 1) && (la::mat_row_count_v<T> > 1);

template<typename T, std::size_t R>
concept sized_column_vector = la::column_vector<T> && (la::mat_row_count_v<T> == R);

template<typename T>
concept row_vector = la::matrix<T> && (la::mat_row_count_v<T> == 1) && (la::mat_col_count_v<T> > 1);

template<typename T, std::size_t C>
concept sized_row_vector = la::row_vector<T> && (la::mat_col_count_v<T> == C);

template<typename T>
concept vector = la::column_vector<T> || la::row_vector<T>;

template<typename T, std::size_t N>
concept sized_vector = la::sized_column_vector<T, N> || la::sized_row_vector<T, N>;

template<la::vector T>
struct vector_length : std::integral_constant<
		std::size_t, std::max(la::mat_row_count_v<T>, la::mat_col_count_v<T>)
> {
};

template<la::vector T>
constexpr auto vector_length_v = la::vector_length<T>::value;


template<la::vector T>
[[nodiscard]] constexpr auto get_vector_element(
		T &vector, std::size_t idx
) noexcept -> typename T::value_type &;

template<la::vector T>
[[nodiscard]] constexpr auto get_vector_element(
		const T &vector, std::size_t idx
) noexcept -> const typename T::value_type &;

}

template<la::vector T>
[[nodiscard]] constexpr auto la::get_vector_element(
		T &vector, std::size_t idx
) noexcept -> typename T::value_type & {
	if constexpr (la::row_vector<T>)
		return vector[0, idx];
	else
		return vector[idx, 0];
}

template<la::vector T>
[[nodiscard]] constexpr auto la::get_vector_element(
		const T &vector, std::size_t idx
) noexcept -> const typename T::value_type & {
	if constexpr (la::row_vector<T>)
		return vector[0, idx];
	else
		return vector[idx, 0];
}
