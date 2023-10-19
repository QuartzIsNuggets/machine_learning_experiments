module;

#include <cstddef>


export module transforms;

import la_concepts;


namespace la {

export namespace mat {

template<
        template<typename, std::size_t, std::size_t> typename M,
        la::scalar T, std::size_t R, std::size_t C
>
requires la::sized_matrix<M<T, R, C>, R, C>
[[nodiscard]] constexpr auto transpose(const M<T, R, C> &mat) noexcept -> M<T, C, R>;

}


template<
		template<typename, std::size_t, std::size_t> typename M,
		la::scalar T, std::size_t R, std::size_t C
>
requires la::sized_matrix<M<T, R, C>, R, C>
[[nodiscard]] constexpr auto mat::transpose(const M<T, R, C> &mat) noexcept -> M<T, C, R> {
	la::matrix auto res = M<T, C, R>{};
	for (std::size_t i = 0; i < R; ++i) {
		for (std::size_t j = 0; j < C; ++j) {
			res[j, i] = mat[i, j];
		}
	}
	return res;
}

}
