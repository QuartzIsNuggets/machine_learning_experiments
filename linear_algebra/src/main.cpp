#include <iostream>

import la_concepts;
import matrix;
import transforms;
import printing;
import combination;
import assembly;

using namespace la;

int main() {
	{
		std::cout << "Zeroes: " << mat::zeroes<double, 3, 4> << '\n' << "Identity: "
				  << mat::identity<float, 4> << '\n';

		constexpr auto m1 = mat::make_row_vector(1.0f, 2.0f, 3.0f);
		constexpr auto m2 = mat::make_column_vector(3.0, 2.0, 1.0);

		constexpr auto square_4x3 = mat::assemble_vectors(
				m1,
				mat::make_row_vector(4.0, 5.0, 6.0),
				mat::make_row_vector(7, 8, 9),
				mat::transpose(m2)
		);
		std::cout << "Assembled rows: " << square_4x3 << ", "
		          << mat::assemble_vectors(mat::transpose(m1), m2, m2) << '\n';

		constexpr auto m1m2 = m1 * m2;
		constexpr auto m2m1 = m2 * m1;
		std::cout << "Multiplications: " << m1 << ", " << m2 << ", " << m1m2 << ", " << m2m1
		          << '\n';
		std::cout << "Equal to itself times identity? " << std::boolalpha
		          << (m2m1 == (m2m1 * mat::make_identity<double, 3>())) << '\n';
	}

	std::cout << '\n';

	{
		constexpr auto n1 = mat::make_matrix<2, 3>(
				1.0, 2.0, 3.0,
				4.0, 5.0, 6.0
		);
		constexpr auto n2 = mat::make_matrix<4, 2>(
				1.0, 2.0,
				3.0, 4.0,
				5.0, 6.0,
				7.0, 8.0
		);
		constexpr auto n2n1 = n2 * n1;
		std::cout << "Multiplications, ep. 2: " << n1 << ", " << n2 << ", " << n2n1 << '\n';

		std::cout << '\n';

		constexpr auto three_times_n2n1 = 3 * n2n1;
		std::cout << "Scalar multiplication: " << three_times_n2n1 << '\n';

		auto three_times_n2n1_runtime_copy = three_times_n2n1;
		auto &[
				a, b, c,
				d, e, f,
				g, h, i,
				k, l, m
		] = three_times_n2n1_runtime_copy;
		std::cout << "Structured binding: " << three_times_n2n1_runtime_copy[0, 0] << ' ';
		a += 1.0;
		std::cout << three_times_n2n1_runtime_copy[0, 0] << '\n';
	}

	{
		//I.1
		constexpr auto factors = mat::make_row_vector(1, 1, -1);
		constexpr auto vec1 = mat::make_column_vector(1, 0, 0, 1);
		constexpr auto vec2 = mat::make_column_vector(0, 0, 1, 0);
		constexpr auto vec3 = mat::make_column_vector(1, 0, 1, 1);
		constexpr auto res = vec::linear_combination(factors, vec1, vec2, vec3);
		std::cout << "Linear combination of " << vec1 << ", " << vec2 << ", " << vec3
				  << " and with the coefficients " << factors << " : " << res << '\n';
	}
	{
		//I.2
		constexpr auto A = mat::make_matrix<3, 3>(
			1, 2, 3,
			4, 5, 6,
			3, 3, 3
		);
		constexpr auto x = mat::make_column_vector(-1, 2, -1);
		constexpr auto y = mat::make_column_vector(-2, 4, -2);
		std::cout << "Ax = " << A * x << ", Ay = " << A * y << ", A * 2(x - y) = "
				  << A * (2 * (x - y)) << '\n';
	}
	{
		//I.4
		constexpr auto A = mat::make_matrix<3, 3>(
				1, 1, 1,
				1, 1, 1,
				1, 1, 1
		);
		constexpr auto x = mat::make_column_vector(1, 1, -2);
		constexpr auto y = mat::make_column_vector(2, 3, -5);
		std::cout << "Ax = " << A * x << ", Ay = " << A * y << '\n';
	}
}
