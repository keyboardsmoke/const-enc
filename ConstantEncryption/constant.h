#pragma once

#include <cstdint>
#include "const_time.h"
#include "const_hash.h"

namespace obfuscate
{
    // TODO: These seeds are generated once, but the keys are not. Yet they start from the same seed.
    // Wondering if rolling this somehow is possible.
    // Why: If we use time in headers, key builder may not produce consistent results
    // Maybe omitting the second could mitigate that but, who cares. We just need a solid seed. Random should be able to roll it well.
    namespace random
    {
        // Generate these here, once...
        static constexpr uint64_t master_seed = __cplusplus * obfuscate::constant::hash::CRC64::WSID(__DATE__);

        static constexpr uint8_t seed8 = static_cast<uint8_t>(master_seed);
        static constexpr uint16_t seed16 = static_cast<uint16_t>(master_seed);
        static constexpr uint32_t seed32 = static_cast<uint32_t>(master_seed);
        static constexpr uint64_t seed64 = static_cast<uint64_t>(master_seed);
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

#include "const_keybuilder.h"
#include "const_integer.h"
#include "const_string.h"