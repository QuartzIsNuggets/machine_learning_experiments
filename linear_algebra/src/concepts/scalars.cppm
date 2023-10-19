module;

#include <concepts>

export module la_concepts:scalars;

export namespace la {

template<typename T>
concept scalar = std::integral<T> || std::floating_point<T>;

}
