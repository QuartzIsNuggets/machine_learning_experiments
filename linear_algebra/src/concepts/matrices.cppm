module;

#include <array>
#include <tuple>
#include <type_traits>


export module la_concepts:matrices;

import :scalars;

export namespace la {

template<typename T>
concept matrix = std::regular<std::remove_const_t<T>> && la::scalar<typename T::value_type>
		&& std::same_as<
				std::remove_const_t<std::tuple_element_t<0, decltype(T::shape)>>, std::size_t
		>
		&& std::same_as<
				std::remove_const_t<std::tuple_element_t<1, decltype(T::shape)>>, std::size_t
		>
		&& requires(
				std::remove_const_t<T> mat, std::add_const_t<T> const_mat,
				std::size_t i, std::size_t j
		) {
			{ mat[i, j] } -> std::same_as<typename T::value_type &>;
			{ const_mat[i, j] } -> std::same_as<const typename T::value_type &>;
		};

template<typename T, std::size_t R, std::size_t C>
concept sized_matrix = la::matrix<T> && (std::get<0>(T::shape) == R && std::get<1>(T::shape) == C);


template<la::matrix T>
struct mat_row_count : std::integral_constant<std::size_t, std::get<0>(T::shape)> {};

template<la::matrix T>
constexpr auto mat_row_count_v = la::mat_row_count<T>::value;

template<la::matrix T>
struct mat_col_count : std::integral_constant<std::size_t, std::get<1>(T::shape)> {};

template<la::matrix T>
constexpr auto mat_col_count_v = la::mat_col_count<T>::value;

}
