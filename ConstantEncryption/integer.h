#pragma once

namespace obfuscate
{
    template<typename T>
    struct ConstantInteger
    {
        static_assert(std::is_integral<T>::value, "Integral required.");

        using LimitsT = std::numeric_limits<T>;
        using StorageT = std::remove_const_t<std::make_unsigned_t<T>>;

        static constexpr size_t KeyCount = RANDOM_UINT(StorageT, 4, 12);

        // The reason for the default variable is too stupid to explain...//
        // Will come back to this. Hint for myself: ConstantInteger<uint32> ......
        // How do we call the constructor with a value inline? Can't use ConstantInteger<uint32_t, value>
        // If we try to use ConstantInteger<value> it says, no, we need a type. So we force the compiler's hand without
        // compromising our "value" to the stack. Jeez.
        explicit constexpr ConstantInteger() noexcept : m_keys() { }

        template<T value>
        constexpr T Encode()
        {
            return m_keys.GetEncodedValueConstant<value>();
        }

        __forceinline T Decode(StorageT encoded) noexcept
        {
            volatile T a = static_cast<T>(m_keys.GetDecodedValueRuntime(encoded));
            return a;
        }

        KeyBuilder<StorageT, T, KeyCount> m_keys;
    };
}