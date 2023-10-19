module;

#include <concepts>


export module la_concepts:operations;

import :scalars;
import :matrices;

export namespace la {

template<typename T, typename Res>
concept operation = std::regular<T> && std::same_as<typename T::result_type, Res>;

template<typename T>
concept value_operand = la::scalar<T> || la::matrix<T>;

template<typename T, typename Res>
concept operand = la::value_operand<T> || la::operation<T, Res>;

}
