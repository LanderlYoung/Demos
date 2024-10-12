/*************************************************************************
  > File Name:     old_lib.h
  > Author:        Landerl Young
  > Mail:          LanderlYoung@gmail.com
  > Created Time:  Tue Feb 21 13:40:16 2023
 ************************************************************************/

#pragma once

#include <cstdint>

namespace mymodule {

// plain add
std::int32_t add(std::int32_t a, std::int32_t b) noexcept;

// template
template<typename T>
T addT(T a, T b) noexcept {
    return a + b;
}

}
