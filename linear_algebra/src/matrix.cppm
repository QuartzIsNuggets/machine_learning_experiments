module;

#include <array>
#include <concepts>
#include <ostream>


export module matrix;

import la_concepts;


export namespace la::mat {


template<la::scalar T, std::size_t R, std::size_t C>
struct Matrix;


template<std::size_t R, std::size_t C, la::scalar ...T>
requires (sizeof...(T) == R * C)
[[nodiscard]] constexpr auto make_matrix(
		T... entries
) noexcept -> Matrix<std::common_type_t<T...>, R, C>;

template<la::scalar ...T>
[[nodiscard]] constexpr auto make_column_vector(
		T... entries
) noexcept -> Matrix<std::common_type_t<T...>, sizeof...(T), 1>;

template<la::scalar ...T>
[[nodiscard]] constexpr auto make_row_vector(
		T... entries
) noexcept -> Matrix<std::common_type_t<T...>, 1, sizeof...(T)>;


template<la::scalar T, std::size_t R, std::size_t C>
[[nodiscard]] consteval auto make_zero() noexcept -> mat::Matrix<T, R, C>;

template<la::scalar T, std::size_t N>
[[nodiscard]] consteval auto make_identity() noexcept -> mat::Matrix<T, N, N>;

template<la::scalar T, std::size_t R, std::size_t C>
constexpr mat::Matrix<T, R, C> zeroes = make_zero<T, R, C>();

template<la::scalar T, std::size_t N>
constexpr mat::Matrix<T, N, N> identity = make_identity<T, N>();


template<la::scalar T, la::scalar U, std::size_t R, std::size_t C>
requires std::convertible_to<U, T>
[[nodiscard]] constexpr auto operator+(
		la::mat::Matrix<T, R, C> a,
		const la::mat::Matrix<U, R, C> &b
) noexcept -> la::mat::Matrix<T, R, C>;

template<la::scalar T, la::scalar U, std::size_t R, std::size_t C>
requires std::convertible_to<U, T>
[[nodiscard]] constexpr auto operator-(
		la::mat::Matrix<T, R, C> a,
		const la::mat::Matrix<U, R, C> &b
) noexcept -> la::mat::Matrix<T, R, C>;

template<la::scalar S, la::scalar T, std::size_t R, std::size_t C>
requires std::common_with<S, T>
[[nodiscard]] constexpr auto operator*(
		S scalar,
		const la::mat::Matrix<T, R, C> &m
) noexcept -> la::mat::Matrix<std::common_type_t<S, T>, R, C>;

template<
		la::scalar T, la::scalar U,
		std::size_t R, std::size_t M, std::size_t C
>
requires std::common_with<T, U>
[[nodiscard]] constexpr auto operator*(
		const la::mat::Matrix<T, R, M> &a,
		const la::mat::Matrix<U, M, C> &b
) noexcept -> la::mat::Matrix<std::common_type_t<T, U>, R, C>;

}




namespace la {

template<la::scalar T, std::size_t R, std::size_t C>
struct mat::Matrix {
	static_assert(R >= 1 && C >= 1, "Matrices should be of non-zero row and column count.");

public:
	typedef T value_type;
	typedef std::array<value_type, R * C> array_type;

public:
	static constexpr std::array<std::size_t, 2> shape = {R, C};

public:
	[[nodiscard]] constexpr Matrix() noexcept;
	[[nodiscard]] explicit constexpr Matrix(const array_type &data) noexcept;
	template<la::scalar U>
	requires std::convertible_to<U, T>
	[[nodiscard]] constexpr Matrix(const Matrix<U, R, C> &other) noexcept;

	[[nodiscard]] constexpr Matrix(const Matrix &other) noexcept = default;
	constexpr Matrix &operator=(const Matrix &other) noexcept = default;
	[[nodiscard]] constexpr Matrix(Matrix &&other) noexcept = default;
	constexpr Matrix &operator=(Matrix &&other) noexcept = default;
	constexpr ~Matrix() noexcept = default;

	[[nodiscard]] constexpr auto operator[](std::size_t i, std::size_t j) noexcept -> T &;
	[[nodiscard]] constexpr auto operator[](
			std::size_t i, std::size_t j
	) const noexcept -> const T &;

	[[nodiscard]] constexpr auto operator==(
			const Matrix<T, R, C> &other
	) const noexcept -> bool = default;
	[[nodiscard]] constexpr auto operator!=(
			const Matrix<T, R, C> &other
	) const noexcept -> bool = default;

