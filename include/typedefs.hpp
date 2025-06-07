#pragma once

#include <memory>

using uint = unsigned int;

template <typename Type>
using u_ptr = std::unique_ptr<Type>;

template <typename Type>
using asset = std::shared_ptr<Type>;

#define makeU std::make_unique
#define makeS std::make_shared