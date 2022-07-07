#pragma once

#include <cstdint>
#include "const_time.h"
#include "const_sequence.h"
#include "const_hash.h"

namespace obfuscate
{
    namespace random
    {
        // Generate these here, once...
        static constexpr uint8_t seed8 = static_cast<uint8_t>(__cplusplus * obfuscate::constant::hash::CRC32::WSID(__DATE__)); // static_cast<uint8_t>(time::Get<uint32_t>());
        static constexpr uint16_t seed16 = static_cast<uint8_t>(__cplusplus * obfuscate::constant::hash::CRC32::WSID(__DATE__)); // static_cast<uint16_t>(time::Get<uint32_t>());
        static constexpr uint32_t seed32 = static_cast<uint8_t>(__cplusplus * obfuscate::constant::hash::CRC32::WSID(__DATE__)); // time::Get<uint32_t>();
        static constexpr uint64_t seed64 = static_cast<uint8_t>(__cplusplus * obfuscate::constant::hash::CRC32::WSID(__DATE__)); // time::Get<uint64_t>();
    }
}

#include "const_array.h"
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

    template<typename T>
    struct Constant;
}

#include "keybuilder.h"
#include "integer.h"
#include "const_string.h"