	template<la::scalar U>
	requires std::convertible_to<U, T>
	constexpr auto operator+=(const Matrix<U, R, C> &other) & noexcept -> Matrix<T, R, C> &;
	template<la::scalar U>
	requires std::convertible_to<U, T>
	constexpr auto operator-=(const Matrix<U, R, C> &other) & noexcept -> Matrix<T, R, C> &;

	template<std::size_t I>
	[[nodiscard]] constexpr auto get(
	) & noexcept -> std::tuple_element_t<I, mat::Matrix<T, R, C>> &;
	template<std::size_t I>
	[[nodiscard]] constexpr auto get(
	) && noexcept -> std::tuple_element_t<I, mat::Matrix<T, R, C>> &&;
	template<std::size_t I>
	[[nodiscard]] constexpr auto get(
	) const & noexcept -> const std::tuple_element_t<I, mat::Matrix<T, R, C>> &;
	template<std::size_t I>
	[[nodiscard]] constexpr auto get(
	) const && noexcept -> const std::tuple_element_t<I, mat::Matrix<T, R, C>> &&;

private:
	array_type m_data;
};

template<la::scalar T, std::size_t R, std::size_t C>
constexpr mat::Matrix<T, R, C>::Matrix() noexcept : Matrix(std::array<T, R * C>{}) {}

template<la::scalar T, std::size_t R, std::size_t C>
constexpr mat::Matrix<T, R, C>::Matrix(
		const mat::Matrix<T, R, C>::array_type &data
) noexcept : m_data{data} {}

template<la::scalar T, std::size_t R, std::size_t C>
template<la::scalar U>
requires std::convertible_to<U, T>
constexpr mat::Matrix<T, R, C>::Matrix(const mat::Matrix<U, R, C> &other) noexcept : m_data{} {
	for (std::size_t i = 0; i < R; ++i) {
		for (std::size_t j = 0; j < C; ++j) {
			this->operator[](i, j) = static_cast<T>(other[i, j]);
		}
	}
}

template<la::scalar T, std::size_t R, std::size_t C>
constexpr auto mat::Matrix<T, R, C>::operator[](std::size_t i, std::size_t j) noexcept -> T & {
	return m_data[i * C + j];
}

template<la::scalar T, std::size_t R, std::size_t C>
constexpr auto mat::Matrix<T, R, C>::operator[](
		std::size_t i, std::size_t j
) const noexcept -> const T & {
	return m_data[i * C + j];
}

template<la::scalar T, std::size_t R, std::size_t C>
template<la::scalar U>
requires std::convertible_to<U, T>
constexpr auto mat::Matrix<T, R, C>::operator+=(
	const mat::Matrix<U, R, C> &other
) & noexcept -> mat::Matrix<T, R, C> & {
	for (std::size_t i = 0; i < R; ++i) {
		for (std::size_t j = 0; j < C; ++j) {
			this->operator[](i, j) += static_cast<T>(other[i, j]);
		}
	}
	return *this;
}

template<la::scalar T, std::size_t R, std::size_t C>
template<la::scalar U>
requires std::convertible_to<U, T>
constexpr auto mat::Matrix<T, R, C>::operator-=(
	const mat::Matrix<U, R, C> &other
) & noexcept -> mat::Matrix<T, R, C> & {
	for (std::size_t i = 0; i < R; ++i) {
		for (std::size_t j = 0; j < C; ++j) {
			this->operator[](i, j) -= static_cast<T>(other[i, j]);
		}
	}
	return *this;
}

template<la::scalar T, std::size_t R, std::size_t C>
template<std::size_t I>
constexpr auto mat::Matrix<T, R, C>::get(
) & noexcept -> std::tuple_element_t<I, la::mat::Matrix<T, R, C>> & {
	return std::get<I>(m_data);
}

template<la::scalar T, std::size_t R, std::size_t C>
template<std::size_t I>
constexpr auto mat::Matrix<T, R, C>::get(
) && noexcept -> std::tuple_element_t<I, la::mat::Matrix<T, R, C>> && {
	return std::get<I>(m_data);
}

template<la::scalar T, std::size_t R, std::size_t C>
template<std::size_t I>
constexpr auto mat::Matrix<T, R, C>::get(
) const & noexcept -> const std::tuple_element_t<I, la::mat::Matrix<T, R, C>> & {
	return std::get<I>(m_data);
}

template<la::scalar T, std::size_t R, std::size_t C>
template<std::size_t I>
constexpr auto mat::Matrix<T, R, C>::get(
) const && noexcept -> const std::tuple_element_t<I, la::mat::Matrix<T, R, C>> && {
	return std::get<I>(m_data);
}


template<std::size_t R, std::size_t C, la::scalar ...T>
requires (sizeof...(T) == R * C)
constexpr auto mat::make_matrix(
		T... entries
) noexcept -> mat::Matrix<std::common_type_t<T...>, R, C> {
	return mat::Matrix<std::common_type_t<T...>, R, C>{
		std::array<std::common_type_t<T...>, R * C>{entries...}
	};
}

template<la::scalar ...T>
constexpr auto mat::make_column_vector(
		T... entries
) noexcept -> mat::Matrix<std::common_type_t<T...>, sizeof...(T), 1> {
	return mat::make_matrix<sizeof...(T), 1, T...>(entries...);
}

template<la::scalar ...T>
constexpr auto mat::make_row_vector(
		T... entries
) noexcept -> mat::Matrix<std::common_type_t<T...>, 1, sizeof...(T)> {
	return mat::make_matrix<1, sizeof...(T), T...>(entries...);
}


template<la::scalar T, std::size_t R, std::size_t C>
consteval auto mat::make_zero() noexcept -> mat::Matrix<T, R, C> {
	return mat::Matrix<T, R, C>{};
}

template<la::scalar T, std::size_t N>
consteval auto mat::make_identity() noexcept -> mat::Matrix<T, N, N> {
	auto data = std::array<T, N * N>{};
	for (std::size_t i = 0; i < N; ++i) {
		data[i * N + i] = T{1};
	}
	return mat::Matrix<T, N, N>{data};
}


template<la::scalar T, la::scalar U, std::size_t R, std::size_t C>
requires std::convertible_to<U, T>
constexpr auto mat::operator+(
		la::mat::Matrix<T, R, C> a,
		const la::mat::Matrix<U, R, C> &b
) noexcept -> la::mat::Matrix<T, R, C> {
	a += b;
	return a;
}

template<la::scalar T, la::scalar U, std::size_t R, std::size_t C>
requires std::convertible_to<U, T>
constexpr auto mat::operator-(
		la::mat::Matrix<T, R, C> a,
		const la::mat::Matrix<U, R, C> &b
) noexcept -> la::mat::Matrix<T, R, C> {
	a -= b;
	return a;
}

template<la::scalar S, la::scalar T, std::size_t R, std::size_t C>
requires std::common_with<S, T>
constexpr auto mat::operator*(
		S scalar,
		const la::mat::Matrix<T, R, C> &m
) noexcept -> la::mat::Matrix<std::common_type_t<S, T>, R, C> {
	auto res = la::mat::Matrix<std::common_type_t<S, T>, R, C>{};
	for (std::size_t i = 0; i < R; ++i) {
		for(std::size_t j = 0; j < C; ++j)
			res[i, j] = static_cast<std::common_type_t<S, T>>(scalar) * m[i, j];
	}
	return res;
}

template<la::scalar T, la::scalar U, std::size_t R, std::size_t M, std::size_t C>
requires std::common_with<T, U>
constexpr auto mat::operator*(
		const la::mat::Matrix<T, R, M> &a,
		const la::mat::Matrix<U, M, C> &b
) noexcept -> la::mat::Matrix<std::common_type_t<T, U>, R, C> {
	auto res = la::mat::Matrix<std::common_type_t<T, U>, R, C>{};
	for (std::size_t i = 0; i < R; ++i) {
		for (std::size_t j = 0; j < C; ++j) {
			for (std::size_t k = 0; k < M; ++k) {
				res[i, j] += static_cast<std::common_type_t<T, U>>(a[i, k]) * b[k, j];
			}
		}
	}
	return res;
}

}

template<la::scalar T, std::size_t R, std::size_t C>
struct std::tuple_size<la::mat::Matrix<T, R, C>>
		: std::tuple_size<typename la::mat::Matrix<T, R, C>::array_type> {};

template<std::size_t I, la::scalar T, std::size_t R, std::size_t C>
struct std::tuple_element<I, la::mat::Matrix<T, R, C>>
		: std::tuple_element<I, typename la::mat::Matrix<T, R, C>::array_type> {};
