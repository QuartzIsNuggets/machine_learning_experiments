module;

#include <array>


module assembly:shapes;

import la_concepts;


namespace la {

namespace mat {

template<template<typename, std::size_t, std::size_t> typename M, typename ...Vs>
struct assembled_vectors;

template<template<typename, std::size_t, std::size_t> typename M, typename ...Vs>
using assembled_vectors_t = typename assembled_vectors<M, Vs...>::type;


template<template<typename, std::size_t, std::size_t> typename TM, la::matrix M>
using matrix_column_t = TM<typename M::value_type, la::mat_row_count_v<M>, 1>;

template<template<typename, std::size_t, std::size_t> typename TM, la::matrix M>
using matrix_column_array_t = std::array<matrix_column_t<TM, M>, la::mat_col_count_v<M>>;

template<template<typename, std::size_t, std::size_t> typename TM, la::matrix M>
using matrix_row_t = TM<typename M::value_type, 1, la::mat_col_count_v<M>>;

template<template<typename, std::size_t, std::size_t> typename TM, la::matrix M>
using matrix_row_array_t = std::array<matrix_row_t<TM, M>, la::mat_row_count_v<M>>;

}


template<template<typename, std::size_t, std::size_t> typename M, la::row_vector ...Vs>
struct mat::assembled_vectors<M, Vs...> {
	typedef M<
			std::common_type_t<typename Vs::value_type...>,
			sizeof...(Vs),
			std::array{la::vector_length_v<Vs>...}[0]
	> type;
};

template<template<typename, std::size_t, std::size_t> typename M, la::column_vector ...Vs>
struct mat::assembled_vectors<M, Vs...> {
	typedef M<
			std::common_type_t<typename Vs::value_type...>,
			std::array{la::vector_length_v<Vs>...}[0],
			sizeof...(Vs)
	> type;
};

}
