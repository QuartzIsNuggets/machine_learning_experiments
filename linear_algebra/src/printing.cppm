module;

#include <ostream>


export module printing;

import la_concepts;


export template<typename CharT, typename Traits, la::matrix M>
auto operator<<(
		std::basic_ostream<CharT, Traits> &ostream,
		const M &m
) noexcept -> std::basic_ostream<CharT, Traits> &;






template<typename CharT, typename Traits, la::matrix M>
auto display_row(
		std::basic_ostream<CharT, Traits> &ostream,
		const M &m,
		std::size_t i
) -> void {
	ostream << '{';
	for (std::size_t j = 0; j < la::mat_col_count_v<M> - 1; ++j)
		ostream << m[i, j] << ", ";
	ostream << m[i, la::mat_col_count_v<M> - 1];
	ostream << '}';
}

template<typename CharT, typename Traits, la::matrix M>
auto operator<<(
		std::basic_ostream<CharT, Traits> &ostream,
		const M &m
) noexcept -> std::basic_ostream<CharT, Traits> & {
	ostream << "Matrix<" << la::mat_row_count_v<M> << ", " << la::mat_col_count_v<M> << ">{";
	for (std::size_t i = 0; i < la::mat_row_count_v<M> - 1; ++i) {
		display_row(ostream, m, i);
		ostream << ", ";
	}
	display_row(ostream, m, la::mat_row_count_v<M> - 1);
	ostream << '}';
	return ostream;
}
