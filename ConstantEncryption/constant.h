#pragma once

#include "const_time.h"
#include "const_array.h"
#include "const_hash.h"
#include "const_random.h"

#include <type_traits>
#include <algorithm>
#include <random>
#include <limits>
#include <tuple>

#define SELECT_IMPL(maximum) static_cast<std::size_t>(__LINE__ % static_cast<unsigned>(maximum))

namespace obfuscate
{
    template<typename T1, typename T2>
    struct ConstantTypeConverter
    {
        union TypeData
        {
            constexpr TypeData(T1 v) : A(v) {}
            constexpr TypeData(T2 v) : B(v) {}

            T1 A;
            T2 B;
        };

        static_assert(sizeof(T1) == sizeof(T2), "Invalid data.");
        static_assert(sizeof(TypeData) == sizeof(T1), "Invalid data.");
        static_assert(sizeof(TypeData) == sizeof(T2), "Invalid data.");

        static constexpr T1 GetA(T2 B) 
        {
            return TypeData{ B }.A;
        }

        static constexpr T2 GetB(T1 A)
        {
            return TypeData{ A }.B;
        }
    };

    template<typename T, T value>
    struct Constant;
}

#include "keybuilder.h"
#include "integer.h"
#include "const_string.h"