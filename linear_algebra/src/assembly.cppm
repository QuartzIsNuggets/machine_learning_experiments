module;

#include <array>
#include <tuple>


export module assembly;

import :shapes;

import la_concepts;
import matrix;


namespace la {

export namespace mat {

template<template<typename, std::size_t, std::size_t> typename TM = mat::Matrix, la::vector ...Vs>
requires ((std::array{Vs::shape...}[0] == Vs::shape) && ...)
[[nodiscard]] constexpr auto assemble_vectors(
		const Vs &...vectors
) noexcept -> mat::assembled_vectors_t<TM, Vs...>;

template<template<typename, std::size_t, std::size_t> typename TM = mat::Matrix, la::matrix M>
[[nodiscard]] constexpr auto disassemble_matrix_columns(
		const M &matrix
) noexcept -> mat::matrix_column_array_t<TM, M>;

template<template<typename, std::size_t, std::size_t> typename TM = mat::Matrix, la::matrix M>
[[nodiscard]] constexpr auto disassemble_matrix_rows(
		const M &matrix
) noexcept -> mat::matrix_row_array_t<TM, M>;

}


template<
        template<typename, std::size_t, std::size_t> typename TM, la::vector ...Vs,
        std::size_t ...Idxs
>
constexpr auto assign(
		std::index_sequence<Idxs...>,
		mat::assembled_vectors_t<TM, Vs...> &res,
		std::size_t index,
		const Vs &...vectors
) -> void {
	if constexpr ((la::row_vector<Vs> && ...))
		((res[Idxs, index] = la::get_vector_element(vectors, index)) , ...);
	else
		((res[index, Idxs] = la::get_vector_element(vectors, index)) , ...);
}

template<template<typename, std::size_t, std::size_t> typename TM, la::vector ...Vs>
requires ((std::array{Vs::shape...}[0] == Vs::shape) && ...)
[[nodiscard]] constexpr auto mat::assemble_vectors(
		const Vs &...vectors
) noexcept -> mat::assembled_vectors_t<TM, Vs...> {
	auto res = mat::assembled_vectors_t<TM, Vs...>{};
	for (std::size_t i = 0; i < std::array{la::vector_length_v<Vs>...}[0]; ++i)
		assign<TM>(std::index_sequence_for<Vs...>{}, res, i, vectors...);
	return res;
}

template<template<typename, std::size_t, std::size_t> typename TM, la::matrix M>
[[nodiscard]] constexpr auto mat::disassemble_matrix_columns(
		const M &matrix
) noexcept -> mat::matrix_column_array_t<TM, M> {
	auto res = mat::matrix_column_array_t<TM, M>{};
	for (std::size_t i = 0; i < la::mat_col_count_v<M>; ++i) {
		for (std::size_t j = 0; j < la::mat_row_count_v<M>; ++j) {
			res[i][j, 0] = matrix[j, i];
		}
	}
	return res;
}

template<template<typename, std::size_t, std::size_t> typename TM, la::matrix M>
[[nodiscard]] constexpr auto mat::disassemble_matrix_rows(
		const M &matrix
) noexcept -> mat::matrix_row_array_t<TM, M> {
	auto res = mat::matrix_row_array_t<TM, M>{};
	for (std::size_t i = 0; i < la::mat_row_count_v<M>; ++i) {
		for (std::size_t j = 0; j < la::mat_col_count_v<M>; ++j) {
			res[i][0, j] = matrix[i, j];
		}
	}
	return res;
}

}
