#pragma once

#include <array>

namespace obfuscate
{
    template<typename T, T Const>
    struct ForceConstant
    {
        enum : T { value = Const };
    };

    template<typename T1, typename T2, T1 Value>
    struct ForceConstantConversion
    {
        static constexpr T2 value = Value;

        // enum : T2 { value = Value };
    };
}